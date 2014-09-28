#ifndef __TEST_TOOLS_H
#define __TEST_TOOLS_H

#include <gmock/gmock.h>
#include "blieng/datafile.h"
#include "blieng/data.h"

class FakeDataFile : public blieng::DataFile
{
public:
    const DataFileObject *getObject(const std::string &name) const {
        auto item = _fake_data.find(name);
        if (item != _fake_data.end()) {
            return new DataFileObject(
                item->second.c_str(),
                item->second.length());
        }
        return blieng::DataFile::getObject(name);
    }

    void setFakeData(const std::string &_name, const std::string &_data) {
        _fake_data[_name] = _data;
        addData(_name, _data);
    }

private:
    std::map<std::string, std::string> _fake_data;
};

class DataMock : public blieng::Data
{
public:
    MOCK_CONST_METHOD1(readString, std::string(const std::string));
    MOCK_CONST_METHOD1(fileExists, bool(const std::string));

    void setFakeData(const std::string &_name, const std::string &_data) {
        FakeDataFile *tmp = nullptr;
        if (m_datafile == nullptr) {
            tmp = new FakeDataFile();
        } else {
            tmp = dynamic_cast<FakeDataFile*>(m_datafile);
        }
        tmp->setFakeData(_name, _data);

        m_datafile = tmp;
    }
private:
    FakeDataFile _fake_data_file;
};

#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stddef.h>

class cout_redirect
{
public:
    cout_redirect( std::streambuf * new_buffer ) : old( std::cout.rdbuf( new_buffer ) ) { }
    ~cout_redirect( ) { std::cout.rdbuf( old ); }
private:
    std::streambuf * old;
};

class cerr_redirect
{
public:
    cerr_redirect( std::streambuf * new_buffer ) : old( std::cerr.rdbuf( new_buffer ) ) { }
    ~cerr_redirect( ) { std::cerr.rdbuf( old ); }
private:
    std::streambuf * old;
};

void mock_io_start();
void mock_io_stop();
void mock_set_file(const std::string &name, const std::string &data);
void mock_remove_file(const std::string &name);
bool mock_is_file(const std::string &name);
void mock_add_folder(const std::string &name);
const std::string mock_get_data(const std::string &name);
std::vector<std::string> mock_list_files();

std::string mock_get_stdout();
std::string mock_get_stderr();
void mock_clear_stdout();
void mock_clear_stderr();

#ifdef __cplusplus
extern "C" {
#endif

FILE* fopen(const char *path, const char *mode);
FILE* fopen64(const char *path, const char *mode);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
int fgetpos(FILE *stream, fpos_t *pos);
int fflush(FILE *stream);
int ferror(FILE *stream) throw ();
void clearerr(FILE *stream) throw();
int fileno(FILE *stream) throw ();
int fsetpos(FILE *stream, const fpos_t *pos);
int feof(FILE *stream) throw ();
int fclose(FILE *fp);

int *__errno_location() __THROW __attribute__ ((__const__));

int open(const char *pathname, int flags, ...) __nonnull ((1));
int open64(const char *pathname, int flags, ...) __nonnull ((1));
int creat(const char *pathname, mode_t mode);
int creat64(const char *pathname, mode_t mode);
int stat(const char *path, struct stat *buf) throw ();
int stat64(const char *path, struct stat64 *buf) throw () __nonnull((1, 2));
int __xstat(int x, const char *path, struct stat *buf) throw ();
int __xstat64(int x, const char *path, struct stat64 *buf) throw () __nonnull((2, 3));
int __lxstat64(int x, const char *path, struct stat64 *buf) throw ();
int statvfs64(const char *path, struct statvfs64 *buf) throw ();
int fstatvfs64(int fd, struct statvfs64 *buf) throw ();
int fstat(int fd, struct stat *buf) throw () __nonnull((2));
int fstat64(int fd, struct stat64 *buf) throw () __nonnull((2));
int lstat(const char *path, struct stat *buf) throw ();
int close(int fd);

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
int ioctl(int d, unsigned long int request, ...) throw ();

int openat(int dirfd, const char *pathname, int flags, ...);

long fpathconf(int fd, int name) throw();
long pathconf(const char *path, int name) throw ();
DIR *opendir(const char *name);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result) __nonnull ((1, 2, 3));
int readdir64_r(DIR *dirp, struct dirent64 *entry, struct dirent64 **result) __nonnull ((1, 2, 3));
struct dirent *readdir(DIR *dirp);
struct dirent64 *readdir64(DIR *dirp);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp) __nonnull ((1));

int __fstat(int fd, struct stat *buf) throw ();
int __lxstat(int x, const char *path, struct stat *buf) throw ();
off_t lseek(int fd, off_t offset, int whence) throw();

#ifdef __cplusplus
}
#endif

#endif
