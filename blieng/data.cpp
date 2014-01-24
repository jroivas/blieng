#include "data.h"
#include "bliany.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <algorithm>

using blieng::Data;

class blieng::DataBuffer
{
public:
    DataBuffer(std::string _name, char *_data, unsigned int _len) : name(_name), data(_data), len(_len) {}
    ~DataBuffer() {
        delete data;
    }

    std::string name;
    char *data;
    unsigned int len;
};

Data::Data()
{
    data_path = findDataPath();

    initialize("", nullptr, 0);
}

Data::~Data()
{
}

bool Data::initialize(std::string datafilename, const char *key, unsigned int key_len)
{
    if (!datafile.get() || datafilename != "") {
        if (datafilename == "") {
            data_file_path = findDataFile();
        } else {
            data_file_path = findDataFile(datafilename);
        }
        if (data_file_path.get()) {
            datafile = std::unique_ptr<blieng::DataFile>(new blieng::DataFile(data_file_path->string()));
        }
    }
    bool res = false;
    if (datafile.get()) {
        res = datafile->read(key, key_len);
    }
#ifndef ANDROID
    if (!res && data_path.get()) {
        res = true;
    }
#endif
    return res;
}

bool Data::initialize(const char *key, unsigned int key_len)
{
    return initialize("", key, key_len);
}

std::unique_ptr<boost::filesystem::path> Data::findDataFile(std::string datafilename)
{
#ifdef ANDROID
    std::unique_ptr<boost::filesystem::path> my_data_path(new boost::filesystem::path);
    *my_data_path.get() = "assets/data/data.dat";
    return my_data_path;
#else
    std::list<std::string> locations;
    locations.push_back("");
    locations.push_back("./");
    if (!game_location.empty()) locations.push_back(game_location);
    locations.push_back("../");
    locations.push_back(".\\");
    locations.push_back("..\\");

    std::unique_ptr<boost::filesystem::path> my_data_path(new boost::filesystem::path);

    BOOST_FOREACH(std::string item, locations) {
        *my_data_path.get() = (item + datafilename).c_str();
        if (boost::filesystem::exists(*my_data_path.get()) && boost::filesystem::is_regular_file(*my_data_path.get())) {
            return my_data_path;
        }
    }

    my_data_path.reset();
    return my_data_path;
#endif
}

std::unique_ptr<boost::filesystem::path> Data::findDataPath()
{
    std::list<std::string> locations;
    locations.push_back("");
    locations.push_back("./");
    if (!game_location.empty()) locations.push_back(game_location);
    locations.push_back("../");
    locations.push_back(".\\");
    locations.push_back("..\\");

    std::unique_ptr<boost::filesystem::path> my_data_path(new boost::filesystem::path);

#ifndef ANDROID
    BOOST_FOREACH(std::string item, locations) {
        *my_data_path.get() = (item + "data").c_str();
        if (boost::filesystem::exists(*my_data_path.get()) && boost::filesystem::is_directory(*my_data_path.get())) {
            return my_data_path;
        }
    }
#endif
    my_data_path.reset();
    return my_data_path;
}

bool Data::fileExists(std::string name)
{
#ifdef ANDROID
    return false;
#else
    return boost::filesystem::exists(name) && boost::filesystem::is_regular_file(name);
#endif
}

std::string Data::findFileRecursive(const boost::filesystem::path &dir_path, std::string name)
{
#ifndef ANDROID
    if (!boost::filesystem::exists(dir_path)) return "";

    boost::filesystem::directory_iterator end_iter;
    boost::filesystem::directory_iterator dir_iter(dir_path);

    for (;dir_iter != end_iter; dir_iter++) {
        if (boost::filesystem::is_directory(dir_iter->status())) {
            std::string res = findFileRecursive(dir_iter->path(), name);
            if (res != "") return res;
        } else if (dir_iter->path().filename().string() == name) {
            return dir_iter->path().string();
        }
    }
#endif
    return "";
}

std::string Data::findFileFromDataFile(std::string name)
{
    BOOST_FOREACH(std::string fname, datafile->listFiles()) {
        if (fname == name) {
            return fname;
        }

        boost::filesystem::path handle(fname.c_str());
        // Find "recursively"
        if (handle.filename() == name) {
            return fname;
        }
    }
    return "";
}

