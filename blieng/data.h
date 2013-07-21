#ifndef __BLIENG_DATA_H
#define __BLIENG_DATA_H

#include <boost/filesystem.hpp>
#include <vector>
#include <json/reader.h>
#include <boost/random/random_device.hpp>
#include "datafile.h"

namespace blieng
{

class Data
{
public:
    static Data *getInstance();

    bool initialize(const char*, unsigned int);

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

    boost::filesystem::path *findDataFile();
    boost::filesystem::path *findDataPath();
    boost::filesystem::path *data_path;
    boost::filesystem::path *data_file_path;
    static Data *__data_instance;
    boost::random::random_device *gen;

    blieng::DataFile *datafile;
};

}

#endif
