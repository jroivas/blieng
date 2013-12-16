#ifndef __BLIENG_DATA_FILE_H
#define __BLIENG_DATA_FILE_H

#include <iostream>
#include "auto_map.h"
#include <vector>
#include <memory>
#ifdef ANDROID
#include <android/asset_manager.h>
#endif

using std::unique_ptr;

namespace blieng
{

class SafeDataPtr;

class DataFile
{
public:
    class DataFileObject
    {
    public:
        DataFileObject() : dataptr(nullptr), len(0), real_len(0) {}
        DataFileObject(const char *new_data, unsigned int new_len);
        ~DataFileObject() {
            if (dataptr != nullptr) delete dataptr;
        }

        std::unique_ptr<DataFileObject> obfuscate(const char *key, unsigned int len, std::string seed="");
        std::unique_ptr<DataFileObject> deobfuscate(const char *key, unsigned int len, std::string seed="");

        inline unsigned int length() const { return len; }
        char *get() { return dataptr; }
        char *get() const { return dataptr; }

    protected:
        char *dataptr;
        unsigned int len;
        friend class DataFile;

    protected:
        std::unique_ptr<blieng::SafeDataPtr> setupKey(const char *key, unsigned int len, const char *iv=nullptr, unsigned int iv_len=0);
        unsigned int real_len;
    };

    DataFile();
    DataFile(std::string name);
    virtual ~DataFile();

    void setName(std::string name);
    bool isValid() const { return _ok; }

    bool addData(std::string name, std::string data);
    bool addData(std::string name, char *data, unsigned int len);
    unsigned int getData(std::string name, const char **data);
    const DataFileObject *getObject(std::string name);

    std::vector<std::string> listFiles();

    bool read(const char *key, unsigned int key_len);
    bool write(const char *key, unsigned int key_len);
    std::unique_ptr<SafeDataPtr> obfuscateSimple(const char *data, unsigned int len);

private:
    std::string _name;
    std::string unifyName(std::string name);

    auto_map<std::string, DataFileObject> _data;
    bool _ok;
};

}

#endif
