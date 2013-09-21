#ifndef __BLIENG_DATA_H
#define __BLIENG_DATA_H

#include <boost/filesystem.hpp>
#include <vector>
#include <memory>
#include <json/reader.h>
#include <boost/random/random_device.hpp>
#include "datafile.h"

using std::auto_ptr;

namespace blieng
{

class Data
{
public:
    virtual ~Data();
    static Data *getInstance();

    bool initialize(const char *key, unsigned int key_len);
    bool initialize(std::string datafilename, const char*, unsigned int);

    std::string readString(std::string name);
    std::vector<std::string> readLinesFromFile(std::string name);
    unsigned int readData(std::string name, char **data);

    Json::Value readJson(std::string name);
    std::vector<std::string> getJsonKeys(Json::Value val);
    bool isJsonKey(Json::Value val, std::string key);
    Json::Value getJsonValue(Json::Value val, std::string key);

    boost::random::random_device *getGen() { return gen; }

    std::string findFile(std::string name);
    std::string formatString(std::string replace_string, unsigned int num);

    std::vector<std::string> findFileExtRecursive(std::vector<std::string> mapfiles, const boost::filesystem::path &dir_path, std::string ext);
    std::vector<std::string> listMaps();

    bool saveMapJSON(std::string name, std::string json);

private:
    Data();

    std::string findFileRecursive(const boost::filesystem::path &dir_path, std::string name);
    boost::filesystem::path solveFilePath(std::string name);

    std::string findFileFromDataFile(std::string name);
    std::vector<std::string> findFileExtFromDataFile(std::string path, std::string ext);

    std::auto_ptr<boost::filesystem::path> findDataFile(std::string datafilename = "data.dat");
    std::auto_ptr<boost::filesystem::path> findDataPath();
    std::auto_ptr<boost::filesystem::path> data_path;
    auto_ptr<boost::filesystem::path> data_file_path;
    boost::random::random_device *gen;

    std::auto_ptr<blieng::DataFile> datafile;

    static Data *__data_instance;
};

}

#endif
