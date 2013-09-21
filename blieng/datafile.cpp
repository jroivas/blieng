#include "datafile.h"
#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>

using blieng::DataFile;

static unsigned int __key_size = 256/8;
//typedef std::pair<std::string, blieng::DataFile::DataFileObject*> datafile_item_t;
//typedef std::pair<std::string, blieng::DataFile::DataFileObject*> datafile_item_t;
typedef auto_map<std::string,blieng::DataFile::DataFileObject>::iterator_value datafile_item_t;

DataFile::DataFile()
{
    _ok = false;
}

DataFile::DataFile(std::string name)
{
    setName(name);
}

void DataFile::setName(std::string name)
{
    _ok = true;
    _name = name;
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
        ++ni;
    }

    return tmp;
}

std::vector<std::string> blieng::DataFile::listFiles()
{
    std::vector<std::string> res;
    
    BOOST_FOREACH(datafile_item_t val, _data) {
        res.push_back(val->key);
    }

    return res;
}

blieng::DataFile::DataFileObject *DataFile::getObject(std::string name)
{
    name = unifyName(name);
    if (name == "") return NULL;

    //std::map<std::string, DataFileObject*>::iterator di = _data.find(name);
    auto_map<std::string, DataFileObject>::iterator di = _data.find(name);
    //if (di != _data.end()) return di->second;
    if (di != _data.end()) return _data[di];

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

    //DataFileObject *tmp = new DataFileObject(data.c_str(), data.size() + 1);

    auto_ptr<DataFileObject> tmp(new DataFileObject(data.c_str(), data.size() + 1));

    _data[name] = tmp;

    return true;
}

bool DataFile::addData(std::string name, char *data, unsigned int len)
{
    name = unifyName(name);
    if (name == "") return false;
    //DataFileObject *tmp = new DataFileObject(data, len);
    auto_ptr<DataFileObject> tmp(new DataFileObject(data, len));

    _data[name] = tmp;

    return true;
}

class blieng::SafeDataPtr
{
public:
    SafeDataPtr(char data[], unsigned int len) : data(data), len(len) {
    }
    virtual ~SafeDataPtr() {
        if (data != NULL) delete [] data;
        data = NULL;
    }

    const char * getData() { return data; }
    unsigned int length() { return len; }
    unsigned int size() { return len; }

private:
    char *data;
    unsigned int len;
};

auto_ptr<blieng::SafeDataPtr> DataFile::obfuscateSimple(const char *data, unsigned int len)
{
    //char *res = (char *)calloc(1, len + 1);
    char *res = new char[len + 1];
    char *tmp = res;
    const char *ptr = data;
    while (len > 0) {
        *tmp = *ptr ^ 0x7f ^ len;
        tmp++;
        ptr++;
        len--;
    }
    return auto_ptr<blieng::SafeDataPtr>(new blieng::SafeDataPtr(res, len));
}