std::string Data::findFile(std::string name)
{
    if (datafile.get()) {
        std::string res = findFileFromDataFile(name);
        if (res != "") return res;
    }

#ifndef ANDROID
    if (!data_path.get()) return "";
#endif
    return findFileRecursive(*data_path, name);
}

std::vector<std::string> Data::findFileExtFromDataFile(std::string path, std::string ext)
{
    std::vector<std::string> res;
    BOOST_FOREACH(std::string fname, datafile->listFiles()) {
        if (fname.substr(0, path.length()) == path) {
            boost::filesystem::path handle(fname.c_str());
            if (handle.extension() == ext) {
                res.push_back(handle.filename().string());
            }
        }
    }
    return res;
}

std::vector<std::string> Data::findFileExtRecursive(std::vector<std::string> mapfiles, const boost::filesystem::path &dir_path, std::string ext)
{
#ifndef ANDROID
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
#endif
    return mapfiles;
}

std::vector<std::string> Data::listMaps()
{
    std::vector<std::string> mapfiles;

    if (datafile.get()) {
        std::vector<std::string> res = findFileExtFromDataFile("data/maps/", ".json"); // FIXME Hardcoded
        if (!res.empty()) return res;
    }

#ifndef ANDROID
    if (!boost::filesystem::exists(*data_path)) return mapfiles;

    boost::filesystem::path maps_path = *data_path;
    maps_path /= "maps"; // FIXME Hardcoded

    if (!boost::filesystem::exists(maps_path)) return mapfiles;

    return findFileExtRecursive(mapfiles, maps_path, ".json");
#else
    return mapfiles;
#endif
}

bool Data::saveMapJSON(std::string name, std::string json)
{
#ifdef ANDROID
    return false;
#else
    // TODO Do we need data file support here? Probably not..
    if (!data_path.get()) {
        std::cerr << "Data path not found, please create it in order to save maps" << "\n";
        return false;
    }
    if (!boost::filesystem::exists(*data_path)) return false;

    boost::filesystem::path maps_path = *data_path;
    maps_path /= "maps"; // FIXME Hardcoded

    if (!boost::filesystem::exists(maps_path)) {
        if (!boost::filesystem::create_directory(maps_path)) return false;
    }

    std::string clean_name = "";
    auto si = name.cbegin();
    while (si != name.cend()) {
        if (std::isalnum(*si) || *si == '_' || *si == '-') {
            clean_name += *si;
        }
        //if (*si == '.') break; // FIXME What to do with dots?
        ++si;
    }

    if (clean_name == "") return false;

    maps_path /= clean_name;

    if (!boost::filesystem::exists(maps_path)) {
        if (!boost::filesystem::create_directory(maps_path)) return false;
    }

    maps_path /= clean_name + ".json";
    boost::filesystem::ofstream fd(maps_path, std::ios_base::out | std::ios_base::trunc | std::ofstream::binary);
    if (!fd.is_open()) return false;

    fd << json;
    fd.close();

    return true;
#endif
}

boost::filesystem::path Data::solveFilePath(std::string name)
{
#ifdef ANDROID
    boost::filesystem::path p;
    return p;
#else
    boost::filesystem::path first_path = *data_path;
    boost::filesystem::path second_path = boost::filesystem::path(name.c_str());
    first_path /= name.c_str();
    if (!boost::filesystem::exists(first_path) && boost::filesystem::exists(second_path)) {
        first_path = second_path;
    }
    return first_path;
#endif
}

std::vector<std::string> Data::readLinesFromFile(std::string name)
{
    std::vector<std::string> tmp;

#ifndef ANDROID
    if (data_path.get()) {
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
            return tmp;
        }
    }
#endif

    if (datafile.get()) {
        // FIXME: Hardcoded "data" path
        const blieng::DataFile::DataFileObject *obj = datafile->getObject("data/" + name);
        if (obj == nullptr) obj = datafile->getObject(name);
        if (obj != nullptr) {
            unsigned int pos = 0;
            std::string line;
            while (pos < obj->length()) {
                if (obj->get()[pos] == '\n') {
                    tmp.push_back(line);
                    line = "";
                } else {
                    line += obj->get()[pos];
                }
                pos++;
            }
            if (line.size() > 0) {
                tmp.push_back(line);
            }
            return tmp;
        }
    }

    return tmp;
}

