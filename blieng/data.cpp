#include "data.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <algorithm>

using blieng::Data;

Data *Data::__data_instance = NULL;

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

Data *Data::getInstance()
{
    if (__data_instance == NULL) {
        __data_instance = new Data();
    }
    return __data_instance;
}

Data::Data()
{
    gen = new boost::random::random_device();
    data_path = findDataPath();
    //datafile = NULL;

    initialize("", NULL, 0);
}

Data::~Data()
{
    if (gen != NULL) {
        delete gen;
    }
    __data_instance = NULL;
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
            datafile = std::unique_ptr<blieng::DataFile>(new blieng::DataFile(data_file_path->filename().string()));
        }
    }
    bool res = false;
    if (datafile.get()) {
        res = datafile->read(key, key_len);
    }
    if (!res && data_path.get()) {
        res = true;
    }
    return res;
}

bool Data::initialize(const char *key, unsigned int key_len)
{
    return initialize("", key, key_len);
}

std::unique_ptr<boost::filesystem::path> Data::findDataFile(std::string datafilename)
{
    std::list<std::string> locations;
    locations.push_back("");
    locations.push_back("./");
    locations.push_back("/usr/share/zombiebli/");
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
}

std::unique_ptr<boost::filesystem::path> Data::findDataPath()
{
    std::list<std::string> locations;
    locations.push_back("");
    locations.push_back("./");
    locations.push_back("/usr/share/zombiebli/");
    locations.push_back("../");
    locations.push_back(".\\");
    locations.push_back("..\\");

    std::unique_ptr<boost::filesystem::path> my_data_path(new boost::filesystem::path);

    BOOST_FOREACH(std::string item, locations) {
        *my_data_path.get() = (item + "data").c_str();
        if (boost::filesystem::exists(*my_data_path.get()) && boost::filesystem::is_directory(*my_data_path.get())) {
            return my_data_path;
        }
    }
    my_data_path.reset();
    return my_data_path;
}

bool Data::fileExists(std::string name)
{
    return boost::filesystem::exists(name) && boost::filesystem::is_regular_file(name);
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
        } else if (dir_iter->path().filename().string() == name) {
            return dir_iter->path().string();
        }
    }
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

    if (!data_path.get()) return "";
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

    if (datafile.get()) {
        std::vector<std::string> res = findFileExtFromDataFile("data/maps/", ".json"); // FIXME Hardcoded
        if (!res.empty()) return res;
    }

    if (!boost::filesystem::exists(*data_path)) return mapfiles;

    boost::filesystem::path maps_path = *data_path;
    maps_path /= "maps"; // FIXME Hardcoded

    if (!boost::filesystem::exists(maps_path)) return mapfiles;

    return findFileExtRecursive(mapfiles, maps_path, ".json");
}

bool Data::saveMapJSON(std::string name, std::string json)
{
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
    std::string::iterator si = name.begin();
    while (si != name.end()) {
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
}

boost::filesystem::path Data::solveFilePath(std::string name)
{
    boost::filesystem::path first_path = *data_path;
    boost::filesystem::path second_path = boost::filesystem::path(name.c_str());
    first_path /= name.c_str();
    if (!boost::filesystem::exists(first_path) && boost::filesystem::exists(second_path)) {
        first_path = second_path;
    }
    return first_path;
}

std::vector<std::string> Data::readLinesFromFile(std::string name)
{
    std::vector<std::string> tmp;
    if (datafile.get()) {
        // XXX
        const blieng::DataFile::DataFileObject *obj = datafile->getObject("data/" + name);
        if (obj == NULL) obj = datafile->getObject(name);
        if (obj != NULL) {
            unsigned int pos = 0;
            std::string line;
            while (pos < obj->len) {
                if (obj->data[pos] == '\n') {
                    tmp.push_back(line);
                    line = "";
                } else {
                    line += obj->data[pos];
                }
                pos++;
            }
            if (line.size() > 0) {
                tmp.push_back(line);
            }
            return tmp;
        }
    }

    if (!data_path.get()) return tmp;

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

unsigned int Data::readData(std::string name, char **data)
{
    if (datafile.get()) {
        // FIXME
        const blieng::DataFile::DataFileObject *obj = datafile->getObject("data/" + name);
        if (obj == NULL) obj = datafile->getObject(name);
        if (obj != NULL) {
            if (data != NULL) *data = obj->data;
            return obj->len;
        }
    }

    if (!data_path.get()) return 0;

    BOOST_FOREACH(blieng::DataBuffer *buf, __buffers)
    {
        if (buf->name == name) {
            if (data != NULL) {
                *data = buf->data;
            }
            return buf->len;
        }
    }

    boost::filesystem::path first_path = solveFilePath(name);
    if (boost::filesystem::exists(first_path)) {
        boost::filesystem::ifstream fd(first_path, std::ifstream::binary);

        #define BUFSIZE 1024
        //char *buffer = new char[BUFSIZE];
        //char *buffer = static_cast<char*>(malloc(sizeof(char) * BUFSIZE));
        char *buffer = (char*)malloc(BUFSIZE);
        char *tmp = buffer;
        unsigned int totalsize = 0;
        while (!fd.eof() && fd.good()) {
            if (fd.rdbuf()->in_avail() <= 0) break;
            int cnt = fd.readsome(tmp, BUFSIZE);
            if (cnt > 0) {
                //buffer = static_cast<char*>(realloc(buffer, totalsize + BUFSIZE));
                //unsigned int cursize = totalsize;
                totalsize += cnt;
                buffer = (char*)(realloc(buffer, totalsize + BUFSIZE));

                //unsigned int cursize = totalsize;
                //totalsize += static_cast<unsigned int>(cnt);

                //char *new_buffer = new char[totalsize + BUFSIZE];
                //memcpy(new_buffer, buffer, cursize);
                //delete [] buffer;

                //buffer = new_buffer;

                tmp = buffer + totalsize;
            }
        }
        #undef BUFSIZE
        fd.close();

        if (data != NULL) {
            unique_ptr<blieng::DataBuffer> tmp_ptr(new blieng::DataBuffer(name, buffer, totalsize));
            __buffers.push_back(std::move(tmp_ptr));
            *data = __buffers.back()->data;
        } else {
            //delete [] buffer;
            free(buffer);
        }

        return totalsize;
    }

    return 0;
}

std::string Data::readString(std::string name)
{
    std::string res = "";

    if (datafile.get()) {
        //FIXME
        const blieng::DataFile::DataFileObject *obj = datafile->getObject("data/" + name);
        if (obj == NULL) obj = datafile->getObject(name);
        if (obj != NULL) {
            res.append(obj->data, obj->len);
            return res;
        }
    }

    if (!data_path.get()) return res;

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
        std::cout << "Parse error while parsing '" << name << "':" << reader.getFormatedErrorMessages() << "!\n";
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
