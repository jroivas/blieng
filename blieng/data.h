#ifndef __BLIENG_DATA_H
#define __BLIENG_DATA_H

#include <boost/filesystem.hpp>
#include <vector>
#include <memory>
#include <map>
#include "json.h"
#include "datafile.h"
#include "auto_vector.h"

#if defined(__cplusplus) && __cplusplus >= 201103L
#include <memory>
using namespace std;
#else

#if defined(__cplusplus) && __cplusplus < 201103L
#include <tr1/memory>
using namespace std;
#else
#include <boost/smart_ptr/shared_ptr.hpp>
using namespace boost;
#endif

#endif

using std::unique_ptr;

namespace blieng
{

class DataBuffer;

class Data
{
public:
    Data();
    virtual ~Data();

    bool initialize(const char *key, unsigned int key_len);
    bool initialize(std::string datafilename, const char*, unsigned int);

    std::string readString(std::string name);
    std::vector<std::string> readLinesFromFile(std::string name);
    unsigned int readData(std::string name, char **data);

    json_value *readJson(const std::string &name);
    std::vector<std::string> getJsonKeys(const json_value *val) const;
    bool isJsonKey(json_value *val, std::string key);
    const json_value *getJsonValue(const json_value *val, std::string key) const;

    bool fileExists(std::string name);

    std::string findFile(std::string name);
    std::string formatString(std::string replace_string, unsigned int num) const;

    std::vector<std::string> findFileExtRecursive(std::vector<std::string> mapfiles, const boost::filesystem::path &dir_path, std::string ext);
    std::vector<std::string> listMaps();

    bool saveMapJSON(std::string name, std::string json);

    inline void setGameLocation(std::string location) { game_location = location; }

private:
    unsigned int readDataFromDataPath(std::string name, char **data);

    std::string findFileRecursive(const boost::filesystem::path &dir_path, std::string name);
    boost::filesystem::path solveFilePath(std::string name);

    std::string findFileFromDataFile(std::string name);
    std::vector<std::string> findFileExtFromDataFile(std::string path, std::string ext);

    std::unique_ptr<boost::filesystem::path> findDataFile(std::string datafilename = "data.dat");
    std::unique_ptr<boost::filesystem::path> findDataPath();
    std::unique_ptr<boost::filesystem::path> data_path;
    std::unique_ptr<boost::filesystem::path> data_file_path;

    std::unique_ptr<blieng::DataFile> datafile;

    auto_vector<DataBuffer> __buffers;

    std::string game_location;
};

}

#endif
