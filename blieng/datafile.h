/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_DATAFILE_H
#define __BLIENG_DATAFILE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#ifdef ANDROID
#include <android/asset_manager.h>
#endif

#include "blieng/auto_map.h"

using std::unique_ptr;

namespace blieng
{

class SafeDataPtr;

/**
 * Archive file reader and writer.
 * Supports arbitrary paths and subfolders, finding and extracting single file.
 * Encryption possibility.
 */
class DataFile
{
public:
    /**
     * For handling the real file data.
     * Utilized for caching the file data,
     * implements also support for encryption/obfuscating.
     */
    class DataFileObject
    {
    public:
        /**
         * Just initializes a empty object.
         */
        DataFileObject() : dataptr(nullptr), len(0), real_len(0) {}
        /**
         * Initialize with the initial data.
         * Allocates memory and copies the data.
         *
         * \param new_data The new data for this object
         * \param new_len Lenght of the new data in bytes
         */
        DataFileObject(const char *new_data, unsigned int new_len);
        /**
         * Ensures we do not left anything behind.
         */
        ~DataFileObject() {
            if (dataptr != nullptr) delete dataptr;
        }

        /**
         * Obfuscates this object.
         * This object will be intact, but will return a new object
         * with the obfuscated data.
         *
         * \param key Encryprtion key
         * \param len Encryprtion key length
         * \param seed A seed info for the obfuscator
         * \returns A new \ref DataFileObject with obfuscated data
         */
        std::unique_ptr<DataFileObject> obfuscate(
            const char *key,
            unsigned int len,
            const std::string &seed="");
        /**
         * Deobfuscates this object.
         * This object will be intact, but will return a new object
         * with the deobfuscated data.
         *
         * \param key Encryprtion key
         * \param len Encryprtion key length
         * \param seed A seed info for the deobfuscator
         * \returns A new \ref DataFileObject with deobfuscated data
         */
        std::unique_ptr<DataFileObject> deobfuscate(
            const char *key,
            unsigned int len,
            const std::string &seed="");

        /**
         * Get lenght of data in this object
         *
         * \return Lenght of data
         */
        inline unsigned int length() const { return len; }
        /**
         * Get pointer to the data.
         * Mainly useful for modifying the contents.
         *
         * \return Pointer to the data inside this object
         */
        char *get() { return dataptr; }
        /**
         * Get constant pointer to the data.
         * Faster and safer way to read the data content.
         *
         * \return Constant pointer to the data inside this object
         */
        char *get() const { return dataptr; }

    protected:
        char *dataptr;  //!< The data itself
        unsigned int len;  //!< Length of the data
        friend class DataFile;

    private:
        /**
         * Helper tool to setup the encryption key.
         *
         * \param key Encryption key
         * \param len Encryprtion key length
         * \param iv Initialization vector
         * \param iv_len Initialization vector lenght
         * \return Safe pointer to modified key data
         */
        std::unique_ptr<blieng::SafeDataPtr> setupKey(
            const char *key,
            unsigned int len,
            const char *iv = nullptr,
            unsigned int iv_len = 0);

        /**
         * Real length of the data.
         * Obfuscating may change the length of data
         * written to file. Need to know the length of the real data as well.
         */
        unsigned int real_len;
    };

    /**
     * Simple constructor to just initialize dummy.
     */
    DataFile();
    /**
     * More advanced constructor to set file name.
     * Calls for \ref setName
     *
     * \param name Data file name
     */
    explicit DataFile(const std::string &name);
    virtual ~DataFile();

    /**
     * Sets data file name.
     * This should contain full path to the file, and file should be accessible.
     * Does not do any checks at this point.
     *
     * \param name Data file name
     */
    void setName(const std::string &name);
    /**
     * Check validity of this object.
     * In fact in current implementation checks only if name is set.
     *
     * \returns True if this object is valid, false otherwise
     */
    bool isValid() const { return _ok; }

    /**
     * Set file contents from string.
     * Assigns data for specified file name inside tha archive.
     *
     * \param name Name and location of file
     * \param data Contents of the file
     * \returns True if successfully added, false otherwise
     */
    bool addData(const std::string &name, const std::string &data);
    /**
     * Set file contents from char array
     * Assigns data for specified file name inside tha archive.
     *
     * \param name Name and location of file
     * \param data Contents of the file
     * \param len Length of the contents
     * \returns True if successfully added, false otherwise
     */
    bool addData(const std::string &name, const char *data, unsigned int len);
    /**
     * Get contents of a file.
     * Receive data contents from a file located inside the archive.
     *
     * \param name Name and location of file
     * \param data Pointer to char array, will be assigned to the contents
     * \returns Number of bytes available in data pointer
     */
    unsigned int getData(const std::string &name, const char **data);
    /**
     * Get a file object.
     * For more info see \ref DataFileObject
     *
     * \param name Name and location of file
     * \returns A new data file object containing file contents or nullptr
     */
    const DataFileObject *getObject(const std::string &name);

    /**
     * List all files.
     * List will contain full paths and names to the files inside the archive.
     *
     * \returns Vector of strings containing file names
     */
    std::vector<std::string> listFiles();

    /**
     * Read and cache contents of the archive.
     * Reads all the contents of the arhchive and caches it to memory as \ref DataFileObject
     *
     * \param key Encryption key
     * \param key_len Encryption key length
     * \return True if successfully read, false otherwise
     */
    bool read(const char *key, unsigned int key_len);
    /**
     * Writes all cached contents to the archive.
     * Writes all of the cached contents to the arhchive
     * By default setData stores contents to memory as \ref DataFileObject
     * This actually does the writing of this data back to the archive.
     *
     * \param key Encryption key
     * \param key_len Encryption key length
     * \return True if successfully written, false otherwise
     */
    bool write(const char *key, unsigned int key_len);
    /**
     * Simple obfuscator for file names.
     * Does very simple obfuscation for data, mainly just to prevent
     * reading it with text editor.
     *
     * \param data Data to be obfuscated
     * \param len Length of the data in bytes to be obfuscated
     * \returns A safe data pointer of the obfuscated data
     */
    std::unique_ptr<SafeDataPtr> obfuscateSimple(
        const char *data,
        unsigned int len);

private:
    std::string _name;
    /**
     * Unify a file name.
     * Get rid of invalid characters.
     *
     * \param name A file name
     * \returns Unified file name
     */
    std::string unifyName(const std::string &name) const;

    auto_map<std::string, DataFileObject> _data;
    bool _ok;
};

}  // namespace blieng

#endif  // __BLIENG_DATAFILE_H
