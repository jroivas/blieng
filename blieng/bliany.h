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
    template<typename ValueType> BliAny(const ValueType & value) : boost::any(value) {}
    /**
     * Constructor, assign value from other object
     *
     * \param other Some boost::any instance
     */
    BliAny(const any & other) : boost::any(other) {}
    ~BliAny() {}

    #define CAST_TO(X,Y,Z) \
    if (Y.type() == typeid(X)) {\
        Z << boost::any_cast<X>(Y); \
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
        CAST_TO(bool, obj, ostr)
        CAST_TO(std::string, obj, ostr)
        {
            ostr << "Unknown type";
        }

        return ostr;
    }
    #undef CAST_TO
    #define AS_VALUE(Y, X)\
    X as ## Y() {\
        if (this->type() == typeid(X)) {\
            return boost::any_cast<X>(*this);\
        }\
        /*doDebug("Error, invalid value, can't convert to " # Y);*/ \
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
     * Get result as numberic value, for now coverted to signed long.
     *
     * \returns Value as numeric (long)
     */
    long asNumber()
    {
        if (this->type() == typeid(int)) return boost::any_cast<int>(*this);
        if (this->type() == typeid(unsigned int)) return static_cast<long>(boost::any_cast<unsigned int>(*this));
        if (this->type() == typeid(long)) return boost::any_cast<long>(*this);
        if (this->type() == typeid(unsigned long)) return static_cast<long>(boost::any_cast<unsigned long>(*this));
        if (this->type() == typeid(float)) return boost::any_cast<float>(*this);
        if (this->type() == typeid(double)) return boost::any_cast<double>(*this);
        throw std::string("Error, invalid value, can't convert to a number");
    }
};

}

#endif
