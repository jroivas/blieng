/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_BLIOBJECT_H
#define __BLIENG_BLIOBJECT_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include "blieng/bliany.h"
#include "blieng/config.h"

#ifdef DATA_MUTEX_LOCK
#include <boost/thread/mutex.hpp>
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
    void setValue(const std::string &key, BliAny value);
    /**
     * Quick way to check if there's value defined for a key.
     *
     * \param key Unique key string
     * \returns True if value for the key is set, false otherwise
     */
    virtual bool isValue(const std::string &key) const;

    /**
     * Increases the value of given key.
     * This assumes that the value at given key is numerical type.
     *
     * \param key Unique key string
     * \returns True if value is incremented, false otherwise
     */
    virtual bool increase(const std::string &key);
    /**
     * Decreases the value of given key.
     * This assumes that the value at given key is numerical type.
     *
     * \param key Unique key string
     * \returns True if value is decreased, false otherwise
     */
    virtual bool decrease(const std::string &key);
    /**
     * Changed numerical value of given key by given difference.
     * This assumes that the value at given key is numerical type.
     *
     * \param key Unique key string
     * \param diff The difference to change, internally effect is: value += diff
     * \returns True if value is changed, false otherwise
     */
    bool changeNumberValue(const std::string &key, int diff);

    /**
     * Get value of given key.
     * Throws an error if key not found.
     *
     * \param key Unique key string
     * \returns BliAny object of the value.
     */
    virtual BliAny getValue(const std::string &key) const;
    /**
     * Get string value of given key.
     * Throws an error if key not found.
     * If value is not presentable as string, returns default value
     *
     * \param key Unique key string
     * \param default_value Value to be returned if value is not representable in string.
     * \returns Value as string object
     */
    virtual std::string getStringValue(
        const std::string &key,
        const std::string &default_value = "") const;
    /**
     * Get boolean value of given key.
     * Throws an error if key not found.
     * If value is not presentable as boolean, returns default value
     *
     * \param key Unique key string
     * \param default_value Value to be returned if value is not representable in boolean.
     * \returns Value as boolean
     */
    virtual bool getBoolValue(
        const std::string &key,
        const bool &default_value = false) const;
    /**
     * Get integer value of given key.
     * Throws an error if key not found.
     * If value is not presentable as integer, returns default value
     *
     * \param key Unique key string
     * \param default_value Value to be returned if value is not representable in integer.
     * \returns Value as integer
     */
    virtual int getIntValue(
        const std::string &key,
        int default_value = 0) const;
    /**
     * Get unsigned integer value of given key.
     * Throws an error if key not found.
     * If value is not presentable as unsigned integer, returns default value
     *
     * \param key Unique key string
     * \param default_value Value to be returned if value is not representable in unsigned integer.
     * \returns Value as unsigned integer
     */
    virtual unsigned int getUIntValue(
        const std::string &key,
        unsigned int default_value = 0) const;
    /**
     * Get double value of given key.
     * Throws an error if key not found.
     * If value is not presentable as double, returns default value
     *
     * \param key Unique key string
     * \param default_value Value to be returned if value is not representable in double.
     * \returns Value as double
     */
    virtual double getDoubleValue(
        const std::string &key,
        double default_value = 0.0) const;
    /**
     * Get vector of strings of given key.
     * Throws an error if key not found or
     * if value is not presentable as vector of strings
     *
     * \param key Unique key string
     * \returns Value as vector of strings
     */
    virtual std::vector<std::string> getListValue(const std::string &key);
    /**
     * Get vector of integers of given key.
     * Throws an error if key not found or
     * if value is not presentable as vector of integers
     *
     * \param key Unique key string
     * \returns Value as vector of integers
     */
    virtual std::vector<int> getIntValues(const std::string &key);

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
    const std::type_info *getValueType(const std::string &key) const;

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
    inline unsigned int size() { return values.size(); }

    /**
     * Assigns other BliObject to this one.
     * Assign is done by iterating all keys of other BliObject
     * and assigning key on this object with the value or the other.
     * So effectively copies key-value pairs from other to this one.
     * If key with same name is already specified in this object it's replaced.
     *
     * \param another Any other BliObject
     */
    void assignObject(const BliObject *another);

    /**
     * Get random integer inside the range.
     * Utilizes uniform distribution in range [min, max)
     *
     * \param limit_low The lower limit of the range
     * \param limit_max The upper limit of the range
     * \returns Integer between the lower and upper limit
     */
    static int getRandomInt(int limit_low, int limit_max);
    /**
     * Get random double inside the range.
     * Utilizes uniform distribution in range [min, max)
     *
     * \param limit_low The lower limit of the range
     * \param limit_max The upper limit of the range
     * \returns Integer between the lower and upper limit
     */
    static double getRandomDouble(double limit_low, double limit_max);
    /**
     * Get a random boolean value.
     * Implementation if this method may change at any point.
     *
     * \returns Randomly true or false
     */
    static bool getRandomBoolean();

protected:
    std::map<std::string, BliAny> values;  //!< Contains key value mappings

#ifdef DATA_MUTEX_LOCK
private:
    boost::mutex value_mutex;
#endif
};

std::string toString(unsigned int value);
std::string toString(int value);
std::string toString(double value);

std::string percentageString(
    double value,
    unsigned int digits=0);

}  // namespace blieng

#endif  // __BLIENG_BLIOBJECT_H
