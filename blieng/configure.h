/*
 * Copyright 2014-2017 Jouni Roivas
 */

#ifndef __BLIENG_CONFIGURE_H
#define __BLIENG_CONFIGURE_H

#include <boost/thread/mutex.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "blieng/config.h"
#include "blieng/blieng.h"
#include "blieng/bliobject.h"
#include "blieng/data.h"
#include "blieng/json.h"

namespace blieng
{

class BliengState;

/**
 * Multipurpose configuration with a data backend.
 * Data backend can be utilized for read and write from/to a file.
 * Internally JSON is chosen for the data format,
 * however this class just assumes that data backend provides
 * methods for reading and parsing the JSON alike format.
 * Underneath the real data format can be anything as long as
 * it can be presented in similar way as JSON.
 * What configure actually assumes is top level JSON array with key-value
 * pairs as JSON, where all JSON values are supported.
 */
class Configure : public blieng::BliObject
{
public:
    /**
     * Constructor with mandatory data backend.
     *
     * \param state State object containing engine object info
     */
    explicit Configure(
        boost::shared_ptr<blieng::BliengState> state);
    virtual ~Configure();

    /**
     * Enumeration of configuration key types.
     */
    typedef enum {
        KeyString,  //!< String value
        KeyDouble,  //!< Double value
        KeyUInt,  //!< Unsigned integer value
        KeyInt,  //!< Integer value
        KeyBool,  //!< Boolean value
        KeyStringList,  //!< String list
        KeyIntList  //!< Integer list
    } key_type_t;
    /**
     * Load configurations from a file.
     * Utilizes the data backend for finding
     * and reading the file.
     *
     * \param config_file Name of the configuration file
     * \returns True if file found and read successfully, false otherwise
     */
    bool load(
        const std::string &config_file);

    /**
     * Adds mandatory key to be parsed from configuration.
     * If given key is not found when loading the file,
     * the \ref validate operation will fail.
     *
     * \param key_name The name of the mandatory key
     * \param key_type Type of the mandatory key, see \ref key_type_t
     */
    void addKey(
        const std::string &key_name,
        key_type_t key_type);
    /**
     * Adds optional key to be parsed from configuration.
     * Like mandatory key, but \ref validate does not fail if key is not found.
     *
     * \param key_name The name of the mandatory key
     * \param key_type Type of the mandatory key, see \ref key_type_t
     */
    void addOptionalKey(
        const std::string &key_name,
        key_type_t key_type);
    /**
     * Validates that all required keys are loaded.
     *
     * Checks if all required keys are set in loaded configuration file
     * \returns True if required keys are loaded, false otherwise
     */
    bool validate() const;
    /**
     * Validates that loaded content is sane.
     *
     * Checks if all keys are set and their values match to the given one.
     * \returns True if required keys are loaded and values matches, false otherwise
     */
    bool validateValues() const;

private:
    /**
     * Does parsing of the loaded data
     */
    void parse();

    std::string config_file;
    BliengJson data_json;
    std::map<std::string, key_type_t> keys;
    std::map<std::string, key_type_t> opt_keys;

    boost::mutex key_mutex;
    boost::shared_ptr<blieng::BliengState> m_state;

    /**
     * Parsing string list values
     *
     * \param key Name of the key for the list
     * \param val JSON value containing the list
     */
    void parseStringList(
        std::string key,
        const BliengJson val);
    /**
     * Parsing integer list values
     *
     * \param key Name of the key for the list
     * \param val JSON value containing the list
     */
    void parseIntList(
        std::string key,
        const BliengJson val);
    /**
     * Parsing boolean value
     *
     * \param key Name of the boolean key
     * \param val JSON value containing the boolean
     */
    void parseBool(
        std::string key,
        const BliengJson val);
};

}  // namespace blieng

#endif  // __BLIENG_CONFIGURE_H
