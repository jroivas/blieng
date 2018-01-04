/*
 * Copyright 2014-2017 Jouni Roivas
 */

#include "blieng/data.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <algorithm>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "blieng/bliany.h"
#include "blieng/logging.h"
#include "blieng/json.h"

#ifdef DATA_MUTEX_LOCK
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#endif

using blieng::Data;
using blieng::BliengJson;

/**
 * Internally utilized data/file buffer to store data in memory.
 */
class blieng::DataBuffer
{
public:
    /**
     * Constructor is utilized to initialize values.
     *
     * \param _name Name of file
     * \param _data Contents of file, should be freeable
     * \param _len Length of file
     */
    DataBuffer(const std::string &_name, char *_data, unsigned int _len) :
        name(_name), data(_data), len(_len) {}
    /**
     * Frees the memory reserved by data
     */
    ~DataBuffer() {
        delete data;
    }

    std::string name;  //!< Name of the file
    char *data;  //!< Contents of the file
    unsigned int len;  //!< Length of the data/file
};

char * blieng::Data::__data_key = nullptr;
unsigned int blieng::Data::__data_key_len = 0;
static std::string __data_folder_name = "data";
static std::string __maps_folder_name = "maps";
static std::string __maps_file_extension = ".json";
std::string Data::game_location = "";

std::map <std::string, blieng::DataFile *> Data::m_datafiles;
boost::mutex Data::m_datafile_mutex;


Data::Data()
    : m_got_data_path(false),
    m_datafile(nullptr)
{
    initialize("", nullptr, 0);
}

Data::~Data()
{
    if (m_datafile != nullptr) {
        //TODO: Unref!
        m_datafile = nullptr;
        //delete m_datafile;
    }
}

void Data::ensureDataPath()
{
    if (m_got_data_path)
        return;

    updateLocations();
    m_data_path = findDataPath();
    m_got_data_path = true;
}

void Data::updateLocations()
{
    m_locations.clear();
    m_locations.push_back("");
    m_locations.push_back("./");
    if (!game_location.empty())
        m_locations.push_back(game_location + "/");
    m_locations.push_back("../");
    m_locations.push_back(".\\");
    m_locations.push_back("..\\");
}

blieng::DataFile *Data::findGlobalDataFile(
    const std::string &datafilename)
{
    boost::lock_guard<boost::mutex> keylock(m_datafile_mutex);

    auto res = m_datafiles.find(datafilename);
    if (res != m_datafiles.end())
        return res->second;

    auto data = new blieng::DataFile(datafilename);
    m_datafiles[datafilename] = data;
    return data;
}

void Data::initializeDataFile(
    const std::string &datafilename)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(m_value_mutex);
#endif
    if (m_datafile == nullptr || datafilename != "") {
        if (datafilename == "") {
            m_data_file_path = findDataFile();
        } else {
            m_data_file_path = findDataFile(datafilename);
        }
        if (!m_data_file_path.empty()) {
            m_datafile = findGlobalDataFile(m_data_file_path.string());
            LOG_INFO("Found data file: " + m_data_file_path.string());
        }
    }
}

void Data::initializeEncryptionKey(
    const char *key,
    const unsigned int key_len)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(m_value_mutex);
#endif
    if (key != nullptr) {
        if (__data_key != nullptr)
            delete __data_key;

        __data_key_len = key_len;
        __data_key = new char[key_len];
        memmove(__data_key, key, key_len);
    }
}

bool Data::initialize(
    const std::string &datafilename,
    const char *key,
    const unsigned int key_len)
{
    ensureDataPath();
    initializeDataFile(datafilename);
    initializeEncryptionKey(key, key_len);

    bool res = false;
    if (m_datafile != nullptr) {
        LOG_DEBUG("Datafile is valid");
        res = m_datafile->read(__data_key, __data_key_len);
    }
#ifndef ANDROID
    if (!res && !m_data_path.empty()) {
        res = true;
    }
#endif
    return res;
}

bool Data::initialize(
    const char *key,
    const unsigned int key_len)
{
    return initialize("", key, key_len);
}

