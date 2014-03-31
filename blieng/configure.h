#ifndef __BLIENG_CONFIGURE_H
#define __BLIENG_CONFIGURE_H

#include "config.h"
#include "bliobject.h"
#include "data.h"
#include <iostream>
#include <vector>
#include <map>
#include "json.h"
#include <boost/thread/mutex.hpp>

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

namespace blieng
{

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
     * \param data The data backend instance.
     */
    Configure(shared_ptr<blieng::Data> data);
    virtual ~Configure();

    /**
     * Enumeration of configuration key types.
     */
    typedef enum {
        KeyString, //!< String value
        KeyDouble, //!< Double value
        KeyUInt, //!< Unsigned integer value
        KeyInt, //!< Integer value
        KeyBool, //!< Boolean value
        KeyStringList, //!< String list
        KeyIntList //!< Integer list
    } key_type_t;
    /**
     * Load configurations from a file.
     * Utilizes the data backend for finding
     * and reading the file.
     *
     * \param config_file Name of the configuration file
     * \returns True if file found and read successfully, false otherwise
     */
    bool load(const std::string &config_file);

    /**
     * Adds mandatory key to be parsed from configuration.
     * If given key is not found when loading the file,
     * the \ref validate operation will fail.
     *
     * \param key_name The name of the mandatory key
     * \param key_type Type of the mandatory key, see \ref key_type_t
     */
    void addKey(const std::string &key_name, key_type_t key_type);
    /**
     * Adds optional key to be parsed from configuration.
     * Like mandatory key, but \ref validate does not fail if key is not found.
     *
     * \param key_name The name of the mandatory key
     * \param key_type Type of the mandatory key, see \ref key_type_t
     */
    void addOptionalKey(const std::string &key_name, key_type_t key_type);
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
    bool validateValues();

private:
    /**
     * Does parsing of the loaded data
     */
    void parse();

    std::string config_file;
    json_value *data_json;
    std::map<std::string, key_type_t> keys;
    std::map<std::string, key_type_t> opt_keys;

    boost::mutex key_mutex;
    shared_ptr<blieng::Data> data;
};

}

#endif
