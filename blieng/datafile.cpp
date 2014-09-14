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
    : m_ok(false),
    m_compress(false)
{
}

DataFile::DataFile(
    const std::string &name)
    : m_ok(true),
    m_compress(false)
{
    setName(name);
}

DataFile::~DataFile()
{
    // Should we write?
}

void DataFile::setName(const std::string &name)
{
    m_ok = true;
    m_name = name;
}

std::string DataFile::unifyName(const std::string &name) const
{
    std::string tmp = "";

    bool got_letter = false;
    char back = 0;
    for (auto letter : name) {
        if (letter == '\n' ||
            letter == '\r' ||
            letter == 0 ||
            letter == '!' ||
            letter == '|') {
            // FIXME: Blacklist?
        } else {
            if (got_letter || (letter != '/' && letter != '\\')) {
                if (letter == '/' || letter == '\\')  {
                    back = letter;
                } else {
                    if (back != 0) tmp += back;
                    back = 0;
                    tmp += letter;
                }
                got_letter = true;
            }
        }
    }

    return tmp;
}

std::vector<std::string> blieng::DataFile::listFiles() const
{
    std::vector<std::string> res;

    for(auto val : m_data) {
        res.push_back(val->key);
    }

    return res;
}

const blieng::DataFile::DataFileObject *DataFile::getObject(
    const std::string &name)
{
    std::string uname = unifyName(name);
    if (uname == "")
        return nullptr;

    auto di = m_data.find(uname);
    if (di != m_data.end())
        return m_data[di];

    return nullptr;
}

unsigned int DataFile::getData(
    const std::string &name,
    const char **data)
{
    const DataFileObject *obj = getObject(name);
    if (obj != nullptr) {
        if (data != nullptr) {
            *data = obj->get();
        }
        return obj->length();
    }
    return 0;
}

bool DataFile::addData(
    const std::string &name,
    const std::string &data)
{
    std::string uname = unifyName(name);
    if (uname == "")
        return false;

    std::unique_ptr<DataFileObject> tmp(
        new DataFileObject(data.c_str(), data.size() + 1));
    m_data[uname] = std::move(tmp);

    return true;
}