boost::filesystem::path Data::findDataFileAndroid() const
{
    boost::filesystem::path my_data_path;

    my_data_path = "assets/data/data.dat";
    return my_data_path;
}

boost::filesystem::path Data::findDataFileCommon(
    const std::string &datafilename) const
{
    for (std::string item : m_locations) {
        boost::filesystem::path my_data_path;
        my_data_path = (item + datafilename).c_str();
        boost::system::error_code ec;
        if (boost::filesystem::exists(my_data_path, ec) &&
            boost::filesystem::is_regular_file(my_data_path, ec)) {
            return my_data_path;
        }
    }

    boost::filesystem::path empty_data_path;
    return empty_data_path;
}

boost::filesystem::path Data::findDataFile(
    const std::string &datafilename) const
{
#ifdef ANDROID
    return findDataFileAndroid();
#else
    return findDataFileCommon(datafilename);
#endif
}

boost::filesystem::path Data::findDataPathBase() const
{
#ifndef ANDROID
    for (std::string item : m_locations) {
        boost::filesystem::path my_data_path;
        my_data_path = item.c_str();
        boost::system::error_code ec;
        if (boost::filesystem::exists(my_data_path, ec) &&
            boost::filesystem::is_directory(my_data_path, ec)) {
            return my_data_path;
        }
    }
#endif
    boost::filesystem::path empty_data_path;
    return empty_data_path;
}

boost::filesystem::path Data::findDataPath() const
{
#ifndef ANDROID
    for (std::string item : m_locations) {
        boost::filesystem::path my_data_path;
        my_data_path = (item + __data_folder_name).c_str();
        boost::system::error_code ec;
        if (boost::filesystem::exists(my_data_path, ec) &&
            boost::filesystem::is_directory(my_data_path, ec)) {
            return my_data_path;
        }
    }
    // Fallback to current path
    return findDataPathBase();
#endif
    LOG_ERROR("Couldn't find data path!");
    boost::filesystem::path empty_data_path;
    return empty_data_path;
}

bool Data::fileExists(const std::string &name) const
{
#ifdef ANDROID
    return false;
#else
    boost::system::error_code ec;
    return boost::filesystem::exists(name, ec) &&
           boost::filesystem::is_regular_file(name, ec);
#endif
}

std::string Data::findFileRecursive(
    const boost::filesystem::path &dir_path,
    const std::string &name) const
{
#ifndef ANDROID
    boost::system::error_code ec;
    if (!boost::filesystem::exists(dir_path, ec))
        return "";

    boost::filesystem::directory_iterator end_iter;
    boost::filesystem::directory_iterator dir_iter(dir_path);

    for (; dir_iter != end_iter; dir_iter++) {
        if (boost::filesystem::is_directory(dir_iter->status())) {
            std::string res = findFileRecursive(dir_iter->path(), name);
            if (res != "")
                return res;
        } else if (dir_iter->path().filename().string() == name) {
            return dir_iter->path().string();
        }
    }
#endif
    return "";
}

