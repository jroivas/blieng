/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/datafile.h"

#include <stdint.h>
#include <string.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <sys/time.h>

#include <cstdlib>
#include <string>
#include <vector>

#include "blieng/rijndael-alg-fst.h"

using blieng::DataFile;

static unsigned int __key_size = 256 / 8;

/**
 * Helper class to safely pass pointer.
 * Pointer data is copied and automatically deleted.
 */
class blieng::SafeDataPtr
{
public:
    /**
     * Initialize with char data.
     * Reserves memory and copies the data
     *
     * \param _data The data
     * \param _len Length of the data
     */
    SafeDataPtr(char _data[], unsigned int _len) : len(_len) {
        dataptr = new char[len];
        memmove(dataptr, _data, len);
    }
    /**
     * Initialize with unsigned char data.
     * Reserves memory and copies the data.
     * Casts the data to signed format.
     *
     * \param _data The data
     * \param _len Length of the data
     */
    SafeDataPtr(unsigned char _data[], unsigned int _len) : len(_len) {
        dataptr = new char[len];
        memmove(dataptr, reinterpret_cast<char*>(_data), len);
    }
    /**
     * Automatically delete the data
     */
    virtual ~SafeDataPtr() {
        if (dataptr != nullptr) delete [] dataptr;
        dataptr = nullptr;
        len = 0;
    }

    /**
     * Get pointer to the data
     *
     * \returns Pointer to the data
     */
    const char * getData() const { return dataptr; }
    /**
     * Get lenght of the data
     *
     * \returns Length of the data
     */
    unsigned int length() const { return len; }
    /**
     * Get lenght of the data
     *
     * \returns Length of the data
     */
    unsigned int size() const { return len; }

private:
    char *dataptr;
    unsigned int len;
};


DataFile::DataFile()
{
    _ok = false;
}

DataFile::DataFile(const std::string &name)
{
    setName(name);
}

DataFile::~DataFile()
{
    // Should we write?
}

void DataFile::setName(const std::string &name)
{
    _ok = true;
    _name = name;
}

