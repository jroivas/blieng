/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_DATA_H
#define __BLIENG_DATA_H

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "blieng/json.h"
#include "blieng/datafile.h"
#include "blieng/auto_vector.h"
#include "blieng/config.h"

#ifdef DATA_MUTEX_LOCK
#include <boost/thread/mutex.hpp>
#endif

namespace blieng
{

class DataBuffer;

/**
 * The data backend, handles writing data to a storage.
 */
class Data
{
public:
    /**
     * Constructor initializes default data backend.
     */
    Data();
    virtual ~Data();

    /**
     * Initialize encrypted backend.
     * If data backend is encrypted, initialize it with this method.
     * I key is nullptr or key_len is zero, no encryption is used.
     *
     * \param key The encryption key as a char array
     * \param key_len Length of the key
     * \returns True on success, false otherwise
     */
    bool initialize(const char *key, unsigned int key_len);
    /**
     * Initialize encrypted backend on a datafile.
     * If data backend is encrypted, initialize it with this method.
     * I key is nullptr or key_len is zero, no encryption is used.
     * Implementation details may vary depending the platform,
     * and available backed options.
     *
     * \param datafilename Name of the data file
     * \param key The encryption key as a char array
     * \param key_len Length of the key
     * \returns True on success, false otherwise
     */
    bool initialize(
        const std::string &datafilename,
        const char *key,
        unsigned int key_len);

    /**
     * Read a file from the backend as a string
     *
     * \param name Name of the file to be read
     * \returns Contents of the file as a string, or empty string
     */
    std::string readString(const std::string &name) const;
    /**
     * Read file from backend as a vector of strings.
     * Strings are split from the new line character.
     * New line characters are stripped away.
     * Returned vector contains one line per string.
     *
     * \param name Name of the file to be read
     * \returns Contents of the file as vector of strings
     */
    std::vector<std::string> readLinesFromFile(const std::string &name) const;
    /**
     * Read file from backend as a raw data.
     * This method is used for example for binary files.
     *
     * \param name Name of the file to be read
     * \param data Pointer to a char array, pointer will point to the read data
     * \returns Number of bytes read, or 0 on error or empty file
     */
    unsigned int readData(const std::string &name, const char **data);

    /**
     * Read JSON data from a file in backend.
     *
     * \param name Name of the file to be read
     * \returns Parsed internal json_value, see \ref json.h
     */
    json_value *readJson(const std::string &name) const;
    /**
     * Get key names under JSON value
     * Gets all the key names, this assumes that given JSON value is array.
     *
     * \param val Reference to \ref json_value, for example one read with \ref readJson
     * \returns Vector of strings of found key names.
     */
    std::vector<std::string> getJsonKeys(const json_value *val) const;
    /**
     * Check if specific key name is found from JSON value.
     * Assumes that the JSON value in question is array.
     *
     * \param val Reference to \ref json_value, for example one read with \ref readJson
     * \param key Name of the key to check
     * \returns True if key exists, false otherwise
     */
    bool isJsonKey(json_value *val, const std::string &key) const;
    /**
     * Get another JSON value from current value.
     *
     * \param val Reference to \ref json_value, for example one read with \ref readJson
     * \param key Name of the key which value to get
     * \returns JSON value at given key, on error returns given value
     */
    const json_value *getJsonValue(
        const json_value *val,
        const std::string &key) const;

    /**
     * Checks if file exists in backend.
     *
     * \param name Name of the file to be check
     * \returns True if file is found, false otherwise
     */
    bool fileExists(const std::string &name) const;

    /**
     * Try to find a file from backend.
     * Will do recursive search and return first occurence of matching file.
     * That's why it's a good practice to utilize unique file names.
     *
     * \param name Name of the file to be check
     * \returns Full path to the found file or empty string.
     */
    std::string findFile(const std::string &name) const;
    /**
     * Format string and replace marks with given number.
     * Will find for # characters and replace them with the given number.
     * For example "test###" with number 42 will be "test042".
     *
     * \param replace_string A string template to do the replacing in
     * \param num Number to be put in the string
     * \returns New formatted string
     */
    std::string formatString(
        const std::string &replace_string,
        unsigned int num) const;

    /**
     * Find files recursively by extension.
     * Goes thorought the data backend, match file with extension and append matching file
     * names to the vector of strings.
     *
     * \param current_files Vector of existsing files, this will be utilized as base for result
     * \param dir_path Subfolder to start the search from, empty string can be utilized to search from whole backend
     * \param ext Extension to find for, needs the dot. For example ".json"
     * \returns Vector of file names as string found from backend
     */
    std::vector<std::string> findFileExtRecursive(
        std::vector<std::string> current_files,
        const boost::filesystem::path &dir_path,
        const std::string &ext) const;
    /**
     * Specific file finding method to find map files.
     * Assuming maps are located at folder data/maps/ with extension ".json"
     *
     * \returns Vector of map file name as string
     */
    std::vector<std::string> listMaps() const;

    /**
     * Writes down map JSON data to backend.
     *
     * \param name Name of the file to be written to data backend
     * \param json The JSON structure as string, to be written
     * \returns True if successfull
     */
    bool saveMapJSON(const std::string &name, const std::string &json);

    /**
     * Set up game data location.
     * Can be utilized to specify custom search locations in file system.
     *
     * \param location Path to appended in search path
     */
    inline void setGameLocation(const std::string &location)
    {
        //FIXME
        game_location = location;
        updateLocations();
    }

private:
    /**
     * Tries to find and initialize data file.
     * Will not do anything if data file not found.
     *
     * \param datafilename Name of the datafile
     */
    void initializeDataFile(const std::string &datafilename);
    /**
     * Initializes global data file encryption keys.
     * To utilize default keys or old key, pass nullptr for key.
     * If key is given, it will override old global key.
     *
     * \param key New data file encryption key, or nullptr
     * \param key_len Length of the key in bytes
     */
    void initializeEncryptionKey(const char *key, unsigned int key_len);
    unsigned int readDataFromDataPath(
        const std::string &name,
        const char **data);

    void updateLocations();
    std::string findFileRecursive(
        const boost::filesystem::path &dir_path,
        const std::string &name) const;
    boost::filesystem::path solveFilePath(const std::string &name) const;

    std::string findFileFromDataFile(const std::string &name) const;
    std::vector<std::string> findFileExtFromDataFile(
        const std::string &path,
        const std::string &ext) const;

    boost::filesystem::path findDataFileAndroid() const;
    boost::filesystem::path findDataFileCommon(
        const std::string &datafilename) const;
    boost::filesystem::path findDataFile(
        const std::string &datafilename = "data.dat") const;
    boost::filesystem::path findDataPath() const;

    boost::filesystem::path data_path;
    boost::filesystem::path data_file_path;

    std::vector<std::string> readLinesFromFileInDataFolder(
        const std::string &name) const;
    std::vector<std::string> readLinesFromFileInDatafile(
        const std::string &name) const;

    auto_vector<DataBuffer> __buffers;

    static std::string game_location;

    static char * __data_key;
    static unsigned int __data_key_len;

    std::list<std::string> m_locations;

protected:
    blieng::DataFile *m_datafile;

    blieng::DataFile *findGlobalDataFile(const std::string &datafilename);
    static std::map <std::string, blieng::DataFile *> m_datafiles;
    static boost::mutex m_datafile_mutex;

#ifdef DATA_MUTEX_LOCK
private:
    boost::mutex m_value_mutex;
#endif
};

}  // namespace blieng

#endif  // __BLIENG_DATA_H
