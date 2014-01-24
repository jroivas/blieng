#ifndef __BLIENG_CONFIGURE_H
#define __BLIENG_CONFIGURE_H

#include "bliobject.h"
#include "data.h"
#include <iostream>
#include <vector>
#include <map>
#include "json.h"
//#include <boost/thread/mutex.hpp>

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

class Configure : public blieng::BliObject
{
public:
    Configure(shared_ptr<blieng::Data> data);
    virtual ~Configure();

    typedef enum { KeyString, KeyDouble, KeyUInt, KeyInt, KeyBool, KeyStringList, KeyIntList } key_type_t;
    bool load(const std::string &config_file);

    void addKey(const std::string &val, key_type_t keytype);
    void addOptionalKey(const std::string &val, key_type_t keytype);
    bool validate();

private:
    void parse();

    std::string config_file;
    json_value *data_json;
    std::map<std::string, key_type_t> keys;
    std::map<std::string, key_type_t> opt_keys;

    //boost::mutex key_mutex;
    shared_ptr<blieng::Data> data;
};

}

#endif