std::string DataFile::unifyName(const std::string &name) const
{
    std::string tmp = "";

    bool got_letter = false;
    auto ni = name.cbegin();
    char back = 0;
    while (ni != name.cend()) {
        if (*ni == '\n' ||
            *ni == '\r' ||
            *ni == 0 ||
            *ni == '!' ||
            *ni == '|') {
            // FIXME: Blacklist?
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

std::vector<std::string> blieng::DataFile::listFiles() const
{
    std::vector<std::string> res;

    for(auto val : _data) {
        res.push_back(val->key);
    }

    return res;
}

const blieng::DataFile::DataFileObject *DataFile::getObject(
    const std::string &name)
{
    std::string uname = unifyName(name);
    if (uname == "") return nullptr;

    auto di = _data.find(uname);
    if (di != _data.end()) return _data[di];

    return nullptr;
}

unsigned int DataFile::getData(
    const std::string &name,
    const char **data)
{
    const DataFileObject *obj = getObject(name);
    if (obj != nullptr) {
        if (data != nullptr) *data = obj->get();
        return obj->length();
    }
    return 0;
}

bool DataFile::addData(
    const std::string &name,
    const std::string &data)
{
    std::string uname = unifyName(name);
    if (uname == "") return false;

    std::unique_ptr<DataFileObject> tmp(
        new DataFileObject(data.c_str(), data.size() + 1));
    _data[uname] = std::move(tmp);

    return true;
}

bool DataFile::addData(
    const std::string &name,
    const char *data,
    unsigned int len)
{
    std::string uname = unifyName(name);
    if (uname == "") return false;

    std::unique_ptr<DataFileObject> tmp(new DataFileObject(data, len));
    _data[uname] = std::move(tmp);

    return true;
}

std::unique_ptr<blieng::SafeDataPtr> DataFile::obfuscateSimple(
    const char *data,
    unsigned int len)
{
    unsigned int orig_len = len;

    std::unique_ptr<char[]> res(new char[len + 1]);
    memset(res.get(), 0, len + 1);
    char *tmp = res.get();

    const char *ptr = data;
    while (len > 0) {
        *tmp = *ptr ^ 0x7f ^ static_cast<int>(len);
        tmp++;
        ptr++;
        len--;
    }
    return std::unique_ptr<blieng::SafeDataPtr>(
        new blieng::SafeDataPtr(res.get(), orig_len));
}

#ifdef ANDROID
#include <QtCore>
#endif
bool DataFile::read(const char *key, unsigned int key_len)
{
    if (!_ok) return false;
    // TODO: Tidy

#ifdef ANDROID
    QFile asset_file("assets:/data/data.dat");
    if (!asset_file.open(QIODevice::ReadOnly)) {
        return false;
    }
#else
    boost::filesystem::ifstream fd(
        _name, std::ios_base::in | std::ofstream::binary);
    if (!fd.is_open()) return false;
#endif

#ifdef ANDROID
    while (!asset_file.atEnd()) {
#else
    while (!fd.eof()) {
#endif
        uint32_t namelen = 0;
#ifdef ANDROID
        int nb_read = 0;
        nb_read = asset_file.read(
            reinterpret_cast<char*>(&namelen),
            sizeof(uint32_t));
        if (nb_read == 0) break;
#else
        fd.read(reinterpret_cast<char*>(&namelen), sizeof(uint32_t));
        if (fd.eof()) break;
#endif
        BOOST_ASSERT(namelen > 0);
        BOOST_ASSERT(namelen < 0x2ff);

        std::unique_ptr<char[]> ob_name(new char[namelen+1]);
#ifdef ANDROID
        nb_read = asset_file.read(ob_name.get(), static_cast<int>(namelen));
        if (nb_read == 0) break;
#else
        fd.read(ob_name.get(), static_cast<int>(namelen));
        if (fd.eof()) break;
#endif

        auto name = obfuscateSimple(ob_name.get(), namelen);

        uint32_t datalen = 0;
#ifdef ANDROID
        nb_read = asset_file.read(
            reinterpret_cast<char*>(&datalen),
            sizeof(uint32_t));
        if (nb_read == 0) break;
#else
        fd.read(reinterpret_cast<char*>(&datalen), sizeof(uint32_t));
        if (fd.eof()) break;
#endif

        uint32_t datareallen = 0;
#ifdef ANDROID
        nb_read = asset_file.read(
            reinterpret_cast<char*>(&datareallen),
            sizeof(uint32_t));
        if (nb_read == 0) break;
#else
        fd.read(reinterpret_cast<char*>(&datareallen), sizeof(uint32_t));
        if (fd.eof()) break;
#endif

        std::unique_ptr<char[]> data(new char[datalen]);
#ifdef ANDROID
        nb_read = asset_file.read(data.get(), static_cast<int>(datalen));
        if (nb_read == 0) break;
#else
        fd.read(data.get(), static_cast<int>(datalen));
        if (fd.eof()) break;
#endif

        std::unique_ptr<DataFileObject> tmp(
            new DataFileObject(data.get(), datalen));
        tmp->real_len = datareallen;

        std::string sname;
        sname.append(name->getData(), name->length());

        if (key != nullptr && key_len > 0) {
            std::unique_ptr<DataFileObject> new_tmp(
                tmp->deobfuscate(key, key_len, sname));
            if (new_tmp.get()) {
                tmp = std::move(new_tmp);
            }
        }
        _data[sname] = std::move(tmp);
    }

#ifdef ANDROID
    asset_file.close();
#else
    fd.close();
#endif

    return true;
}

bool DataFile::write(const char *key, unsigned int key_len)
{
    if (!_ok) return false;
#ifdef ANDROID
    return false;
#endif

    boost::filesystem::ofstream fd(
        _name,
        std::ios_base::out | std::ios_base::trunc | std::ofstream::binary);
    if (!fd.is_open()) return false;

    auto di = _data.begin();
    while (di != _data.end()) {
        DataFileObject* tmp = _data[di];
        if (key != nullptr && key_len > 0) {
            std::unique_ptr<DataFileObject> new_tmp = tmp->obfuscate(
                key,
                key_len,
                (*di)->key);
            if (new_tmp.get()) {
                _data[di] = std::move(new_tmp);
                tmp = _data[di];
            }
        }

        uint32_t itmp = (*di)->key.length();
        fd.write(reinterpret_cast<char*>(&itmp), sizeof(uint32_t));

        auto ob_name = obfuscateSimple((*di)->key.c_str(), itmp);
        fd.write(ob_name->getData(), static_cast<int>(ob_name->length()));

        itmp = tmp->len;
        fd.write(reinterpret_cast<char*>(&itmp), sizeof(uint32_t));

        itmp = tmp->real_len;
        fd.write(reinterpret_cast<char*>(&itmp), sizeof(uint32_t));

        fd.write(tmp->get(), static_cast<int>(tmp->length()));
        fd.flush();
        ++di;
    }

    fd.close();

    return true;
}


blieng::DataFile::DataFileObject::DataFileObject(
    const char *new_data,
    unsigned int new_len) :
    len(new_len), real_len(new_len)
{
    dataptr = new char[len];
    memmove(dataptr, new_data, len);
}

std::unique_ptr<blieng::SafeDataPtr> blieng::DataFile::DataFileObject::setupKey(
    const char *key,
    unsigned int key_len,
    const char *iv,
    unsigned int iv_len)
{
    std::unique_ptr<unsigned char[]> res(new unsigned char[__key_size]);
    memset(res.get(), 0, __key_size);
    unsigned int index = 0;

    #define KEY_INIT_LOOP(A_res, A_index, A_cnt, A_tmp)\
        while (A_cnt > 0) {\
            A_res[A_index++] ^= *(A_tmp++);\
            A_index %= (__key_size-1);\
            A_cnt--;\
        }

    if (iv != nullptr && iv_len > 0) {
        const char *tmp = iv;
        unsigned int cnt = iv_len;
        KEY_INIT_LOOP(res.get(), index, cnt, tmp);
    }

    if (key != nullptr && key_len > 0) {
        const char *tmp = key;
        unsigned int cnt = key_len;
        KEY_INIT_LOOP(res.get(), index, cnt, tmp);
    }

    return std::unique_ptr<SafeDataPtr>(new SafeDataPtr(res.get(), __key_size));
    #undef KEY_INIT_LOOP
}

std::unique_ptr<blieng::DataFile::DataFileObject>
blieng::DataFile::DataFileObject::obfuscate(
    const char *key,
    unsigned int key_len,
    const std::string &seed)
{
    unsigned int olen = len / 16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    std::unique_ptr<uint32_t[]> key_data(new uint32_t[(MAXNR+1)*4]);

    std::unique_ptr<SafeDataPtr> init_key = setupKey(
        seed.c_str(),
        seed.length());
    std::unique_ptr<SafeDataPtr> tmp_key = setupKey(
        key,
        key_len,
        const_cast<const char*>(init_key->getData()), __key_size);

    int nr = rijndaelKeySetupEnc(
        key_data.get(),
        reinterpret_cast<const unsigned char*>(tmp_key->getData()),
        256);

    if (nr == 0) {
        return std::unique_ptr<DataFileObject>();
    }

    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memmove(iv, key, key_len>16?16:key_len);

    unsigned int cnt = len;
    unsigned int ocnt = olen;

    char *pos = dataptr;
    std::unique_ptr<char[]> tmp(new char[olen]);
    char *opos = tmp.get();
    unsigned int blocknum = 0;
    while (cnt > 0) {
        for (int i = 0; i < 16; i++) {
            if (cnt > 0) {
                in_block[i] = *pos ^ iv[i];
                pos++;
                cnt--;
            } else {
                in_block[i] = 0x0 ^ iv[i];
            }
        }

        rijndaelEncrypt(key_data.get(), nr, in_block, out_block);

        for (int i = 0; i < 16; i++) {
            if (ocnt > 0) {
                *opos = static_cast<char>(out_block[i]);
                opos++;
                ocnt--;
            }
        }
        memmove(iv, out_block, 16);
        blocknum++;
    }

    std::unique_ptr<DataFileObject> res(new DataFileObject(tmp.get(), olen));
    res->real_len = len;
    return res;
}

std::unique_ptr<blieng::DataFile::DataFileObject>
blieng::DataFile::DataFileObject::deobfuscate(
    const char *key,
    unsigned int key_len,
    const std::string &seed)
{
    std::unique_ptr<uint32_t[]> key_data(new uint32_t[(MAXNR+1)*4]);

    std::unique_ptr<SafeDataPtr> init_key = setupKey(
        seed.c_str(),
        seed.length());
    std::unique_ptr<SafeDataPtr> tmp_key = setupKey(
        key,
        key_len,
        const_cast<const char*>(init_key->getData()), __key_size);

    int nr = rijndaelKeySetupDec(
        key_data.get(),
        reinterpret_cast<const unsigned char*>(tmp_key->getData()),
        256);
    if (nr == 0) {
        return std::unique_ptr<DataFileObject>();
    }

    std::unique_ptr<char[]> tmp(new char[real_len]);
    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memmove(iv, key, key_len>16?16:key_len);

    unsigned int cnt = len;
    unsigned int ocnt = real_len;
    char *pos = dataptr;
    char *opos = tmp.get();
    while (cnt > 0) {
        for (int i = 0; i < 16; i++) {
            if (cnt > 0) {
                in_block[i] = static_cast<unsigned char>(*pos);
                pos++;
                cnt--;
            } else {
                in_block[i] = 0;
            }
        }
        rijndaelDecrypt(key_data.get(), nr, in_block, out_block);
        for (int i = 0; i < 16; i++) {
            if (ocnt > 0) {
                *opos = out_block[i] ^ static_cast<char>(iv[i]);
                opos++;
                ocnt--;
            }
        }
        memmove(iv, in_block, 16);
    }

    std::unique_ptr<DataFileObject> res(
        new DataFileObject(tmp.get(), real_len));
    res->real_len = real_len;
    return res;
}
