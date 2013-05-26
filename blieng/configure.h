#ifndef __BLIENG_CONFIGURE_H
#define __BLIENG_CONFIGURE_H

#include "bliobject.h"
#include <iostream>
#include <vector>
#include <map>
#include <json/reader.h>

namespace blieng
{

class Configure : public blieng::BliObject
{
public:
    typedef enum { KeyString, KeyDouble, KeyUInt, KeyInt, KeyBool } key_type_t;
    static Configure *getInstance();
    static Configure *getInstance(std::string config);
    void load(std::string config_file);

    void addKey(std::string val, key_type_t keytype);
    bool validate();

private:
    Configure();
    void parse();

    std::string config_file;
    Json::Value data_json;
    std::map<std::string, key_type_t> keys;
};

}

#endif
