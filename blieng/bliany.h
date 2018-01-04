/*
 * Copyright 2014-2017 Jouni Roivas
 */

#ifndef __BLIENG_BLIANY_H
#define __BLIENG_BLIANY_H

#include <ostream>
#ifdef USE_BOOST_ANY
#include <boost/any.hpp>
#else
#include "3rdparty/cdiggins.h"
#endif

/**
 * Print a string to debug stream.
 *
 * This is defined in bliobject.cpp
 * \param str String to print in debug stream
 */
void doDebug(const std::string &str);

namespace blieng
{

/**
 * Supports any type with built-in casting operations.
 *
 * Inheriting boost::any class to allow easy conversion to stream with <<
 * Also provides easy access to converted values
 */
class BliAny : public
#ifdef USE_BOOST_ANY
    boost::any
#else
    cdiggins::any
#endif
{
public:
    /**
     * Dymmy constructor only calls boost constructor
     */
    BliAny()
#ifdef USE_BOOST_ANY
        : boost::any()
#else
        : cdiggins::any()
#endif
    {
    }
    /**
     * Constructor with template
     *
     * \param value Value to be assigned, type defined by template
     */
    template<typename ValueType> BliAny(
        const ValueType & value)
#ifdef USE_BOOST_ANY
        : boost::any(value)
#else
        : cdiggins::any(value)
#endif
    {
    }
    /**
     * Constructor, assign value from other object
     *
     * \param other Some boost::any instance
     */
    BliAny(const any & other)
#ifdef USE_BOOST_ANY
        : boost::any(other)
#else
        : cdiggins::any(other)
#endif
    {
    }

    ~BliAny() {}

#ifdef USE_BOOST_ANY
    #define CAST_TO(X, Y, Z)\
    if (Y.type() == typeid(X))\
    {\
        Z << boost::any_cast<X>(Y);\
    } else

    #define CAST_TO_BOOL(X, Y, Z)\
    if (Y.type() == typeid(X))\
    {\
        Z << (boost::any_cast<X>(Y) ? "true" : "false");\
    } else
#else
    #define CAST_TO(X, Y, Z)\
    if (Y.type() == typeid(X))\
    {\
        BliAny tmp = Y;\
        Z << tmp.cast<X>();\
    } else

    #define CAST_TO_BOOL(X, Y, Z)\
    if (Y.type() == typeid(X))\
    {\
        BliAny tmp = Y;\
        Z << (tmp.cast<bool>() ? "true" : "false");\
    } else
#endif

    /**
     * Print value to ostream.
     *
     * This is utilized for string conversions and printing the value.
     * Casts to proper known value and append to stream or print "Uknown type"
     * if type not castable.
     *
     * \param ostr Stream to write to
     * \param obj BliAny object to write to the stream
     */
    friend std::ostream& operator<< (std::ostream& ostr, BliAny const& obj)
    {
        CAST_TO(int, obj, ostr)
        CAST_TO(unsigned int, obj, ostr)
        CAST_TO(long, obj, ostr)
        CAST_TO(unsigned long, obj, ostr)
        CAST_TO(int64_t, obj, ostr)
        CAST_TO(int64_t, obj, ostr)
        CAST_TO(float, obj, ostr)
        CAST_TO(double, obj, ostr)
        CAST_TO(char, obj, ostr)
        CAST_TO(const char *, obj, ostr)
        CAST_TO(char, obj, ostr)
        CAST_TO_BOOL(bool, obj, ostr)
        CAST_TO(std::string, obj, ostr)
        {
            ostr << "<unknown type>";
        }

        return ostr;
    }
    #undef CAST_TO
    #undef CAST_TO_BOOL

    template<typename T> T asValue()
    {
        if (type() == typeid(T)) {
#ifdef USE_BOOST_ANY
            return boost::any_cast<T>(*this);
#else
            return this->cast<T>();
#endif
        }
        throw std::string("Error, invalid value, can't convert to given type");
    }

    /**
     * Get result as integer.
     * Throws error if type does not match.
     *
     * \returns Value as integer
     */
    inline int asInt()
    {
        return asValue<int>();
    }

    /**
     * Get result as unsigned integer.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned integer
     */
    inline unsigned int asUInt()
    {
        return asValue<unsigned int>();
    }

    /**
     * Get result as long.
     * Throws error if type does not match.
     *
     * \returns Value as long
     */
    inline long asLong()
    {
        return asValue<long>();
    }

    /**
     * Get result as unsigned long.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned long
     */
    inline unsigned long asULong()
    {
        return asValue<unsigned long>();
    }

    /**
     * Get result as char.
     * Throws error if type does not match.
     *
     * \returns Value as char
     */
    inline char asChar()
    {
        return asValue<char>();
    }

    /**
     * Get result as unsigned char.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned char
     */
    inline unsigned char asUChar()
    {
        return asValue<unsigned char>();
    }

    /**
     * Get result as short.
     * Throws error if type does not match.
     *
     * \returns Value as short
     */
    inline short asShort()
    {
        return asValue<short>();
    }

    /**
     * Get result as unsigned short.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned short
     */
    inline unsigned short asUShort()
    {
        return asValue<unsigned short>();
    }

    /**
     * Get result as long long.
     * Throws error if type does not match.
     *
     * \returns Value as long long
     */
    inline long long asLongLong()
    {
        return asValue<long long>();
    }

    /**
     * Get result as unsigned long long.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned long long
     */
    inline unsigned long long asULongLong()
    {
        return asValue<unsigned long long>();
    }

    /**
     * Get result as double.
     * Throws error if type does not match.
     *
     * \returns Value as double
     */
    inline double asDouble()
    {
        return asValue<double>();
    }

    /**
     * Get result as float.
     * Throws error if type does not match.
     *
     * \returns Value as float
     */
    inline float asFloat()
    {
        return asValue<float>();
    }

    /**
     * Get result as boolean.
     * Throws error if type does not match.
     *
     * \returns Value as boolean
     */
    inline bool asBool()
    {
        return asValue<bool>();
    }

    /**
     * Get result as string.
     * Throws error if type does not match.
     *
     * \returns Value as string
     */
    inline std::string asString()
    {
        return asValue<std::string>();
    }

    /**
     * Get result as numeric value, for now converted to signed long.
     *
     * \returns Value as numeric (long)
     */
    long asNumber()
    {
#ifdef USE_BOOST_ANY
        if (this->type() == typeid(int))
            return boost::any_cast<int>(*this);
        if (this->type() == typeid(unsigned int))
            return static_cast<long>(boost::any_cast<unsigned int>(*this));
        if (this->type() == typeid(long))
            return boost::any_cast<long>(*this);
        if (this->type() == typeid(unsigned long))
            return static_cast<long>(boost::any_cast<unsigned long>(*this));
        if (this->type() == typeid(float))
            return boost::any_cast<float>(*this);
        if (this->type() == typeid(double))
            return boost::any_cast<double>(*this);
#else
        if (this->type() == typeid(int))
            return this->cast<int>();
        if (this->type() == typeid(unsigned int))
            return static_cast<long>(this->cast<unsigned int>());
        if (this->type() == typeid(long))
            return this->cast<long>();
        if (this->type() == typeid(unsigned long))
            return static_cast<long>(this->cast<unsigned long>());
        if (this->type() == typeid(float))
            return this->cast<float>();
        if (this->type() == typeid(double))
            return this->cast<double>();
#endif
        throw std::string("Error, invalid value, can't convert to a number");
    }

    std::string typeString() const
    {
        if (this->type() == typeid(int))
            return "int";
        if (this->type() == typeid(unsigned int))
            return "unsigned int";
        if (this->type() == typeid(long))
            return "long";
        if (this->type() == typeid(unsigned long))
            return "unsigned long";
        if (this->type() == typeid(short))
            return "short";
        if (this->type() == typeid(unsigned short))
            return "unsigned short";
        if (this->type() == typeid(long long))
            return "long long";
        if (this->type() == typeid(unsigned long long))
            return "unsigned long long";
        if (this->type() == typeid(char))
            return "char";
        if (this->type() == typeid(unsigned char))
            return "unsigned char";
        if (this->type() == typeid(double))
            return "double";
        if (this->type() == typeid(float))
            return "float";
        if (this->type() == typeid(std::string))
            return "string";
        if (this->type() == typeid(bool))
            return "bool";

        return "unknown";
    }
};

}

#endif
