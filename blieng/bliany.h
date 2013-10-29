#ifndef __BLIENG_BLIANY_H
#define __BLIENG_BLIANY_H

#include <boost/any.hpp>

namespace blieng
{

/**
 * Inheriting boost class to allow easy conversion to stream with <<
 * Also provides easy access to converted values
 */
class BliAny : public boost::any
{
public:
    BliAny() : boost::any() {}
    template<typename ValueType> BliAny(const ValueType & value) : boost::any(value) {}
    BliAny(const any & other) : boost::any(other) {}
    ~BliAny() {}

    #define CAST_TO(X,Y,Z) \
    if (Y.type() == typeid(X)) {\
        Z << boost::any_cast<X>(Y); \
    } else

    friend std::ostream& operator<< (std::ostream& o, BliAny const& obj)
    {
        CAST_TO(int, obj, o)
        CAST_TO(unsigned int, obj, o)
        CAST_TO(long, obj, o)
        CAST_TO(unsigned long, obj, o)
        CAST_TO(float, obj, o)
        CAST_TO(double, obj, o)
        CAST_TO(char, obj, o)
        CAST_TO(const char *, obj, o)
        CAST_TO(char, obj, o)
        CAST_TO(bool, obj, o)
        CAST_TO(std::string, obj, o)
        {
            o << "Unknown type";
        }

        return o;
    }
    #undef CAST_TO

    #define AS_VALUE(Y, X)\
    X as ## Y() {\
        if (this->type() == typeid(X)) {\
            return boost::any_cast<X>(*this);\
        }\
        /*std::cerr << "Error, invalid value, can't convert to " # Y;*/\
        throw std::string("Error, invalid value, can't convert to " # Y); \
    }

    AS_VALUE(Int, int);
    AS_VALUE(UInt, unsigned int);
    AS_VALUE(Long, long);
    AS_VALUE(ULong, unsigned long);
    AS_VALUE(Double, double);
    AS_VALUE(Float, float);
    AS_VALUE(Bool, bool);
    AS_VALUE(String, std::string);

    #undef AS_VALUE

    long asNumber()
    {
        if (this->type() == typeid(int)) return boost::any_cast<int>(*this);
        if (this->type() == typeid(unsigned int)) return boost::any_cast<unsigned int>(*this);
        if (this->type() == typeid(long)) return boost::any_cast<long>(*this);
        if (this->type() == typeid(unsigned long)) return static_cast<long>(boost::any_cast<unsigned long>(*this));
        if (this->type() == typeid(float)) return boost::any_cast<float>(*this);
        if (this->type() == typeid(double)) return boost::any_cast<double>(*this);
        //std::cerr << "Error, invalid value, can't convert to a number\n";
        throw "Error, invalid value, can't convert to a number";
    }
};

}

#endif
