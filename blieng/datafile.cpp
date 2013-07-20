#include "datafile.h"
#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <boost/filesystem/fstream.hpp>

using blieng::DataFile;

static unsigned int __key_size = 256/8;

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

char *DataFile::obfuscateSimple(const char *data, unsigned int len)
{
    char *res = (char *)calloc(1, len + 1);
    char *tmp = res;
    const char *ptr = data;
    while (len > 0) {
        *tmp = *ptr ^ 0x7f ^ len;
        tmp++;
        ptr++;
        len--;
    }
    return res;
}

bool DataFile::read(std::string key)
{
    boost::filesystem::ifstream fd(_name, std::ios_base::in | std::ofstream::binary);
    if (!fd.is_open()) return false;

    while (!fd.eof()) {
        uint32_t namelen;
        fd.read((char*)&namelen, sizeof(uint32_t));
        if (fd.eof()) break;

        char *ob_name = (char*)calloc(1, namelen+1);
        fd.read(ob_name, namelen);
        if (fd.eof()) break;

        char *name = obfuscateSimple(ob_name, namelen);
        free(ob_name);
        ob_name = NULL;

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

        std::string sname = name;
        free(name);

        if (key != "") {
            DataFileObject *new_tmp = tmp->deobfuscate(key.c_str(), key.size(), sname);
            if (new_tmp) {
                free(tmp);
                tmp = new_tmp;
            }
        }
        _data[sname] = tmp;
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
            DataFileObject *new_tmp = tmp->obfuscate(key.c_str(), key.size(), di->first);
            if (new_tmp) {
                free(tmp);
                tmp = new_tmp;
            }
        }

        uint32_t itmp = di->first.length();
        fd.write((char*)&itmp, sizeof(uint32_t));

        char *ob_name = obfuscateSimple(di->first.c_str(), itmp);
        fd.write(ob_name, itmp);
        free(ob_name);
        ob_name = NULL;

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

unsigned char *blieng::DataFile::DataFileObject::setupKey(const char *key, unsigned int len, const char *iv, unsigned int iv_len)
{
    unsigned char *res = (unsigned char*)calloc(1, __key_size);
    unsigned int index = 0;
    #define KEY_INIT_LOOP(A_res, A_index, A_cnt, A_tmp) \
        while (A_cnt > 0) {\
            A_res[A_index] ^= *(A_tmp++);\
            (++A_index) %= __key_size;\
            A_cnt--;\
        }
        
    if (iv != NULL && iv_len > 0) {
        const char *tmp = iv;
        unsigned int cnt = iv_len;
        KEY_INIT_LOOP(res, index, cnt, tmp);
    }

    if (key != NULL && len > 0) {
        const char *tmp = key;
        unsigned int cnt = len;
        KEY_INIT_LOOP(res, index, cnt, tmp);
    }

    return res;
    #undef KEY_INIT_LOOP
}

#include "rijndael-alg-fst.h"
blieng::DataFile::DataFileObject* blieng::DataFile::DataFileObject::obfuscate(const char *key, unsigned int key_len, std::string seed)
{
    unsigned int olen = len/16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    uint32_t *key_data = (uint32_t*)calloc(sizeof(uint32_t),(MAXNR+1)*4);

    unsigned char *init_key = setupKey(seed.c_str(), seed.length());
    unsigned char *tmp_key = setupKey(key, key_len, (const char*)init_key, __key_size);
    free(init_key);
    init_key = NULL;

    int nr = rijndaelKeySetupEnc(key_data, (const unsigned char*)tmp_key, 256);
    free(tmp_key);
    tmp_key = NULL;
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
    key_data = NULL;

    blieng::DataFile::DataFileObject *res = new DataFileObject(tmp, olen);
    res->real_len = len;
    return res;
}

blieng::DataFile::DataFileObject* blieng::DataFile::DataFileObject::deobfuscate(const char *key, unsigned int key_len, std::string seed)
{
    unsigned int olen = len/16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    char *tmp = (char*)calloc(1, real_len);

    uint32_t *key_data = (uint32_t*)calloc(sizeof(uint32_t),(MAXNR+1)*4);

    unsigned char *init_key = setupKey(seed.c_str(), seed.length());
    unsigned char *tmp_key = setupKey(key, key_len, (const char*)init_key, __key_size);
    free(init_key);
    init_key = NULL;

    int nr = rijndaelKeySetupDec(key_data, (const unsigned char*)tmp_key, 256);
    free(tmp_key);
    tmp_key = NULL;
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
    key_data = NULL;

    blieng::DataFile::DataFileObject *res = new DataFileObject(tmp, real_len);
    return res;
}
