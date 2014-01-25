#ifndef __TEST_TOOLS_H
#define __TEST_TOOLS_H

#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>

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
void mock_set_file(std::string name, std::string data);
void mock_remove_file(std::string name);
bool mock_is_file(std::string name);
void mock_add_folder(std::string name);
std::string mock_get_data(std::string name);
std::vector<std::string> mock_list_files();

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
void clearerr(FILE *stream);
int fileno(FILE *stream) throw ();
int fsetpos(FILE *stream, const fpos_t *pos);
int feof(FILE *stream) throw ();
int fclose(FILE *fp);

int *__errno_location();

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
//off_t lseek(int fd, off_t offset, int whence) throw ();

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
int ioctl(int d, unsigned long int request, ...) throw ();

int openat(int dirfd, const char *pathname, int flags, ...);

#ifdef __cplusplus
}
#endif

#endif
