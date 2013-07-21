#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include "blieng/datafile.h"

int main(int argc, char **argv)
{
#if 1
    unsigned char _mul_array[] = {
        0xF2, 0x0F, 0x11, 0x40,
        0x28, 0x77, 0x05, 0x83,
        0xC6, 0x01, 0x31, 0xD2,
        0x48, 0x83, 0xC1, 0x08,
        0x49, 0x39, 0xC8, 0x75,
        0xAE, 0xF3, 0xC3 };
    unsigned int _mul_array_len = 23;
#else
    unsigned char *_mul_array = NULL;
    unsigned int _mul_array_len = 0;
#endif
    
    int mode = 0;
    if (basename(argv[0]) == std::string("datafile-extractor") ||
        basename(argv[0]) == std::string("datafile-extractor.exe")) {
        mode = 1;
        if (argc < 3) {
            std::cout << "Usage: " << argv[0] << " datafile.dat file/to/read\n";
            return 1;
        }
    } else {
        if (argc < 3) {
            std::cout << "Usage: " << argv[0] << " datafile.dat /real/file/to/add new/file/name\n";
            return 1;
        }
    }

    blieng::DataFile datafile(argv[1]);
    if (!datafile.read((const char*)_mul_array, _mul_array_len)) {
        std::cerr << "WARNING: Can't read data file!\n";
        if (mode == 1) return 1;
    }

    if (mode == 0) {
        boost::filesystem::path file_path = argv[2];
        if (!boost::filesystem::exists(file_path)) {
            std::cerr << "ERROR: File does not exists: " << argv[2] <<"\n";
        }

        boost::filesystem::ifstream fd(file_path);
        #define BUFSIZE 1024
        char *buffer = (char*)malloc(BUFSIZE);
        char *tmp = buffer;
        unsigned int totalsize = 0;
        while (!fd.eof() && fd.good()) {
            if (fd.rdbuf()->in_avail() <= 0) break;
            int cnt = fd.readsome(tmp, BUFSIZE);
            if (cnt > 0) {
                totalsize += cnt;
                buffer = (char*)realloc(buffer, totalsize + BUFSIZE);
                tmp = buffer + totalsize;
            }
        }
        #undef BUFSIZE
        fd.close();

        if (totalsize > 0) {
            std::cout << "Adding " << argv[3] << " (input file " << argv[2] << ", " << totalsize << " bytes)\n";
            datafile.addData(argv[3], buffer, totalsize);
        }
        free(buffer);

        if (!datafile.write((const char*)_mul_array, _mul_array_len)) {
            std::cerr << "Can't write data file!\n";
            return 1;
        }
    } else if (mode == 1) {
        blieng::DataFile::DataFileObject *obj = datafile.getObject(argv[2]);
        if (obj == NULL) {
            std::cerr << "ERROR: File not found: " << argv[2] << "\n";;
            return 1;
        }
        std::cout.write(obj->data, obj->len);
    }

    return 0;
}