bool DataFile::read(const char *key, unsigned int key_len)
{
    if (!_ok) return false;

    boost::filesystem::ifstream fd(_name, std::ios_base::in | std::ofstream::binary);
    if (!fd.is_open()) return false;

    while (!fd.eof()) {
        uint32_t namelen;
        fd.read((char*)&namelen, sizeof(uint32_t));
        if (fd.eof()) break;

        //char *ob_name = (char*)calloc(1, namelen+1);
        char *ob_name = new char[namelen+1];
        fd.read(ob_name, namelen);
        if (fd.eof()) break;

        //char *name = obfuscateSimple(ob_name, namelen);
        auto_ptr<SafeDataPtr> name = obfuscateSimple(ob_name, namelen);
        //fd.write(ob_name->getData(), ob_name->length());
        //free(ob_name);
        delete [] ob_name;
        ob_name = NULL;

        uint32_t datalen;
        fd.read((char*)&datalen, sizeof(uint32_t));
        if (fd.eof()) break;

        uint32_t datareallen;
        fd.read((char*)&datareallen, sizeof(uint32_t));
        if (fd.eof()) break;

        //char *data = (char*)malloc(datalen);
        char *data = new char[datalen];
        fd.read(data, datalen);
        if (fd.eof()) break;

        //DataFileObject *tmp = new DataFileObject(data, datalen);
        auto_ptr<DataFileObject> tmp(new DataFileObject(data, datalen));
        tmp->real_len = datareallen;

        //std::string sname = name;
        std::string sname;
        sname.append(name->getData(), name->length());
        //free(name);
        //delete [] name;

        if (key != NULL && key_len > 0) {
            //FIXME
            //DataFileObject *new_tmp = tmp->deobfuscate(key, key_len, sname);
            auto_ptr<DataFileObject> new_tmp(tmp->deobfuscate(key, key_len, sname));
            if (new_tmp.get()) {
                // cppcheck-suppress mismatchAllocDealloc
                //free(tmp);
                //delete [] tmp;
                tmp = new_tmp;
            }
        }
        _data[sname] = tmp;
    }

    fd.close();

    return true;
}

bool DataFile::write(const char *key, unsigned int key_len)
{
    if (!_ok) return false;

    boost::filesystem::ofstream fd(_name, std::ios_base::out | std::ios_base::trunc | std::ofstream::binary);
    if (!fd.is_open()) return false;

    //std::map<std::string, DataFileObject*>::iterator di = _data.begin();
    auto_map<std::string, DataFileObject>::iterator di = _data.begin();
    while (di != _data.end()) {
        //DataFileObject *tmp = di->val;
        //auto_ptr<DataFileObject> tmp = *di;
        DataFileObject* tmp = _data[di];
        //const DataFileObject* tmp = (*di)->getValue();
        if (key != NULL && key_len > 0) {
            //DataFileObject *new_tmp = tmp->obfuscate(key, key_len, di->first);
            auto_ptr<DataFileObject> new_tmp = tmp->obfuscate(key, key_len, (*di)->key);
            if (new_tmp.get()) {
                //free(tmp);
                _data[di] = new_tmp;
            }
        }

        uint32_t itmp = (*di)->key.length();
        fd.write((char*)&itmp, sizeof(uint32_t));

        //char *ob_name = obfuscateSimple(di->first.c_str(), itmp);
        auto_ptr<SafeDataPtr> ob_name = obfuscateSimple((*di)->key.c_str(), itmp);
        fd.write(ob_name->getData(), ob_name->length());
        //free(ob_name);
        //delete [] ob_name;
        //ob_name = NULL;

        itmp = tmp->len;
        fd.write((char*)&itmp, sizeof(uint32_t));

        itmp = tmp->real_len;
        fd.write((char*)&itmp, sizeof(uint32_t));

        fd.write(tmp->data, tmp->len);
        fd.flush();
        ++di;
    }

    fd.close();

    return true;
}


blieng::DataFile::DataFileObject::DataFileObject(const char *new_data, unsigned int new_len) : len(new_len), real_len(new_len)
{
    //data = (char*)malloc(len);
    data = new char[len];
    memcpy(data, new_data, len);
}

