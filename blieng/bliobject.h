/*
 * Copyright 2014-2017 Jouni Roivas
 */

#ifndef __BLIENG_BLIOBJECT_H
#define __BLIENG_BLIOBJECT_H

#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include "blieng/bliany.h"
#include "blieng/config.h"

#ifdef DATA_MUTEX_LOCK
#include <boost/thread/mutex.hpp>
#endif
#ifdef PSEUDO_RANDOM
#include <boost/random/mersenne_twister.hpp>
#else
#include <boost/random/random_device.hpp>
#endif

namespace blieng
{

/**
 * Base class for all other Blieng objects.
 * Contains general and useful methods utilized by other classes.
 */
class BliObject
{
public:
    /**
     * This constructor should be called by all the inherited classes.
     * Currently does nothing, but might change any day...
     */
    BliObject();
    virtual ~BliObject();

    /**
     * Setup key-value pair.
     * Key is always unique, value can be any supported type.
     * Calling this method again with the same key
     * will overwrite the existing value.
     *
     * \param key Unique key string
     * \param value Arbitrary value, which is mapped to the specified key
     */
    void setValue(
        const std::string &key,
        BliAny value);
    /**
     * This method is called whenever any value is changed
     * with setValue method. Easy to trigger other updates
     * on object by implementing this method.
     * By default does nothing.
     */
    virtual void valuesUpdated(const std::string &);

    /**
     * Quick way to check if there's value defined for a key.
     *
     * \param key Unique key string
     * \returns True if value for the key is set, false otherwise
     */
    virtual bool isValue(
        const std::string &key) const;

    /**
     * Increases the value of given key.
     * This assumes that the value at given key is numerical type.
     *
     * \param key Unique key string
     * \throw If key value is not numerical
     * \returns True if value is incremented, false otherwise
     */
    virtual bool increase(
        const std::string &key);
    /**
     * Decreases the value of given key.
     * This assumes that the value at given key is numerical type.
     *
     * \param key Unique key string
     * \throw If key value is not numerical
     * \returns True if value is decreased, false otherwise
     */
    virtual bool decrease(
        const std::string &key);
    /**
     * Changed numerical value of given key by given difference.
     * This assumes that the value at given key is numerical type.
     *
     * \param key Unique key string
     * \param diff The difference to change, internally effect is: value += diff
     * \throw If key value is not numerical
     * \returns True if value is changed, false otherwise
     */
    bool changeNumberValue(
        const std::string &key, int diff);

    /**
     * Get value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \throw When key does not exists
     * \returns BliAny object of the value.
     */
    BliAny getValue(
        const std::string &key) const;
    /**
     * Get string value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns Value as string object
     */
    std::string getStringValue(
        const std::string &key) const;
    /**
     * Get boolean value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns Value as boolean
     */
    bool getBoolValue(
        const std::string &key) const;
    /**
     * Get integer value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns Value as integer
     */
    int getIntValue(
        const std::string &key) const;
    /**
     * Get long long value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns Value as long long
     */
    long long getLongLongValue(
        const std::string &key) const;
    /**
     * Get char value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns Value as integer
     */
    char getCharValue(
        const std::string &key) const;
    /**
     * Get unsigned integer value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns Value as unsigned integer
     */
    unsigned int getUIntValue(
        const std::string &key) const;
    /**
     * Get double value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns Value as double
     */
    double getDoubleValue(
        const std::string &key) const;
    /**
     * Get vector of strings of given key.
     * Throws an error if key not found or
     * if value is not presentable as vector of strings
     *
     * \param key Unique key string
     * \returns Value as vector of strings
     */
    virtual std::vector<std::string> getListValue(
        const std::string &key) const;
    /**
     * Get vector of integers of given key.
     * Throws an error if key not found or
     * if value is not presentable as vector of integers
     *
     * \param key Unique key string
     * \returns Value as vector of integers
     */
    virtual std::vector<int> getIntValues(
        const std::string &key) const;