std::string Data::findFileFromDataFile(const std::string &name) const
{
    BOOST_ASSERT(m_datafile != nullptr);
    for (std::string fname : m_datafile->listFiles()) {
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

std::string Data::findFile(const std::string &name) const
{
    if (m_datafile != nullptr) {
        LOG_DEBUG("Searching file from datafile");
        std::string res = findFileFromDataFile(name);
        if (res != "")
            return res;
    }

#ifndef ANDROID
    if (m_data_path.empty())
        return "";
#endif
    LOG_DEBUG("Searching file recursive from data path");
    return findFileRecursive(m_data_path, name);
}

std::vector<std::string> Data::findFileExtFromDataFile(
    const std::string &path,
    const std::string &ext) const
{
    BOOST_ASSERT(m_datafile != nullptr);
    std::vector<std::string> res;
    BOOST_FOREACH(std::string fname, m_datafile->listFiles()) {
        if (fname.substr(0, path.length()) == path) {
            boost::filesystem::path handle(fname.c_str());
            if (handle.extension() == ext) {
                res.push_back(handle.filename().string());
            }
        }
    }
    return res;
}

std::vector<std::string> Data::findFileExtRecursive(
    std::vector<std::string> mapfiles,
    const boost::filesystem::path &dir_path,
    const std::string &ext) const
{
#ifndef ANDROID
    boost::system::error_code ec;
    if (!boost::filesystem::exists(dir_path, ec))
        return mapfiles;

    boost::filesystem::directory_iterator end_iter;
    boost::filesystem::directory_iterator dir_iter(dir_path);

    for (; dir_iter != end_iter; dir_iter++) {
        if (boost::filesystem::is_directory(dir_iter->status())) {
            mapfiles = findFileExtRecursive(mapfiles, dir_iter->path(), ext);
        } else if (dir_iter->path().extension() == ext) {
            mapfiles.push_back(dir_iter->path().filename().string());
        }
    }
#endif
    return mapfiles;
}

std::vector<std::string> Data::listMaps() const
{
    std::vector<std::string> mapfiles;

    if (m_datafile != nullptr) {
        std::vector<std::string> res = findFileExtFromDataFile(
            __data_folder_name + "/" + __maps_folder_name + "/",
            __maps_file_extension);
        if (!res.empty())
            return res;
    }

#ifndef ANDROID
    boost::system::error_code ec;
    if (!boost::filesystem::exists(m_data_path, ec))
        return mapfiles;

    boost::filesystem::path maps_path = m_data_path;
    maps_path /= __maps_folder_name;

    if (!boost::filesystem::exists(maps_path, ec))
        return mapfiles;

    return findFileExtRecursive(mapfiles, maps_path, __maps_file_extension);
#else
    return mapfiles;
#endif
}

bool Data::saveMapJSON(const std::string &name, const std::string &json)
{
#ifdef ANDROID
    return false;
#else
    // TODO Split
    // TODO Do we need data file support here? Probably not..
    if (m_data_path.empty()) {
        std::cerr
            << "Data path not found, please create it in order to save maps"
            << "\n";
        return false;
    }
    boost::system::error_code ec;
    if (!boost::filesystem::exists(m_data_path, ec))
        return false;

    boost::filesystem::path maps_path = m_data_path;
    maps_path /= "maps";  // FIXME Hardcoded

    if (!boost::filesystem::exists(maps_path, ec)) {
        if (!boost::filesystem::create_directory(maps_path))
            return false;
    }

    std::string clean_name = "";
    auto si = name.cbegin();
    while (si != name.cend()) {
        if (std::isalnum(*si) || *si == '_' || *si == '-') {
            clean_name += *si;
        }
        // if (*si == '.') break;  // FIXME What to do with dots?
        ++si;
    }

    if (clean_name == "")
        return false;

    maps_path /= clean_name;

    if (!boost::filesystem::exists(maps_path, ec)) {
        if (!boost::filesystem::create_directory(maps_path))
            return false;
    }

    maps_path /= clean_name + ".json";
    boost::filesystem::ofstream fd(
        maps_path,
        std::ios_base::out | std::ios_base::trunc | std::ofstream::binary);
    if (!fd.is_open())
        return false;

    fd << json;
    fd.close();

    return true;
#endif
}

boost::filesystem::path Data::solveFilePath(const std::string &name) const
{
#ifdef ANDROID
    boost::filesystem::path p;
    return p;
#else
    boost::filesystem::path first_path = m_data_path;
    boost::filesystem::path second_path = boost::filesystem::path(name.c_str());
    first_path /= name.c_str();
    boost::system::error_code ec;
    if (!boost::filesystem::exists(first_path, ec) &&
        boost::filesystem::exists(second_path, ec)) {
        first_path = second_path;
    }
    return first_path;
#endif
}

std::vector<std::string> Data::readLinesFromFileInDataFolder(
    const std::string &name) const
{
    std::vector<std::string> tmp;
#ifndef ANDROID
    if (!m_data_path.empty()) {
        boost::filesystem::path first_path = solveFilePath(name);
        boost::system::error_code ec;
        if (boost::filesystem::exists(first_path, ec)) {
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
    }
#endif

    return tmp;
}

std::vector<std::string> Data::readLinesFromFileInDatafile(
    const std::string &name) const
{
    std::vector<std::string> tmp;

    if (m_datafile != nullptr) {
        const blieng::DataFile::DataFileObject *obj =
            m_datafile->getObject(__data_folder_name + "/" + name);
        if (obj == nullptr)
            obj = m_datafile->getObject(name);
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
        }
    }

    return tmp;
}

std::vector<std::string> Data::readLinesFromFile(
    const std::string &name) const
{
    std::vector<std::string> tmp;

    tmp = readLinesFromFileInDataFolder(name);
    if (tmp.size() != 0)
        return tmp;

    tmp = readLinesFromFileInDatafile(name);

    return tmp;
}

unsigned int Data::readDataFromDataPath(
    const std::string &name,
    const char **data)
{
    // TODO Split
#ifndef ANDROID
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(m_value_mutex);
#endif

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
    boost::system::error_code ec;
    if (boost::filesystem::exists(first_path, ec)) {
        boost::filesystem::ifstream fd(first_path, std::ifstream::binary);

        #define BUFSIZE 1024
        char *buffer = new char[BUFSIZE];
        char *tmp = buffer;
        unsigned int totalsize = 0;
        while (!fd.eof() && fd.good()) {
            if (fd.rdbuf()->in_avail() <= 0)
                break;
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
            std::unique_ptr<blieng::DataBuffer> tmp_ptr(
                    new blieng::DataBuffer(name, buffer, totalsize));
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

unsigned int Data::readData(
    const std::string &name,
    const char **data)
{
#ifndef ANDROID
    if (!m_data_path.empty()) {
        unsigned int res = readDataFromDataPath(name, data);
        if (res > 0)
            return res;
    }
#endif

    if (m_datafile != nullptr) {
        const blieng::DataFile::DataFileObject *obj =
            m_datafile->getObject(__data_folder_name + "/" + name);
        if (obj == nullptr) obj = m_datafile->getObject(name);
        if (obj != nullptr) {
            if (data != nullptr)
                *data = obj->get();
            return obj->length();
        }
    }

    return 0;
}

std::string Data::readString(
    const std::string &name) const
{
    std::string res = "";

#ifndef ANDROID
    if (!m_data_path.empty()) {
        boost::filesystem::path first_path = solveFilePath(name);
        boost::system::error_code ec;
        if (boost::filesystem::exists(first_path, ec)) {
            boost::filesystem::ifstream fd(first_path, std::ifstream::binary);
            while (fd.good() && !fd.eof()) {
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

    if (m_datafile != nullptr) {
        const blieng::DataFile::DataFileObject *obj =
            m_datafile->getObject(__data_folder_name + "/" + name);
        if (obj == nullptr) obj = m_datafile->getObject(name);
        if (obj != nullptr) {
            res.append(obj->get(), obj->length());
            return res;
        }
    }


    throw std::string("Can't open file: " + name);
}

BliengJson Data::parseJson(
    const std::string &datas)
{
    try {
        return BliengJson::parse(datas);
    }
    catch (nlohmann::detail::parse_error e) {
        throw std::string("JSON parsing error");
    }
}


void Data::freeJson(BliengJson &)
{
}

BliengJson Data::readJson(const std::string &name) const
{
    std::string datas = readString(name);

    return parseJson(datas);
}

std::vector<std::string> Data::getJsonKeys(const BliengJson &val)
{
    return blieng::JsonKeys(val);
}

bool Data::isJsonKey(
    const BliengJson &val,
    const std::string &key)
{
    if (!val.is_object()) return false;
    return val[key] != nullptr;
}

const BliengJson Data::getJsonValue(
    const BliengJson &val,
    const std::string &key)
{
    if (!val.is_object()) return val;
    return val[key];
}

std::string Data::formatString(
    const std::string &replace_string,
    unsigned int num) const
{
    std::ostringstream stream;
    stream << num;
    std::string numstr = stream.str();

    std::string res = "";
    bool num_replace = false;
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < replace_string.length(); i++) {
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
