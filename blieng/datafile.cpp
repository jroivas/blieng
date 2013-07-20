#include "datafile.h"
#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <boost/filesystem/fstream.hpp>

using blieng::DataFile;

DataFile::DataFile(std::string name) : _name(name)
{
}

std::string DataFile::unifyName(std::string name)
{
    std::string tmp = "";

    bool got_letter = false;
    std::string::iterator ni = name.begin();
    char back = 0;
    while (ni != name.end()) {
        if (*ni == '\n' || *ni == '\r' || *ni == 0 || *ni == '!' || *ni == '|') {
            // Blacklist
        } else {
            if (got_letter || (*ni != '/' && *ni != '\\')) {
                if (*ni == '/' || *ni == '\\')  {
                    back = *ni;
                } else {
                    if (back != 0) tmp += back;
                    back = 0;
                    tmp += *ni;
                }
                got_letter = true;
            }
        }
        ni++;
    }

    return tmp;
}

blieng::DataFile::DataFileObject *DataFile::getObject(std::string name)
{
    name = unifyName(name);
    if (name == "") return NULL;

    std::map<std::string, DataFileObject*>::iterator di = _data.find(name);
    if (di != _data.end()) return di->second;

    return NULL;
}

unsigned int DataFile::getData(std::string name, const char **data)
{
    DataFileObject *obj = getObject(name);
    if (obj != NULL) {
        if (data != NULL) *data = obj->data;
        return obj->len;
    }
    return 0;
}

bool DataFile::addData(std::string name, std::string data)
{
    name = unifyName(name);
    if (name == "") return false;
    DataFileObject *tmp = new DataFileObject(data.c_str(), data.size() + 1);

    _data[name] = tmp;

    return true;
}

bool DataFile::addData(std::string name, char *data, unsigned int len)
{
    name = unifyName(name);
    if (name == "") return false;
    DataFileObject *tmp = new DataFileObject(data, len);

    _data[name] = tmp;

    return true;
}

bool DataFile::read(std::string key)
{
    boost::filesystem::ifstream fd(_name, std::ios_base::in | std::ofstream::binary);
    if (!fd.is_open()) return false;

    while (!fd.eof()) {
        uint32_t namelen;
        fd.read((char*)&namelen, sizeof(uint32_t));
        if (fd.eof()) break;

        char *name = (char*)calloc(1, namelen+1);
        fd.read(name, namelen);
        if (fd.eof()) break;

        uint32_t datalen;
        fd.read((char*)&datalen, sizeof(uint32_t));
        if (fd.eof()) break;

        uint32_t datareallen;
        fd.read((char*)&datareallen, sizeof(uint32_t));
        if (fd.eof()) break;

        char *data = (char*)malloc(datalen);
        fd.read(data, datalen);
        if (fd.eof()) break;

        DataFileObject *tmp = new DataFileObject(data, datalen);
        tmp->real_len = datareallen;

        if (key != "") {
            DataFileObject *new_tmp = tmp->deobfuscate(key.c_str(), key.size());
            if (new_tmp) {
                free(tmp);
                tmp = new_tmp;
            }
        }
        _data[name] = tmp;
    }

    fd.close();

    return true;
}

bool DataFile::write(std::string key)
{
    boost::filesystem::ofstream fd(_name, std::ios_base::out | std::ios_base::trunc | std::ofstream::binary);
    if (!fd.is_open()) return false;

    std::map<std::string, DataFileObject*>::iterator di = _data.begin();
    while (di != _data.end()) {
        DataFileObject *tmp = di->second;
        if (key != "") {
            DataFileObject *new_tmp = tmp->obfuscate(key.c_str(), key.size());
            if (new_tmp) {
                free(tmp);
                tmp = new_tmp;
            }
        }

        uint32_t itmp = di->first.length();
        fd.write((char*)&itmp, sizeof(uint32_t));

        fd.write(di->first.c_str(), itmp);

        itmp = tmp->len;
        fd.write((char*)&itmp, sizeof(uint32_t));

        itmp = tmp->real_len;
        fd.write((char*)&itmp, sizeof(uint32_t));

        fd.write(tmp->data, tmp->len);
        fd.flush();
        di++;
    }

    fd.close();

    return true;
}


blieng::DataFile::DataFileObject::DataFileObject(const char *new_data, unsigned int new_len) : len(new_len), real_len(new_len)
{
    data = (char*)malloc(len);
    memcpy(data, new_data, len);
}

unsigned char *blieng::DataFile::DataFileObject::setupKey(const char *key, unsigned int len)
{
    unsigned char *res = (unsigned char*)calloc(1, 256/8);
    unsigned int cnt = len;
    unsigned int index = 0;
    
    const char *tmp = key;
    
    while (cnt > 0) {
        res[index] ^= *tmp;
        index++;
        index %= (256/8);
        tmp++;
        cnt--; 
    }

    return res;
}

#include "rijndael-alg-fst.h"
blieng::DataFile::DataFileObject* blieng::DataFile::DataFileObject::obfuscate(const char *key, unsigned int key_len)
{
    unsigned int olen = len/16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    uint32_t *key_data = (uint32_t*)calloc(sizeof(uint32_t),(MAXNR+1)*4);
    unsigned char *tmp_key = setupKey(key, key_len);
    int nr = rijndaelKeySetupEnc(key_data, (const unsigned char*)tmp_key, 256);
    free(tmp_key);
    if (nr == 0) {
        free(key_data);
        return NULL;
    }

    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned int cnt = len;
    unsigned int ocnt = olen;

    const char *pos = data;
    char *tmp = (char*)calloc(1, len);
    char *opos = tmp;
    while (cnt > 0) {
        for (int i=0; i <16; i++) {
            if (cnt > 0) {
                in_block[i] = *pos;
                pos++;
                cnt--;
            } else {
                in_block[i] = 0;
            }
        }
        rijndaelEncrypt(key_data, nr, in_block, out_block);
        for (int i = 0; i < 16; i++) {
            if (ocnt > 0) {
                *opos = out_block[i];
                opos++;
                ocnt--;
            }
        }
    }
    free(key_data);

    blieng::DataFile::DataFileObject *res = new DataFileObject(tmp, olen);
    res->real_len = len;
    return res;
}

blieng::DataFile::DataFileObject* blieng::DataFile::DataFileObject::deobfuscate(const char *key, unsigned int key_len)
{
    unsigned int olen = len/16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    char *tmp = (char*)calloc(1, real_len);

    uint32_t *key_data = (uint32_t*)calloc(sizeof(uint32_t),(MAXNR+1)*4);
    unsigned char *tmp_key = setupKey(key, key_len);
    int nr = rijndaelKeySetupDec(key_data, (const unsigned char*)tmp_key, 256);
    free(tmp_key);
    if (nr == 0) {
        free(key_data);
        return NULL;
    }

    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned int cnt = len;
    unsigned int ocnt = real_len;
    const char *pos = data;
    char *opos = tmp;
    while (cnt > 0) {
        for (int i=0; i < 16; i++) {
            if (cnt > 0) {
                in_block[i] = *pos;
                pos++;
                cnt--;
            } else {
                in_block[i] = 0;
            }
        }
        rijndaelDecrypt(key_data, nr, in_block, out_block);
        for (int i = 0; i < 16; i++) {
            if (ocnt > 0) {
                *opos = out_block[i];
                opos++;
                ocnt--;
            }
        }
    }
    free(key_data);

    blieng::DataFile::DataFileObject *res = new DataFileObject(tmp, real_len);
    return res;
}