bool DataFile::addData(
    const std::string &name,
    const char *data,
    unsigned int len)
{
    std::string uname = unifyName(name);
    if (uname == "")
        return false;

    std::unique_ptr<DataFileObject> tmp(new DataFileObject(data, len));
    m_data[uname] = std::move(tmp);

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
// TODO On Android we need to rely on Qt
// Might be good to find better way
#include <QtCore>
#endif

bool DataFile::read(
    const char *key,
    unsigned int key_len)
{
    if (!m_ok)
        return false;
    // TODO: Tidy

#ifdef ANDROID
    // FIXME Config
    QFile asset_file("assets:/data/data.dat");
    if (!asset_file.open(QIODevice::ReadOnly)) {
        return false;
    }
#else
    boost::filesystem::ifstream fd(
        m_name,
        std::ios_base::in | std::ofstream::binary);
    if (!fd.is_open())
        return false;
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
        if (nb_read == 0)
            break;
#else
        fd.read(
            reinterpret_cast<char*>(&namelen),
            sizeof(uint32_t));
        if (fd.eof())
            break;
#endif
        BOOST_ASSERT(namelen > 0);
        BOOST_ASSERT(namelen < 0x2ff);

        std::unique_ptr<char[]> ob_name(new char[namelen + 1]);
#ifdef ANDROID
        nb_read = asset_file.read(
            ob_name.get(),
            static_cast<int>(namelen));
        if (nb_read == 0)
            break;
#else
        fd.read(
            ob_name.get(),
            static_cast<int>(namelen));
        if (fd.eof())
            break;
#endif

        auto name = obfuscateSimple(ob_name.get(), namelen);

        uint32_t datalen = 0;
#ifdef ANDROID
        nb_read = asset_file.read(
            reinterpret_cast<char*>(&datalen),
            sizeof(uint32_t));
        if (nb_read == 0)
            break;
#else
        fd.read(
            reinterpret_cast<char*>(&datalen),
            sizeof(uint32_t));
        if (fd.eof())
            break;
#endif

        uint32_t datareallen = 0;
#ifdef ANDROID
        nb_read = asset_file.read(
            reinterpret_cast<char*>(&datareallen),
            sizeof(uint32_t));
        if (nb_read == 0)
            break;
#else
        fd.read(
            reinterpret_cast<char*>(&datareallen),
            sizeof(uint32_t));
        if (fd.eof())
            break;
#endif

        std::unique_ptr<char[]> data(new char[datalen]);
#ifdef ANDROID
        nb_read = asset_file.read(
            data.get(),
            static_cast<int>(datalen));
        if (nb_read == 0)
            break;
#else
        fd.read(
            data.get(),
            static_cast<int>(datalen));
        if (fd.eof())
            break;
#endif

        std::unique_ptr<DataFileObject> tmp(
            new DataFileObject(data.get(), datalen));
        tmp->m_real_len = datareallen;

        std::string sname;
        sname.append(name->getData(), name->length());

        if (key != nullptr && key_len > 0) {
            std::unique_ptr<DataFileObject> new_tmp(
                tmp->deobfuscate(key, key_len, sname));
            if (new_tmp.get()) {
                tmp = std::move(new_tmp);
            }
        }
        m_data[sname] = std::move(tmp);
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
    if (!m_ok)
        return false;
#ifdef ANDROID
    return false;
#endif

    boost::filesystem::ofstream fd(
        m_name,
        std::ios_base::out | std::ios_base::trunc | std::ofstream::binary);
    if (!fd.is_open())
        return false;

    auto di = m_data.begin();
    while (di != m_data.end()) {
        DataFileObject* tmp = m_data[di];
        if (m_compress) {
            std::unique_ptr<DataFileObject> new_tmp = tmp->compress();
            if (new_tmp.get()
                && new_tmp.get()->isValid()
                && new_tmp.get()->m_len <= new_tmp.get()->m_real_len) {
                m_data[di] = std::move(new_tmp);
                tmp = m_data[di];
                //std::cout << "Got size: " << tmp->m_len * 100 / tmp->m_real_len << "% " << tmp->m_real_len << "\n";
            }
        }
        if (key != nullptr && key_len > 0) {
            std::unique_ptr<DataFileObject> new_tmp = tmp->obfuscate(
                key,
                key_len,
                (*di)->key);
            if (new_tmp.get()) {
                m_data[di] = std::move(new_tmp);
                tmp = m_data[di];
            }
        }

        uint32_t itmp = (*di)->key.length();
        fd.write(reinterpret_cast<char*>(&itmp), sizeof(uint32_t));

        auto ob_name = obfuscateSimple((*di)->key.c_str(), itmp);
        fd.write(ob_name->getData(), static_cast<int>(ob_name->length()));

        itmp = tmp->m_len;
        fd.write(reinterpret_cast<char*>(&itmp), sizeof(uint32_t));

        itmp = tmp->m_real_len;
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
    unsigned int new_len)
    : m_len(new_len),
    m_real_len(new_len)
{
    if (blieng::isCompressed(new_data, new_len)) {
        std::tie(
            m_dataptr,
            m_len) = blieng::decompress(
                new_data,
                new_len);
        if (m_dataptr == nullptr) {
            std::cout << "ERROR: Invalid decompress\n";
        }
        m_real_len = m_len;
    } else {
        m_dataptr = new char[m_len];
        memmove(m_dataptr, new_data, m_len);
    }
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
            m_dataptr,
            m_len,
            key,
            key_len,
            seed);
    if (tmp == nullptr)
        return std::unique_ptr<DataFileObject>();

    std::unique_ptr<DataFileObject> res(
        new DataFileObject(tmp, res_len));
    res->m_real_len = res_real_len;
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
            m_dataptr,
            m_len,
            m_real_len,
            key,
            key_len,
            seed);
    if (tmp == nullptr)
        return std::unique_ptr<DataFileObject>();

    std::unique_ptr<DataFileObject> res(
        new DataFileObject(tmp, m_real_len));
    delete []tmp;

    res->m_real_len = m_real_len;

    return res;
}

std::unique_ptr<blieng::DataFile::DataFileObject>
blieng::DataFile::DataFileObject::compress()
{
    std::unique_ptr<DataFileObject> res(
        new DataFileObject());
    std::tie(
        res->m_dataptr,
        res->m_len) = blieng::compress(
            m_dataptr,
            m_len);
    res->m_real_len = m_real_len;
    return res;
}
