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
    bool initialize(const std::string &datafilename, const char*, unsigned int);

    std::string readString(const std::string &name) const;
    std::vector<std::string> readLinesFromFile(const std::string &name) const;
    unsigned int readData(const std::string &name, char **data);

    json_value *readJson(const std::string &name);
    std::vector<std::string> getJsonKeys(const json_value *val) const;
    bool isJsonKey(json_value *val, const std::string &key) const;
    const json_value *getJsonValue(const json_value *val, const std::string &key) const;

    bool fileExists(const std::string &name) const;

    std::string findFile(const std::string &name) const;
    std::string formatString(const std::string &replace_string, unsigned int num) const;

    std::vector<std::string> findFileExtRecursive(std::vector<std::string> mapfiles, const boost::filesystem::path &dir_path, const std::string &ext) const;
    std::vector<std::string> listMaps();

    bool saveMapJSON(const std::string &name, const std::string &json);

    inline void setGameLocation(const std::string &location) { game_location = location; }

private:
    unsigned int readDataFromDataPath(const std::string &name, char **data);

    std::string findFileRecursive(const boost::filesystem::path &dir_path, const std::string &name) const;
    boost::filesystem::path solveFilePath(const std::string &name) const;

    std::string findFileFromDataFile(const std::string &name) const;
    std::vector<std::string> findFileExtFromDataFile(const std::string &path, const std::string &ext) const;

    std::unique_ptr<boost::filesystem::path> findDataFile(const std::string &datafilename = "data.dat");
    std::unique_ptr<boost::filesystem::path> findDataPath();
    std::unique_ptr<boost::filesystem::path> data_path;
    std::unique_ptr<boost::filesystem::path> data_file_path;

    std::unique_ptr<blieng::DataFile> datafile;

    auto_vector<DataBuffer> __buffers;

    std::string game_location;
};

}

#endif