    /**
     * Get string presentation of object.
     * Get all key and value as a string, some values might not be able
     * to be converted to string. Those are marked as "unknown type".
     *
     * \returns String presentation of object
     */
    virtual std::string toString() const;
    /**
     * Get type info of the value at given unique key.
     *
     * \param key Unique key string
     * \returns Standard C++ type info of the value
     */
    const std::type_info *getValueType(
        const std::string &key) const;

    /**
     * List all known keys.
     *
     * \returns std::vector of all known key names.
     */
    virtual std::vector<std::string> getKeys();
    /**
     * Get number of unique keys defined.
     *
     * \returns Number of unique keys.
     */
    inline unsigned int size() const
    {
        return m_values.size();
    }

    /**
     * Assigns other BliObject to this one.
     * Assign is done by iterating all keys of other BliObject
     * and assigning key on this object with the value or the other.
     * So effectively copies key-value pairs from other to this one.
     * If key with same name is already specified in this object it's replaced.
     *
     * \param another Any other BliObject
     */
    void assignObject(
        const BliObject *another);

    /**
     * Assign other BliObject to this one.
     */
    virtual BliObject &operator=(const BliObject *other);

    /**
     * Get random integer inside the range.
     * Utilizes uniform distribution in range [min, max)
     *
     * \param limit_low The lower limit of the range
     * \param limit_max The upper limit of the range
     * \returns Integer between the lower and upper limit
     */
    static int getRandomInt(
        int limit_low,
        int limit_max);
    /**
     * Get random double inside the range.
     * Utilizes uniform distribution in range [min, max)
     *
     * \param limit_low The lower limit of the range
     * \param limit_max The upper limit of the range
     * \returns Integer between the lower and upper limit
     */
    static double getRandomDouble(
        double limit_low,
        double limit_max);
    /**
     * Get a random boolean value.
     * Implementation if this method may change at any point.
     *
     * \returns Randomly true or false
     */
    static bool getRandomBoolean();

    /**
     * Get value of key, utilize template for output type
     * Throws error if conversion is not possible
     *
     * \param key Unique key string
     * \returns Value as given type
     */
    template<typename T>
    T getValue(const std::string &key) const;

    /**
     * Serialize all key-value pairs on this object.
     * Will return serialized binary data
     *
     * \param type Object type for detection, by default BliObject
     * \returns Serialized object as string
     */
    virtual std::string serialize(
        std::string type="BliObject") const;

    /**
     * Deserialize a serialized object
     * Will read data from stream and update current object
     *
     * \param data Serialized data
     * \param type Object type for detection, by default BliObject
     * \returns True on success, false otherwise
     */
    virtual bool deserialize(
        const std::string &data,
        std::string type="BliObject");

    /**
     * Query type of object in serialized string.
     * Will read type from string, which is serialized
     * with BliObject::serialize method.
     *
     * \param data Serialized data
     * \returns Object type for detection, by default BliObject
     */
    static std::string serializedType(
        const std::string &data);

    /**
     * Get the unique ID of this object.
     * All objects gets automatically unique ID upon creation,
     * that way object can be referred and handled easily.
     *
     * One should not do any assumptions about this ID,
     * and the format of the sting may be changed at any time.
     * The only thing that's ensured is that object can be identified
     * with this string.
     *
     * \returns Unique ID of this object as string
     */
    virtual inline std::string uuid() const
    {
        return m_uuid;
    }

protected:
    std::unordered_map<std::string, BliAny> m_values;  //!< Contains key value mappings

    template<typename A, typename B>
    bool fitsLimits(
        BliAny val,
        A &res) const;
    template<typename T>
    bool changeNumValue(
        std::string key,
        BliAny val,
        int diff);

    /**
     * Generates the Unique ID.
     */
    void genUUID();

    std::string m_uuid;  //!< Unique ID

#ifdef DATA_MUTEX_LOCK
private:
    boost::mutex m_value_mutex;
#endif

#ifdef PSEUDO_RANDOM
    static boost::random::mt19937 m_rand_gen;
#else
    static boost::random::random_device m_rand_gen;
#endif
};

std::string toString(unsigned int value);
std::string toString(int value);
std::string toString(double value);

std::string percentageString(
    double value,
    unsigned int digits=0);

}

#endif  // __BLIENG_BLIOBJECT_H