unsigned int Data::readDataFromDataPath(std::string name, char **data)
{
#ifndef ANDROID
    BOOST_FOREACH(blieng::DataBuffer *buf, __buffers)
    {
        if (buf->name == name) {
            if (data != nullptr) {
                *data = buf->data;
            }
            return buf->len;
        }
    }

    boost::filesystem::path first_path = solveFilePath(name);
    if (boost::filesystem::exists(first_path)) {
        boost::filesystem::ifstream fd(first_path, std::ifstream::binary);

        #define BUFSIZE 1024
        char *buffer = new char[BUFSIZE];
        char *tmp = buffer;
        unsigned int totalsize = 0;
        while (!fd.eof() && fd.good()) {
            if (fd.rdbuf()->in_avail() <= 0) break;
            int cnt = fd.readsome(tmp, BUFSIZE);
            if (cnt > 0) {
                totalsize += static_cast<unsigned int>(cnt);

                char *new_buffer = new char[totalsize + BUFSIZE];
                memmove(new_buffer, buffer, totalsize);
                delete [] buffer;
                buffer = new_buffer;

                tmp = buffer + totalsize;
            }
        }
        #undef BUFSIZE
        fd.close();

        if (data != nullptr) {
            unique_ptr<blieng::DataBuffer> tmp_ptr(new blieng::DataBuffer(name, buffer, totalsize));
            __buffers.push_back(std::move(tmp_ptr));
            *data = __buffers.back()->data;
        } else {
            delete [] buffer;
        }

        return totalsize;
    }
#endif

    return 0;
}

unsigned int Data::readData(std::string name, char **data)
{
#ifndef ANDROID
    if (data_path.get()) {
        unsigned int res = readDataFromDataPath(name, data);
        if (res > 0) return res;
    }
#endif

    if (datafile.get()) {
        // FIXME: Hardcoded data path
        const blieng::DataFile::DataFileObject *obj = datafile->getObject("data/" + name);
        if (obj == nullptr) obj = datafile->getObject(name);
        if (obj != nullptr) {
            if (data != nullptr) *data = obj->get();
            return obj->length();
        }
    }

    return 0;
}

std::string Data::readString(std::string name)
{
    std::string res = "";

#ifndef ANDROID
    if (data_path.get()) {
        boost::filesystem::path first_path = solveFilePath(name);
        if (boost::filesystem::exists(first_path)) {
            boost::filesystem::ifstream fd(first_path, std::ifstream::binary);
            while (!fd.eof()) {
                char tmp[256];
                fd.read(tmp, 255);
                unsigned int cnt = static_cast<unsigned int>(fd.gcount());
                res.append(tmp, cnt);
            }
            fd.close();
            return res;
        }
    }
#endif

    if (datafile.get()) {
        //FIXME
        const blieng::DataFile::DataFileObject *obj = datafile->getObject("data/" + name);
        if (obj == nullptr) obj = datafile->getObject(name);
        if (obj != nullptr) {
            res.append(obj->get(), obj->length());
            return res;
        }
    }


    return res;
}

json_value *Data::readJson(const std::string &name)
{
    std::string datas = readString(name);

    json_value *val = json_parse(datas.c_str(), datas.length());
    if (val == nullptr) {
        doDebug("Parse error while parsing '" + name + "'!");
        throw "JSON parse error";
    }
    return val;
}

std::vector<std::string> Data::getJsonKeys(const json_value *val) const
{
    BOOST_ASSERT( val != nullptr );
    return val->getMemberNames();
}

bool Data::isJsonKey(json_value *val, std::string key)
{
    if (val->isObject()) {
        return val->isMember(key);
    }
    return false;
}

const json_value *Data::getJsonValue(const json_value *val, std::string key) const
{
    if (val->isObject()) {
        if (val->isMember(key)) {
            const json_value *res = val->getMember(key);
            if (res != nullptr) return res;
        }
    }
    return val;
}

std::string Data::formatString(std::string replace_string, unsigned int num) const
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
