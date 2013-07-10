#include "data.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <sstream>

using blieng::Data;

Data *Data::__data_instance = NULL;

Data *Data::getInstance()
{
    if (__data_instance == NULL) {
        __data_instance = new Data();
    }
    return __data_instance;
}

Data::Data()
{
    data_path = findDataPath();
    gen = new boost::random::random_device();
}

boost::filesystem::path *Data::findDataPath()
{
    std::list<std::string> locations;
    locations.push_back("");
    locations.push_back("./");
    locations.push_back("/usr/share/zombiebli");
    locations.push_back("../");
    locations.push_back(".\\");
    locations.push_back("..\\");

    boost::filesystem::path *my_data_path = new boost::filesystem::path;
    BOOST_FOREACH(std::string item, locations) {
        *my_data_path = (item + "data");
        if (boost::filesystem::exists(*my_data_path) && boost::filesystem::is_directory(*my_data_path)) {
            return my_data_path;
        }
    }
    delete my_data_path;
    return NULL;
}

std::string Data::findFileRecursive(const boost::filesystem::path &dir_path, std::string name)
{
    if (!boost::filesystem::exists(dir_path)) return "";

    boost::filesystem::directory_iterator end_iter;
    boost::filesystem::directory_iterator dir_iter(dir_path);
    
    for (;dir_iter != end_iter; dir_iter++) {
        if (boost::filesystem::is_directory(dir_iter->status())) {
            std::string res = findFileRecursive(dir_iter->path(), name);
            if (res != "") return res;
        } else if (dir_iter->path().filename() == name) {
            return dir_iter->path().string();
        }
    }
    return "";
}

std::string Data::findFile(std::string name)
{
    if (data_path == NULL) return "";
    return findFileRecursive(*data_path, name);
}

std::vector<std::string> Data::findFileExtRecursive(std::vector<std::string> mapfiles, const boost::filesystem::path &dir_path, std::string ext)
{
    if (!boost::filesystem::exists(dir_path)) return mapfiles;

    boost::filesystem::directory_iterator end_iter;
    boost::filesystem::directory_iterator dir_iter(dir_path);
    
    for (;dir_iter != end_iter; dir_iter++) {
        if (boost::filesystem::is_directory(dir_iter->status())) {
            mapfiles = findFileExtRecursive(mapfiles, dir_iter->path(), ext);
        } else if (dir_iter->path().extension() == ext) {
            mapfiles.push_back(dir_iter->path().filename().string());
        }
    }
    return mapfiles;
}

std::vector<std::string> Data::listMaps()
{
    std::vector<std::string> mapfiles;

    if (!boost::filesystem::exists(*data_path)) return mapfiles;

    boost::filesystem::path maps_path = *data_path;
    maps_path /= "maps"; // FIXME Hardcoded

    if (!boost::filesystem::exists(maps_path)) return mapfiles;

    return findFileExtRecursive(mapfiles, maps_path, ".json");
}

boost::filesystem::path Data::solveFilePath(std::string name)
{
    boost::filesystem::path first_path = *data_path;
    boost::filesystem::path second_path = boost::filesystem::path(name);
    first_path /= name;
    if (!boost::filesystem::exists(first_path) && boost::filesystem::exists(second_path)) {
        first_path = second_path;
    }
    return first_path;
}

std::vector<std::string> Data::readLinesFromFile(std::string name)
{
    std::vector<std::string> tmp;
    if (data_path == NULL) return tmp;
    
    boost::filesystem::path first_path = solveFilePath(name);
    if (boost::filesystem::exists(first_path)) {
        boost::filesystem::ifstream fd(first_path);
        while (!fd.eof()) {
            std::string line;
            std::getline(fd, line);
            boost::algorithm::trim(line);
            if (line.length() > 0) {
                tmp.push_back(line);
            }
        }
        fd.close();
    }

    return tmp;
}

std::string Data::readString(std::string name)
{
    std::string res = "";
    if (data_path == NULL) return res;
    
    boost::filesystem::path first_path = solveFilePath(name);
    if (boost::filesystem::exists(first_path)) {
        boost::filesystem::ifstream fd(first_path, std::ifstream::binary);
        while (!fd.eof()) {
            char tmp[256];
            fd.read(tmp, 255);
            //tmp[255] = 0;
            unsigned int cnt = fd.gcount();
            res.append(tmp, cnt);
            //res += tmp;
        }
        fd.close();
    }

    return res;
}

Json::Value Data::readJson(std::string name)
{
    std::string datas = Data::getInstance()->readString(name);

    Json::Reader reader;
    Json::Value val;
    bool parse_ok;
    parse_ok = reader.parse(datas, val);
    if (!parse_ok) {
    std::cout << "Parse error: " << reader.getFormatedErrorMessages() << "!\n";
        throw "JSON parse error";	
    }
    return val;
}

std::vector<std::string> Data::getJsonKeys(Json::Value val)
{
    return val.getMemberNames();
}

bool Data::isJsonKey(Json::Value val, std::string key)
{
    if (val.isObject()) {
        return val.isMember(key);
    }
    return false;
}

Json::Value Data::getJsonValue(Json::Value val, std::string key)
{
    if (val.isObject()) {
        if (val.isMember(key)) {
            Json::Value res;
            return val.get(key, res);
        }
    }
    return val;
}

std::string Data::formatString(std::string replace_string, unsigned int num)
{
    std::ostringstream stream;
    stream << num;
    std::string numstr = stream.str();

    std::string res = "";
    bool num_replace = false;
    unsigned int cnt = 0;
    for (unsigned int i=0; i<replace_string.length(); i++) {
        if (replace_string[i] == '#') {
            num_replace = true;
            cnt++;
        } else {
            if (num_replace) {
                while (numstr.length() < cnt) {
                    numstr = "0" + numstr;
                }
                res += numstr;
                num_replace = false;
                cnt = 0;
                numstr = "";
            }
            res += replace_string[i];
        }
    }
    return res;
}
