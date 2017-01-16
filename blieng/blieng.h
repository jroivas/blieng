/*
 * Copyright 2014-2017 Jouni Roivas
 */

#ifndef __BLIENG_BLIENG_H
#define __BLIENG_BLIENG_H

#include <boost/smart_ptr/shared_ptr.hpp>

#include "blieng/data.h"
#include "blieng/configure.h"
#include "blieng/bliobject.h"

namespace blieng
{

static const double PI = 3.14159;
static const double RADIUS = 6378137;


class Configure;

/**
 * Class to combine all state information and objects
 * in easy passable format.
 */
class BliengState
{
public:
    /**
     * Contruct empty state info
     */
    BliengState();
    ~BliengState();

    BliengState(const BliengState *another);

    /**
     * Initializes state info with fresh
     * instances of different state objects
     */
    void initialize();

    /**
     * Override data object
     *
     * \param _data New data object
     */
    inline void setData(blieng::Data* _data)
    {
        m_data = _data;
    }
    /**
     * Override config object
     *
     * \param _config New config object
     */
    inline void setConfig(blieng::Configure* _config)
    {
        m_config = _config;
    }
    /**
     * Override style config object
     *
     * \param _config New style config object
     */
    inline void setStyleConfig(blieng::Configure*  _config)
    {
        m_styleconfig = _config;
    }

    /**
     * Record object into global state info
     *
     * \param obj BliObject to add
     */
    void recordObject(
        blieng::BliObject *obj);
    /**
     * Remove object from global state info
     *
     * \param obj BliObject to remove
     * \returns True if removed, false otherwise
     */
    bool unrecordObject(
        blieng::BliObject *obj);

    blieng::Data* m_data;
    blieng::Configure* m_config;
    blieng::Configure* m_styleconfig;

private:
    std::vector<blieng::BliObject *> m_objects;
};

/**
 * Helper class to safely pass pointer.
 * Pointer data is copied and automatically deleted.
 */
class SafeDataPtr
{
public:
    /**
     * Initialize with char data.
     * Reserves memory and copies the data
     *
     * \param _data The data
     * \param _len Length of the data
     */
    SafeDataPtr(
        char _data[],
        unsigned int _len)
        : len(_len)
    {
        dataptr = new char[len];
        memmove(dataptr, _data, len);
    }
    /**
     * Initialize with unsigned char data.
     * Reserves memory and copies the data.
     * Casts the data to signed format.
     *
     * \param _data The data
     * \param _len Length of the data
     */
    SafeDataPtr(
        unsigned char _data[],
        unsigned int _len)
        : len(_len)
    {
        dataptr = new char[len];
        memmove(dataptr, reinterpret_cast<char*>(_data), len);
    }
    /**
     * Automatically delete the data
     */
    ~SafeDataPtr()
    {
        if (dataptr != nullptr)
            delete [] dataptr;
        dataptr = nullptr;
        len = 0;
    }

    /**
     * Get pointer to the data
     *
     * \returns Pointer to the data
     */
    const char * getData() const
    {
        return dataptr;
    }
    /**
     * Get lenght of the data
     *
     * \returns Length of the data
     */
    unsigned int length() const
    {
        return len;
    }
    /**
     * Get length of the data
     *
     * \returns Length of the data
     */
    unsigned int size() const
    {
        return len;
    }

private:
    char *dataptr;
    unsigned int len;
};


/**
 * Helper tool to setup the encryption key.
 *
 * \param key Encryption key
 * \param len Encryprtion key length
 * \param iv Initialization vector
 * \param iv_len Initialization vector lenght
 * \return Safe pointer to modified key data
 */
std::unique_ptr<SafeDataPtr> setupKey(
    const char *key,
    unsigned int key_len,
    const char *iv = nullptr,
    unsigned int iv_len = 0);

/**
 * Obfuscates given data.
 * Data will be intact, will return a new char array
 * with the obfuscated data.
 * Caller need to destroy the data after use.
 *
 * \param dataptr Plain text data
 * \param len Data length
 * \param key Encryprtion key
 * \param key_len Encryprtion key length
 * \param seed A seed info for the obfuscator
 * \returns Tuple of encrypted data, encrypted length and real length
 */
std::tuple<
    char *,
    unsigned int,
    unsigned int>
obfuscate(
    const char *dataptr,
    unsigned int len,
    const char *key,
    unsigned int key_len,
    const std::string &seed);

/**
 * Deobfuscates given data.
 * Data will be intact, will return a new char array
 * with the obfuscated data.
 * Caller need to destroy the data after use.
 *
 * \param dataptr Plain text data
 * \param len Crypted data length
 * \param real_len Data real length
 * \param key Encryprtion key
 * \param key_len Encryprtion key length
 * \param seed A seed info for the deobfuscator
 * \returns Tuple of decrypted data, decrypted length and real length
 */
std::tuple<
    char *,
    unsigned int,
    unsigned int>
deobfuscate(
    const char *dataptr,
    unsigned int len,
    unsigned int real_len,
    const char *key,
    unsigned int key_len,
    const std::string &seed);

/**
 * Compress given data.
 * Algorithm and binary format may change, but it's guaranteed that
 * you can always decompress data compressed with this method.
 * This may return nullptr and zero size, that means data is
 * not compressable, or there's no point to compress it.
 *
 * \param dataptr Data to compress
 * \param len Data length
 * \returns Compressed data and length as tuple
 */
std::tuple<
    char *,
    unsigned int>
compress(
    const char *dataptr,
    unsigned int len);

/**
 * Uncompress given data.
 * It's guaranteed that you can always decompress data
 * compressed with compress method.
 *
 * \param dataptr Data to decompress
 * \param len Compressed data length
 * \returns Decompressed data and length as tuple
 */
std::tuple<
    char *,
    unsigned int>
decompress(
    const char *dataptr,
    unsigned int len);

/**
 * Check if data is compressed.
 *
 * \param dataptr Data to check
 * \param len Data length
 * \returns True if data is compressed, false otherwiser
 */
bool isCompressed(
    const char *dataptr,
    unsigned int len);

/**
 * Get path to permanent storage for specified target.
 * Target means mostly application/game name.
 *
 * \param target Target to get the path for
 * \returns Full path to permanent writable folder
 */
std::string permanentStoragePath(
    const std::string &target);

/**
 * Ensure the permanent storage path exists.
 *
 * \param target Target to get the path for
 * \returns Full path to permanent writable folder
 */
std::string ensurePermanentStoragePath(
    const std::string &target);

}

#endif  // __BLIENG_BLIENG_H
