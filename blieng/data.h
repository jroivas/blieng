#ifndef __BLIENG_DATA_H
#define __BLIENG_DATA_H

#include <boost/filesystem.hpp>
#include <vector>
#include <memory>
#include <map>
#include <json/reader.h>
#include <boost/random/random_device.hpp>
#include "datafile.h"
#include "auto_vector.h"

using std::unique_ptr;

namespace blieng
{

class DataBuffer;

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

    bool fileExists(std::string name);

    boost::random::random_device *getGen() { return gen; }

    std::string findFile(std::string name);
    std::string formatString(std::string replace_string, unsigned int num);

    std::vector<std::string> findFileExtRecursive(std::vector<std::string> mapfiles, const boost::filesystem::path &dir_path, std::string ext);
    std::vector<std::string> listMaps();

    bool saveMapJSON(std::string name, std::string json);

    void setGameLocation(std::string location) { game_location = location; }

private:
    Data();

    unsigned int readDataFromDataPath(std::string name, char **data);

    std::string findFileRecursive(const boost::filesystem::path &dir_path, std::string name);
    boost::filesystem::path solveFilePath(std::string name);

    std::string findFileFromDataFile(std::string name);
    std::vector<std::string> findFileExtFromDataFile(std::string path, std::string ext);

    std::unique_ptr<boost::filesystem::path> findDataFile(std::string datafilename = "data.dat");
    std::unique_ptr<boost::filesystem::path> findDataPath();
    std::unique_ptr<boost::filesystem::path> data_path;
    std::unique_ptr<boost::filesystem::path> data_file_path;
    boost::random::random_device *gen;

    std::unique_ptr<blieng::DataFile> datafile;

    static Data *__data_instance;
    auto_vector<DataBuffer> __buffers;

    std::string game_location;
};

}

#endif
