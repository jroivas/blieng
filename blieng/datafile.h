#ifndef __BLIENG_DATA_FILE_H
#define __BLIENG_DATA_FILE_H

#include <iostream>
#include <map>

namespace blieng
{

class DataFile
{
public:
    class DataFileObject
    {
    public:
        DataFileObject() : data(NULL), len(0), real_len(0) {}
        DataFileObject(const char *new_data, unsigned int new_len);
        DataFileObject* obfuscate(const char *key, unsigned int len);
        DataFileObject* deobfuscate(const char *key, unsigned int len);

        char *data;
        unsigned int len;
        friend class DataFile;

    protected:
        unsigned char *setupKey(const char *key, unsigned int len);
        unsigned int real_len;
    };

    DataFile(std::string name);
    bool addData(std::string name, std::string data);
    bool addData(std::string name, char *data, unsigned int len);
    unsigned int getData(std::string name, const char **data);
    DataFileObject *getObject(std::string name);

    bool read(std::string key="");
    bool write(std::string key="");

private:
    std::string _name;
    std::string unifyName(std::string name);

    std::map<std::string, DataFileObject*> _data;
};

}

#endif
