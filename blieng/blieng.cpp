#include "blieng/blieng.h"
#include "blieng/rijndael-alg-fst.h"

#include "fastlz.h"


blieng::BliengState::BliengState()
    : m_data(nullptr),
    m_config(nullptr),
    m_styleconfig(nullptr)
{
}

blieng::BliengState::~BliengState()
{
    if (m_config != nullptr) {
        delete m_config;
    }
    if (m_styleconfig != nullptr) {
        delete m_styleconfig;
    }
    if (m_data != nullptr) {
        delete m_data;
    }
}

void blieng::BliengState::initialize()
{
    m_data = new blieng::Data();

    boost::shared_ptr<blieng::BliengState> _state(this);

    m_config = new blieng::Configure(_state);
    m_styleconfig = new blieng::Configure(_state);
}

// FIXME Hardcoded
static unsigned int __key_size = 256 / 8;

std::unique_ptr<blieng::SafeDataPtr>
blieng::setupKey(
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

std::tuple<
    char *,
    unsigned int,
    unsigned int>
blieng::obfuscate(
    const char *dataptr,
    unsigned int len,
    const char *key,
    unsigned int key_len,
    const std::string &seed)
{
    unsigned int olen = len / 16;
    if (len % 16 != 0) olen++;
    olen *= 16;

    std::unique_ptr<uint32_t[]> key_data(new uint32_t[(MAXNR + 1) * 4]);

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
        return std::forward_as_tuple(nullptr, 0, 0);
    }

    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memmove(iv, key, (key_len > 16) ? 16 : key_len);

    unsigned int cnt = len;
    unsigned int ocnt = olen;

    const char *pos = dataptr;
/*
    std::unique_ptr<char[]> tmp(new char[olen]);
    char *opos = tmp.get();
*/
    char *tmp = new char[olen];
    char *opos = tmp;
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

    //std::unique_ptr<DataFileObject> res(new DataFileObject(tmp.get(), olen));
    //res->real_len = len;
    return std::forward_as_tuple(tmp, olen, len);
}

std::tuple<
    char *,
    unsigned int,
    unsigned int>
blieng::deobfuscate(
    const char *dataptr,
    unsigned int len,
    unsigned int real_len,
    const char *key,
    unsigned int key_len,
    const std::string &seed)
{
    std::unique_ptr<uint32_t[]> key_data(new uint32_t[(MAXNR + 1) * 4]);

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
        return std::forward_as_tuple(nullptr, 0, 0);
    }

    char *tmp = new char[real_len];
    unsigned char in_block[16];
    unsigned char out_block[16];
    unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memmove(iv, key, (key_len >16) ? 16 : key_len);

    unsigned int cnt = len;
    unsigned int ocnt = real_len;
    const char *pos = dataptr;
    char *opos = tmp;
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

#if 0
    std::unique_ptr<DataFileObject> res(
        new DataFileObject(tmp.get(), real_len));
    res->real_len = real_len;
    return res;
#endif
    return std::forward_as_tuple(tmp, real_len, real_len);
}

class Header
{
public:
    Header() : Header(0)
    {
    }
    Header(int len_) : len(len_)
    {
        id[0] = 'F';
        id[1] = 'L';
        id[2] = 'Z';
        id[3] = '1';
    }
    bool validate() const
    {
        return (id[0] == 'F'
            && id[1] == 'L'
            && id[2] == 'Z'
            && id[3] == '1'
            && len != 0);
    }

    char id[4];
    uint32_t len;
};

std::tuple<
    char *,
    unsigned int>
blieng::compress(
    const char *dataptr,
    unsigned int len)
{
    if (len < 16) {
        return std::forward_as_tuple(nullptr, 0);
    }

    char *res = new char[len * 110 / 100];
    int level = 2;
    int size = fastlz_compress_level(
        level,
        dataptr,
        len,
        res);
    if (size < 0) {
        delete[] res;
        return std::forward_as_tuple(nullptr, 0);
    }

    unsigned int res_len = size;
    Header hdr(len);

    unsigned int hdr_size = sizeof(hdr);
    char *real_res = new char[res_len + hdr_size];
    memcpy(real_res, &hdr, hdr_size);
    memcpy(real_res + hdr_size, res, res_len);

    delete[] res;
    return std::forward_as_tuple(real_res, res_len + hdr_size);
}

std::tuple<
    char *,
    unsigned int>
blieng::decompress(
    const char *dataptr,
    unsigned int len)
{
    const Header *hdr = nullptr;
    unsigned int hdr_size = sizeof(hdr);
    if (len < hdr_size) {
        return std::forward_as_tuple(nullptr, 0);
    }
    hdr = static_cast<const Header*>(static_cast<const void*>(dataptr));
    if (hdr == nullptr || !hdr->validate()) {
        return std::forward_as_tuple(nullptr, 0);
    }

    char *res = new char[hdr->len];

    int size = fastlz_decompress(
        dataptr + hdr_size,
        len,
        res,
        hdr->len);

    if (size < 0) {
        delete[] res;
        return std::forward_as_tuple(nullptr, 0);
    }

    return std::forward_as_tuple(res, hdr->len);
}

bool blieng::isCompressed(
    const char *dataptr,
    unsigned int len)
{
    const Header *hdr = nullptr;
    unsigned int hdr_size = sizeof(hdr);
    if (len < hdr_size) {
        return false;
    }
    hdr = static_cast<const Header*>(static_cast<const void*>(dataptr));
    if (hdr == nullptr || !hdr->validate()) {
        return false;
    }

    return true;
}
