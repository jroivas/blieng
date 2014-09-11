/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_BLIANY_H
#define __BLIENG_BLIANY_H

#include <ostream>
#include <boost/any.hpp>

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
class BliAny : public boost::any
{
public:
    /**
     * Dymmy constructor only calls boost constructor
     */
    BliAny() : boost::any() {}
    /**
     * Constructor with template
     *
     * \param value Value to be assigned, type defined by template
     */
    template<typename ValueType> BliAny(
        const ValueType & value) : boost::any(value) {}
    /**
     * Constructor, assign value from other object
     *
     * \param other Some boost::any instance
     */
    BliAny(const any & other) : boost::any(other) {}
    ~BliAny() {}

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

    /**
     * Print value to ostream.
     *
     * This is utilized for string conversions and printing the value.
     * Casts to proper known value and append to stream or print "Uknown type" if type not castable.
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

    #define AS_VALUE(Y, X)\
    X as ## Y()\
    {\
        if (this->type() == typeid(X)) {\
            return boost::any_cast<X>(*this);\
        }\
        throw std::string("Error, invalid value, can't convert to " # Y); \
    }

    /**
     * Get result as integer.
     * Throws error if type does not match.
     *
     * \returns Value as integer
     */
    AS_VALUE(Int, int);
    /**
     * Get result as unsigned integer.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned integer
     */
    AS_VALUE(UInt, unsigned int);
    /**
     * Get result as long.
     * Throws error if type does not match.
     *
     * \returns Value as long
     */
    AS_VALUE(Long, long);
    /**
     * Get result as unsigned long.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned long
     */
    AS_VALUE(ULong, unsigned long);
    /**
     * Get result as char.
     * Throws error if type does not match.
     *
     * \returns Value as char
     */
    AS_VALUE(Char, char);
    /**
     * Get result as unsigned char.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned char
     */
    AS_VALUE(UChar, unsigned char);
    /**
     * Get result as short.
     * Throws error if type does not match.
     *
     * \returns Value as short
     */
    AS_VALUE(Short, short);
    /**
     * Get result as unsigned short.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned short
     */
    AS_VALUE(UShort, unsigned short);
    /**
     * Get result as long long.
     * Throws error if type does not match.
     *
     * \returns Value as long long
     */
    AS_VALUE(LongLong, long long);
    /**
     * Get result as unsigned long long.
     * Throws error if type does not match.
     *
     * \returns Value as unsigned long long
     */
    AS_VALUE(ULongLong, unsigned long long);
    /**
     * Get result as double.
     * Throws error if type does not match.
     *
     * \returns Value as double
     */
    AS_VALUE(Double, double);
    /**
     * Get result as float.
     * Throws error if type does not match.
     *
     * \returns Value as float
     */
    AS_VALUE(Float, float);
    /**
     * Get result as boolean.
     * Throws error if type does not match.
     *
     * \returns Value as boolean
     */
    AS_VALUE(Bool, bool);
    /**
     * Get result as string.
     * Throws error if type does not match.
     *
     * \returns Value as string
     */
    AS_VALUE(String, std::string);

    #undef AS_VALUE

    /**
     * Get result as numeric value, for now converted to signed long.
     *
     * \returns Value as numeric (long)
     */
    long asNumber()
    {
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

        return "unknown";
    }
};

}

#endif