unsigned char *blieng::DataFile::DataFileObject::setupKey(const char *key, unsigned int len, const char *iv, unsigned int iv_len)
{
    //unsigned char *res = (unsigned char*)calloc(1, __key_size);
    unsigned char *res = new unsigned char[__key_size];
    unsigned int index = 0;
#if 1
    #define KEY_INIT_LOOP(A_res, A_index, A_cnt, A_tmp) \
        while (A_cnt > 0) {\
            A_res[A_index++] ^= *(A_tmp++);\
            A_index %= (__key_size-1);\
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
#else
    if (iv != NULL && iv_len > 0) {
        const char *tmp = iv;
        unsigned int cnt = iv_len;
        while (cnt > 0) {
            res[index] ^= *tmp;
            index++;
            index %= (__key_size);
            tmp++;
            cnt--;
        }
    }

    if (key != NULL && len > 0) {
        const char *tmp = key;
        unsigned int cnt = len;
        res[index] ^= *tmp;
        index++;
        index %= (__key_size);
        tmp++;
        cnt--;
    }
#endif

    return res;
    #undef KEY_INIT_LOOP
}

#include "rijndael-alg-fst.h"
auto_ptr<blieng::DataFile::DataFileObject> blieng::DataFile::DataFileObject::obfuscate(const char *key, unsigned int key_len, std::string seed)
{
    unsigned int olen = len/16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    //uint32_t *key_data = (uint32_t*)calloc(sizeof(uint32_t),(MAXNR+1)*4);
    uint32_t *key_data = new uint32_t[(MAXNR+1)*4];

    unsigned char *init_key = setupKey(seed.c_str(), seed.length());
    unsigned char *tmp_key = setupKey(key, key_len, (const char*)init_key, __key_size);
    delete [] init_key;
    //free(init_key);
    init_key = NULL;

    int nr = rijndaelKeySetupEnc(key_data, (const unsigned char*)tmp_key, 256);
    //free(tmp_key);
    delete [] tmp_key;
    tmp_key = NULL;
    if (nr == 0) {
        //free(key_data);
        delete [] key_data;
        //return NULL;
        return auto_ptr<DataFileObject>();
    }

    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memcpy(iv, key, key_len>16?16:key_len);

    unsigned int cnt = len;
    unsigned int ocnt = olen;

    char *pos = data;
    //char *tmp = (char*)calloc(1, olen);
    char *tmp = new char[olen];
    char *opos = tmp;
    unsigned int blocknum = 0;
    while (cnt > 0) {
        for (int i=0; i < 16; i++) {
            if (cnt > 0) {
                in_block[i] = *pos ^ iv[i];
                pos++;
                cnt--;
            } else {
                in_block[i] = 0x0 ^ iv[i];
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
        memcpy(iv, out_block, 16);
        blocknum++;
    }
    //free(key_data);
    delete [] key_data;
    key_data = NULL;

    //blieng::DataFile::DataFileObject *res = new DataFileObject(tmp, olen);
    auto_ptr<DataFileObject> res(new DataFileObject(tmp, olen));
    res->real_len = len;
    return res;
}

auto_ptr<blieng::DataFile::DataFileObject> blieng::DataFile::DataFileObject::deobfuscate(const char *key, unsigned int key_len, std::string seed)
{
    //uint32_t *key_data = (uint32_t*)calloc(sizeof(uint32_t),(MAXNR+1)*4);
    uint32_t *key_data = new uint32_t[(MAXNR+1)*4];

    unsigned char *init_key = setupKey(seed.c_str(), seed.length());
    unsigned char *tmp_key = setupKey(key, key_len, (const char*)init_key, __key_size);
    //free(init_key);
    delete [] init_key;
    init_key = NULL;

    int nr = rijndaelKeySetupDec(key_data, (const unsigned char*)tmp_key, 256);
    //free(tmp_key);
    delete [] tmp_key;
    tmp_key = NULL;
    if (nr == 0) {
        //free(key_data);
        delete [] key_data;
        return auto_ptr<DataFileObject>();
    }

    //char *tmp = (char*)calloc(1, real_len);
    char *tmp = new char[real_len];
    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memcpy(iv, key, key_len>16?16:key_len);

    unsigned int cnt = len;
    unsigned int ocnt = real_len;
    char *pos = data;
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
                *opos = out_block[i] ^ iv[i];
                opos++;
                ocnt--;
            }
        }
        memcpy(iv, in_block, 16);
    }
    //free(key_data);
    delete [] key_data;
    key_data = NULL;

    //blieng::DataFile::DataFileObject *res = new DataFileObject(tmp, real_len);
    auto_ptr<DataFileObject> res(new DataFileObject(tmp, real_len));
    res->real_len = real_len;
    return res;
}
