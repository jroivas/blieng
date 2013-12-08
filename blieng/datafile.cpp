#include "datafile.h"
#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <sys/time.h>

using blieng::DataFile;

static unsigned int __key_size = 256/8;
typedef auto_map<std::string,blieng::DataFile::DataFileObject>::iterator_value datafile_item_t;

class blieng::SafeDataPtr
{
public:
    SafeDataPtr(char _data[], unsigned int _len) : len(_len) {
        data = new char[len];
        memmove(data, _data, len);
    }
    SafeDataPtr(unsigned char _data[], unsigned int _len) : len(_len) {
        data = new char[len];
        memmove(data, reinterpret_cast<char*>(_data), len);
    }
    virtual ~SafeDataPtr() {
        if (data != nullptr) delete [] data;
        data = nullptr;
        len = 0;
    }

    const char * getData() const { return data; }
    unsigned int length() const { return len; }
    unsigned int size() const { return len; }

private:
    char *data;
    unsigned int len;
};


DataFile::DataFile()
{
    _ok = false;
}

DataFile::DataFile(std::string name)
{
    setName(name);
}

DataFile::~DataFile()
{
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
    auto ni = name.cbegin();
    char back = 0;
    while (ni != name.cend()) {
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

const blieng::DataFile::DataFileObject *DataFile::getObject(std::string name)
{
    name = unifyName(name);
    if (name == "") return nullptr;

    auto di = _data.find(name);
    if (di != _data.end()) return _data[di];

    return nullptr;
}

unsigned int DataFile::getData(std::string name, const char **data)
{
    const DataFileObject *obj = getObject(name);
    if (obj != nullptr) {
        if (data != nullptr) *data = obj->data;
        return obj->len;
    }
    return 0;
}

bool DataFile::addData(std::string name, std::string data)
{
    name = unifyName(name);
    if (name == "") return false;

    std::unique_ptr<DataFileObject> tmp(new DataFileObject(data.c_str(), data.size() + 1));
    _data[name] = std::move(tmp);

    return true;
}

bool DataFile::addData(std::string name, char *data, unsigned int len)
{
    name = unifyName(name);
    if (name == "") return false;

    std::unique_ptr<DataFileObject> tmp(new DataFileObject(data, len));
    _data[name] = std::move(tmp);

    return true;
}

std::unique_ptr<blieng::SafeDataPtr> DataFile::obfuscateSimple(const char *data, unsigned int len)
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
    return std::unique_ptr<blieng::SafeDataPtr>(new blieng::SafeDataPtr(res.get(), orig_len));
}

#ifdef Q_OS_ANDROID
#include <QtCore>
#endif
bool DataFile::read(const char *key, unsigned int key_len)
{
    if (!_ok) return false;

#ifdef Q_OS_ANDROID
    QFile asset_file("assets:/data/data.dat");
    if (!asset_file.open(QIODevice::ReadOnly)) {
        return false;
    }
#else
    boost::filesystem::ifstream fd(_name, std::ios_base::in | std::ofstream::binary);
    if (!fd.is_open()) return false;
#endif

#ifdef Q_OS_ANDROID
    int nb_read = 0;
    while (!asset_file.atEnd()) {
#else
    while (!fd.eof()) {
#endif
        uint32_t namelen = 0;
#ifdef Q_OS_ANDROID
        nb_read = asset_file.read(reinterpret_cast<char*>(&namelen), sizeof(uint32_t));
        if (nb_read == 0) break;
#else
        fd.read(reinterpret_cast<char*>(&namelen), sizeof(uint32_t));
        if (fd.eof()) break;
#endif
        BOOST_ASSERT(namelen > 0);
        BOOST_ASSERT(namelen < 0x2ff);

        std::unique_ptr<char[]> ob_name(new char[namelen+1]);
#ifdef Q_OS_ANDROID
        nb_read = asset_file.read(ob_name.get(), static_cast<int>(namelen));
        if (nb_read == 0) break;
#else
        fd.read(ob_name.get(), static_cast<int>(namelen));
        if (fd.eof()) break;
#endif

        auto name = obfuscateSimple(ob_name.get(), namelen);

        //ob_name = nullptr;

        uint32_t datalen = 0;
#ifdef Q_OS_ANDROID
        nb_read = asset_file.read(reinterpret_cast<char*>(&datalen), sizeof(uint32_t));
        if (nb_read == 0) break;
#else
        fd.read(reinterpret_cast<char*>(&datalen), sizeof(uint32_t));
        if (fd.eof()) break;
#endif

        uint32_t datareallen = 0;
#ifdef Q_OS_ANDROID
        nb_read = asset_file.read(reinterpret_cast<char*>(&datareallen), sizeof(uint32_t));
        if (nb_read == 0) break;
#else
        fd.read(reinterpret_cast<char*>(&datareallen), sizeof(uint32_t));
        if (fd.eof()) break;
#endif

        std::unique_ptr<char[]> data(new char[datalen]);
#ifdef Q_OS_ANDROID
        nb_read = asset_file.read(data.get(), static_cast<int>(datalen));
        if (nb_read == 0) break;
#else
        fd.read(data.get(), static_cast<int>(datalen));
        if (fd.eof()) break;
#endif

        std::unique_ptr<DataFileObject> tmp(new DataFileObject(data.get(), datalen));
        tmp->real_len = datareallen;

        std::string sname;
        sname.append(name->getData(), name->length());

        if (key != nullptr && key_len > 0) {
            std::unique_ptr<DataFileObject> new_tmp(tmp->deobfuscate(key, key_len, sname));
            if (new_tmp.get()) {
                tmp = std::move(new_tmp);
            }
        }
        _data[sname] = std::move(tmp);
    }

#ifdef Q_OS_ANDROID
    asset_file.close();
#else
    fd.close();
#endif

    return true;
}

bool DataFile::write(const char *key, unsigned int key_len)
{
    if (!_ok) return false;
#ifdef Q_OS_ANDROID
    return false;
#endif

    boost::filesystem::ofstream fd(_name, std::ios_base::out | std::ios_base::trunc | std::ofstream::binary);
    if (!fd.is_open()) return false;

    auto di = _data.cbegin();
    while (di != _data.cend()) {
        DataFileObject* tmp = _data[di];
        if (key != nullptr && key_len > 0) {
            std::unique_ptr<DataFileObject> new_tmp = tmp->obfuscate(key, key_len, (*di)->key);
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

        fd.write(tmp->data, static_cast<int>(tmp->len));
        fd.flush();
        ++di;
    }

    fd.close();

    return true;
}


blieng::DataFile::DataFileObject::DataFileObject(const char *new_data, unsigned int new_len) : len(new_len), real_len(new_len)
{
    data = new char[len];
    memmove(data, new_data, len);
}

std::unique_ptr<blieng::SafeDataPtr> blieng::DataFile::DataFileObject::setupKey(const char *key, unsigned int key_len, const char *iv, unsigned int iv_len)
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

#include "rijndael-alg-fst.h"
std::unique_ptr<blieng::DataFile::DataFileObject> blieng::DataFile::DataFileObject::obfuscate(const char *key, unsigned int key_len, std::string seed)
{
    unsigned int olen = len / 16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    std::unique_ptr<uint32_t[]> key_data(new uint32_t[(MAXNR+1)*4]);

    std::unique_ptr<SafeDataPtr> init_key = setupKey(seed.c_str(), seed.length());
    std::unique_ptr<SafeDataPtr> tmp_key = setupKey(key, key_len, const_cast<const char*>(init_key->getData()), __key_size);

    int nr = rijndaelKeySetupEnc(key_data.get(), reinterpret_cast<const unsigned char*>(tmp_key->getData()), 256);
    if (nr == 0) {
        return std::unique_ptr<DataFileObject>();
    }

    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memmove(iv, key, key_len>16?16:key_len);

    unsigned int cnt = len;
    unsigned int ocnt = olen;

    char *pos = data;
    std::unique_ptr<char[]> tmp(new char[olen]);
    char *opos = tmp.get();
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

std::unique_ptr<blieng::DataFile::DataFileObject> blieng::DataFile::DataFileObject::deobfuscate(const char *key, unsigned int key_len, std::string seed)
{
    std::unique_ptr<uint32_t[]> key_data(new uint32_t[(MAXNR+1)*4]);

    std::unique_ptr<SafeDataPtr> init_key = setupKey(seed.c_str(), seed.length());
    std::unique_ptr<SafeDataPtr> tmp_key = setupKey(key, key_len, const_cast<const char*>(init_key->getData()), __key_size);

    int nr = rijndaelKeySetupDec(key_data.get(), reinterpret_cast<const unsigned char*>(tmp_key->getData()), 256);
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
    char *pos = data;
    char *opos = tmp.get();
    while (cnt > 0) {
        for (int i=0; i < 16; i++) {
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

    std::unique_ptr<DataFileObject> res(new DataFileObject(tmp.get(), real_len));
    res->real_len = real_len;
    return res;
}
