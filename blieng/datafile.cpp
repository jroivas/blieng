/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/datafile.h"
#include "blieng/blieng.h"

#include <stdint.h>
#include <string.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <sys/time.h>

#include <cstdlib>
#include <string>
#include <vector>

using blieng::DataFile;

DataFile::DataFile()
{
    _ok = false;
}

DataFile::DataFile(
    const std::string &name)
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

std::unique_ptr<blieng::DataFile::DataFileObject>
blieng::DataFile::DataFileObject::obfuscate(
    const char *key,
    unsigned int key_len,
    const std::string &seed)
{
    char *tmp = nullptr;
    unsigned int res_len;
    unsigned int res_real_len;
    std::tie(
        tmp,
        res_len,
        res_real_len) = blieng::obfuscate(
            dataptr,
            len,
            key,
            key_len,
            seed);
    if (tmp == nullptr)
        return std::unique_ptr<DataFileObject>();

    std::unique_ptr<DataFileObject> res(new DataFileObject(tmp, res_len));
    res->real_len = res_real_len;
    delete []tmp;

    return res;
}

std::unique_ptr<blieng::DataFile::DataFileObject>
blieng::DataFile::DataFileObject::deobfuscate(
    const char *key,
    unsigned int key_len,
    const std::string &seed)
{
    char *tmp = nullptr;
    unsigned int res_len;
    unsigned int res_real_len;
    std::tie(
        tmp,
        res_len,
        res_real_len) = blieng::deobfuscate(
            dataptr,
            len,
            real_len,
            key,
            key_len,
            seed);
    if (tmp == nullptr)
        return std::unique_ptr<DataFileObject>();

    std::unique_ptr<DataFileObject> res(
        new DataFileObject(tmp, real_len));
    delete []tmp;

    res->real_len = real_len;

    return res;
}
