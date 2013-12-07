#ifndef __BLIENG_CONFIGURE_H
#define __BLIENG_CONFIGURE_H

#include "bliobject.h"
#include "data.h"
#include <iostream>
#include <vector>
#include <map>
#include "json.h"

#if defined(__cplusplus) && __cplusplus >= 201103L
#include <memory>
using namespace std;
#else
#include <boost/smart_ptr/shared_ptr.hpp>
using namespace boost;
#endif

namespace blieng
{

class Configure : public blieng::BliObject
{
public:
    //Configure();
    Configure(shared_ptr<blieng::Data> data);
    virtual ~Configure();

    typedef enum { KeyString, KeyDouble, KeyUInt, KeyInt, KeyBool, KeyStringList } key_type_t;
    void load(std::string config_file);

    void addKey(std::string val, key_type_t keytype);
    void addOptionalKey(std::string val, key_type_t keytype);
    bool validate();

private:
    void parse();

    std::string config_file;
    json_value *data_json;
    std::map<std::string, key_type_t> keys;
    std::map<std::string, key_type_t> opt_keys;

    shared_ptr<blieng::Data> data;
};

}

#endif
