#include "test_tools.h"
#include <map>
#include <iostream>
#include <cstdio>
#include <dlfcn.h>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <errno.h>
#include <error.h>

/*
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
static boost::mutex __mutex;
static boost::mutex __data_mutex;
static boost::mutex __fold_mutex;
static boost::mutex __mockid_mutex;
*/

static int __real__errno = 0;

int *__errno_location() __THROW
{
    return &__real__errno;
}

class MockFile
{
public:
    MockFile(const std::string &name) : magic(0x420012), name(name), open(true), mode(""), pos(0), eof(false), error(0) {}
    MockFile(const std::string &name, const std::string &mode) : magic(0x420012), name(name), open(true), mode(mode), pos(0), eof(false), error(0) {}

    bool readonly() const
    {
        return (mode == "r");
    }

    bool writeable() const
    {
        return (mode == "r+" || mode == "w" || mode == "rw" || mode == "w+" || mode == "a" || mode == "a+");
    }

    unsigned int magic;
    std::string name;
    bool open;
    std::string mode;
    size_t pos;
    bool eof;
    int error;
};

class MockDir
{
public:
    MockDir(const std::string &path) : magic(0x420015), name(path), entry(0) {}

    unsigned int magic;
    std::string name;
    unsigned int entry;
};

#define VALIDATE(X) BOOST_ASSERT(X != NULL); BOOST_ASSERT(reinterpret_cast<MockFile*>(X)->magic == 0x420012);
#define VALIDATE_DIR(X) BOOST_ASSERT(X != NULL); BOOST_ASSERT(reinterpret_cast<MockDir*>(X)->magic == 0x420015);
#define MF(X) reinterpret_cast<MockFile*>(X)
#define MD(X) reinterpret_cast<MockDir*>(X)

static bool __mocking_io = false;
static std::map<std::string, std::string> mock_files;
static std::vector<MockFile*> mock_ids;
static std::vector<std::string> mock_folders;

void mock_io_start()
{
    if (!mock_is_file("__stdin")) mock_set_file("__stdin", "");
    if (!mock_is_file("__stdout")) mock_set_file("__stdout", "");
    if (!mock_is_file("__stderr")) mock_set_file("__stderr", "");
    if (!mock_is_file("/dev/urandom")) mock_set_file("/dev/urandom", "");
    __mocking_io = true;
}

void mock_io_stop()
{
    __mocking_io = false;

    std::string a = mock_get_data("__stdout");
    if (!a.empty()) std::cout << a << std::endl;
    a = mock_get_data("__stderr");
    if (!a.empty()) std::cerr << a << std::endl;
}

void mock_set_file(const std::string &name, const std::string &data)
{
    //boost::lock_guard<boost::mutex> keylock(__data_mutex);
    //mock_files[name] = data.c_str();
    std::string tmp_copy(data.data(), data.size());

    mock_files[name] = tmp_copy;
}

bool mock_is_folder(const std::string &name)
{
    //boost::lock_guard<boost::mutex> keylock(__fold_mutex);
    BOOST_FOREACH(std::string f, mock_folders) {
        if (f == name) return true;
    }
    return false;
}

void mock_add_folder(const std::string &name)
{
    //boost::lock_guard<boost::mutex> keylock(__fold_mutex);
    mock_folders.push_back(name);
    mock_folders.push_back(name+"/.");
}

void mock_remove_file(const std::string &name)
{
    //boost::lock_guard<boost::mutex> keylock(__data_mutex);
    std::map<std::string, std::string>::iterator it = mock_files.find(name);
    if (it != mock_files.end()) {
        mock_files.erase(it);
    }
}

bool mock_is_file(const std::string &name)
{
    //boost::lock_guard<boost::mutex> keylock(__data_mutex);
    return (mock_files.find(name) != mock_files.end());
}

const std::string mock_get_data(const std::string &name)
{
    BOOST_ASSERT( mock_is_file(name) );
    //boost::lock_guard<boost::mutex> keylock(__data_mutex);
    return mock_files[name];
}

std::vector<std::string> mock_list_files()
{
    //boost::lock_guard<boost::mutex> keylock(__data_mutex);
    std::vector<std::string> res;
    std::map<std::string, std::string>::iterator it = mock_files.begin();
    while (it != mock_files.begin()) {
        res.push_back(it->first);
        ++it;
    }
    return res;
}

FILE* (*orig_fopen)(const char *path, const char *mode) = NULL;
int (*orig_open)(const char *path, int flags, ...) = NULL;
int (*orig_openat)(int dirfd, const char *pathname, int flags, ...) = NULL;
long (*orig_pathconf)(char*,int) = NULL;
long (*orig_fpathconf)(int fd, int name) = NULL;
DIR* (*orig_opendir)(const char *name) = NULL;
struct dirent *(*orig_readdir)(DIR *dirp) = NULL;
int (*orig_readdir_r)(DIR *dirp, struct dirent64 *entry, struct dirent64 **result) = NULL;
int (*orig_closedir)(DIR *dirp) = NULL;
int (*orig_close)(int fd) = NULL;
int (*orig_stat)(const char *path, struct stat *buf) = NULL;
int (*orig_xstat)(int x, const char *path, struct stat *buf) = NULL;
int (*orig_xstat64)(int x, const char *path, struct stat64 *buf) = NULL;
int (*orig_fstat)(int fd, struct stat *buf) = NULL;
int (*orig_fstat64)(int fd, struct stat64 *buf) = NULL;
int (*orig_lstat)(const char *path, struct stat *buf) = NULL;
int (*orig_fflush)(FILE *stream) = NULL;
int (*orig_ferror)(FILE *stream) = NULL;
void (*orig_clearerr)(FILE *stream) = NULL;
ssize_t (*orig_read)(int fd, void *buf, size_t count) = NULL;
ssize_t (*orig_write)(int fd, const void *buf, size_t count) = NULL;
size_t (*orig_fread)(void *ptr, size_t size, size_t nmemb, FILE *stream) = NULL;
size_t (*orig_fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream) = NULL;
int (*orig_fseek)(FILE *stream, long offset, int whence) = NULL;
int (*orig_lseek)(int, off_t, int) = NULL;
long (*orig_ftell)(FILE *stream) = NULL;
int (*orig_feof)(FILE *stream) = NULL;
int (*orig_fileno)(FILE *stream) = NULL;
void (*orig_rewind)(FILE *stream) = NULL;
int (*orig_fgetpos)(FILE *stream, fpos_t *pos) = NULL;
int (*orig_fsetpos)(FILE *stream, const fpos_t *pos) = NULL;
int (*orig_fclose)(FILE *fp) = NULL;
int (*orig_poll)(struct pollfd *, nfds_t, int) = NULL;
int (*orig_ioctl)(int, int, ...) = NULL;

void prepare_std()
{
    if (mock_ids.size() >= 3) return;

    mock_ids.push_back(new MockFile("__stdin", "r"));
    mock_ids.push_back(new MockFile("__stdout", "w"));
    mock_ids.push_back(new MockFile("__stderr", "w"));
    mock_ids.push_back(new MockFile("/dev/urandom", "r"));

    if (!mock_is_file("__stdin")) mock_set_file("__stdin", "");
    if (!mock_is_file("__stdout")) mock_set_file("__stdout", "");
    if (!mock_is_file("__stderr")) mock_set_file("__stderr", "");
    if (!mock_is_file("/dev/urandom")) mock_set_file("/dev/urandom", "");
}

int real_open(const char *pathname, int flags, mode_t mode)
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        prepare_std();

        std::string amode = "";

        if (flags & O_RDONLY) amode = "r";
        else if (flags & O_TRUNC) amode = "w+";
        //else if (flags & O_WRONLY) amode = "w";
        else {
            amode = "w";
        }

        if (!mock_is_file(pathname)) {
            if (!((flags & O_TRUNC) || (flags & O_CREAT))) {
                errno = ENOENT;
                return -1;
            }
        }

        MockFile *f = new MockFile(pathname, amode);
        mock_ids.push_back(f);

        if (!mock_is_file(pathname)) {
            mock_set_file(pathname, "");
        }

        return mock_ids.size() - 1;
    }
    if (!orig_open) orig_open = *(int (*)(const char *path, int flags, ...))dlsym(RTLD_NEXT, "open");
    return orig_open(pathname, flags, mode);
}

int open64(const char *pathname, int flags, ...)
{
    return real_open(pathname, flags, 0644);
}

int open(const char *pathname, int flags, ...)
{
    return real_open(pathname, flags, 0644);
}

/*
int open64(const char *pathname, int flags)
{
    return real_open(pathname, flags, 0644);
}
*/

int creat(const char *pathname, mode_t mode)
{
    return real_open(pathname, O_CREAT|O_WRONLY|O_TRUNC, mode);
}

int creat64(const char *pathname, mode_t mode)
{
    return real_open(pathname, O_CREAT|O_WRONLY|O_TRUNC, mode);
}

int close(int fd)
{
    if (__mocking_io) {
        if (fd < 3) return 0;

    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (mock_ids.size() <= (unsigned int)fd) return -1;

        mock_ids[fd]->open = false;
        delete mock_ids[fd];
        return 0;
    }

    if (!orig_close) orig_close = *(int (*)(int))dlsym(RTLD_NEXT, "close");
    return orig_close(fd);
}

int fstat(int fd, struct stat *buf) throw ()
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (fd < 3 || fd >= (int)mock_ids.size()) return 0;

        return __xstat(0, mock_ids[fd]->name.c_str(), (struct stat*)buf);
    }
    if (!orig_fstat) orig_fstat = *(int (*)(int, struct stat *))dlsym(RTLD_NEXT, "fstat");
    return orig_fstat(fd, buf);
}
int __fstat(int fd, struct stat *buf) throw ()
{
    return fstat(fd, buf);
}

int fstat64(int fd, struct stat64 *buf) throw ()
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (fd < 3 || fd >= (int)mock_ids.size()) return 0;

        return __xstat64(0, mock_ids[fd]->name.c_str(), buf);
    }
    if (!orig_fstat64) orig_fstat64 = *(int (*)(int, struct stat64 *))dlsym(RTLD_NEXT, "fstat64");
    return orig_fstat64(fd, buf);
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (nfds > 0) {
            unsigned int nn = 0;
            for (size_t a = 0; a < nfds; a++) {
                bool got = false;
                int fd = fds[a].fd;
                fds[a].revents = 0;
                if (fd < 0) {
                }
                else if (fd < 3) {
                    if (fd == 0 && (fds[a].events & POLLIN)) {
                        std::string data = mock_get_data("__stdin");

                        fds[a].revents = 0;
                        if (!data.empty()) {
                            fds[a].revents |= POLLIN;
                            got = true;
                        }
                    }
                    if ((fd == 1 || fd == 2) && (fds[a].events & POLLOUT)) {
                        fds[a].revents = POLLOUT;
                        got = true;
                    }
                }
                else if (fd >= (int)mock_ids.size()) {
                    fds[a].revents = POLLNVAL;
                    got = true;
                }
                else {
                    MockFile *f = mock_ids[fd];
                    std::string data = mock_get_data(f->name);
                    if ((fds[a].events & POLLIN) && (f->pos+1 < data.size())) {
                        fds[a].revents |= POLLIN;
                        got = true;
                    }
                    if ((fds[a].events & POLLOUT) && f->writeable()) {
                        fds[a].revents |= POLLOUT;
                        got = true;
                    }
                    if (fds[a].events != 0) {
                    }
                }
                if (got) ++nn;
            }
            return nn;
        }
        return -1;
    }
    if (!orig_poll) orig_poll = *(int (*)(struct pollfd *, nfds_t, int))dlsym(RTLD_NEXT, "poll");
    return orig_poll(fds, nfds, timeout);
}

int ioctl(int fd, unsigned long int request, ...) throw ()
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (fd < 3) return 0;
        else if (fd >= (int)mock_ids.size()) {
            errno = EBADF;
            return -1;
        }
        if (request == FIONREAD) {
            MockFile *f = mock_ids[fd];
            std::string data = mock_get_data(f->name);
            int cnt = data.size() - f->pos;

            va_list vl;
            va_start(vl, request);
            int* val = va_arg(vl, int*);
            if (val != nullptr) {
                *val = cnt;
            }
            va_end(vl);
            return 0;
        }
        return 0;
    }
    if (!orig_ioctl) orig_ioctl = *(int (*)(int, int, ...))dlsym(RTLD_NEXT, "ioctl");
    return orig_ioctl(fd, request);
}

int stat(const char *path, struct stat *buf) throw ()
{
    return __xstat(0, path, buf);
}

int stat64(const char *path, struct stat64 *buf) throw ()
{
    return __xstat64(0, path, buf);
}

int __xstat(int x, const char *path, struct stat *buf) throw ()
{
   return __xstat64(x, path, (struct stat64 *)buf);
}

int __xstat64(int x, const char *path, struct stat64 *buf) throw ()
{
    if (__mocking_io) {
	//boost::lock_guard<boost::mutex> keylock(__mutex);
        if (mock_is_folder(path)) {
            if (buf) {
                buf->st_dev = 1;
                buf->st_ino = 800 + strlen(path);
                buf->st_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
                buf->st_nlink = 1;
                buf->st_uid = 0;
                buf->st_gid = 0;
                buf->st_rdev = 0;
                buf->st_size = 4096;
                buf->st_blksize = 512;
                buf->st_blocks = 4;
            }
            return 0;
        }
        else if (mock_is_file(path)) {
            if (buf) {
                buf->st_dev = 1;
                buf->st_ino = 1000 + strlen(path);
                buf->st_mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
                buf->st_nlink = 1;
                buf->st_uid = 0;
                buf->st_gid = 0;
                buf->st_rdev = 0;
                const std::string file_data = mock_get_data(path);
                buf->st_size = file_data.length();
                buf->st_blksize = 512;
                buf->st_blocks = buf->st_size / 512 + 1;
            }
            return 0;
        }
        errno = ENOENT;
        return -1;
    }
    if (!orig_xstat64) orig_xstat64 = *(int (*)(int, const char *, struct stat64 *))dlsym(RTLD_NEXT, "__xstat64");
    return orig_xstat64(x, path, buf);
}

//TODO Implement these as well
#if 0
#include <sys/types.h>
#include <dirent.h>

int rmdir(const char *path)
{
    errno = ENOENT;
    return -1;
}
int mkdir(const char *path, mode_t mode)
{
    errno = ENOENT;
    return -1;
}

int symlink(const char *oldpath, const char *newpath)
{
    errno = ENOENT;
    return -1;
}
int truncate(const char *path, off_t length)
{
    errno = ENOENT;
    return -1;
}
int truncate64(const char *path, off_t length)
{
    errno = ENOENT;
    return -1;
}
int unlink(const char *pathname)
{
    errno = ENOENT;
    return -1;
}
int rename(const char *oldpath, const char *newpath)
{
    errno = ENOENT;
    return -1;
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz)
{
    errno = ENOENT;
    return NULL;
}
int chdir(const char *path)
{
    errno = ENOENT;
    return -1;
}
int fchdir(int fd)
{
    errno = ENOENT;
    return -1;
}
int chmod(const char *path, mode_t mode)
{
    errno = ENOENT;
    return -1;
}
int fchmod(int path, mode_t mode)
{
    errno = ENOENT;
    return -1;
}

char *getcwd(char *buf, size_t size)
{
    if (size < 2 || buf == NULL) return NULL;
    buf[0] = '/';
    buf[1] = 0;
    return buf;
}

int link(const char *oldpath, const char *newpath)
{
    return 0;
}

DIR *fdopendir(int fd)
{
    errno = ENOENT;
    return NULL;
}

int fstatat (int fd, const char *file, struct stat *buf, int flag)
{
    errno = ENOENT;
    return -1;
}
int __fstatat (int fd, const char *file, struct stat *buf, int flag)
{
    errno = ENOENT;
    return -1;
}
int fstatat64 (int fd, const char *file, struct stat *buf, int flag)
{
    errno = ENOENT;
    return -1;
}
int __fstatat64 (int fd, const char *file, struct stat *buf, int flag)
{
    errno = ENOENT;
    return -1;
}

int fstatvfs(const char *path, struct statvfs *buf)
{
    errno = ENOENT;
    return -1;
}
int statvfs(const char *path, struct statvfs *buf)
{
    errno = ENOENT;
    return -1;
}
int __fstatfs(int path, struct statvfs *buf)
{
    errno = ENOENT;
    return -1;
}
int __fstatfs64(int path, struct statvfs64 *buf)
{
    errno = ENOENT;
    return -1;
}
int fstatvfs(int path, struct statvfs *buf)
{
    errno = ENOENT;
    return -1;
}

int __fxstatat (int fd, const char *file, struct stat *buf, int flag)
{
    errno = ENOENT;
    return -1;
}
int __fxstatat64 (int fd, const char *file, struct stat *buf, int flag)
{
    errno = ENOENT;
    return -1;
}

int statvfs64(const char *path, struct statvfs64 *buf) throw ()
{
    return -1;
}

int fstatvfs64(int fd, struct statvfs64 *buf) throw ()
{
    return -1;
}
#endif

int __lxstat64(int x, const char *path, struct stat64 *buf) throw ()
{
    return __xstat64(x, path, buf);
}

int __lxstat(int x, const char *path, struct stat *buf) throw ()
{
    return __xstat(x, path, buf);
}


int lstat(const char *path, struct stat *buf) throw ()
{
    if (__mocking_io) {
        return stat(path, buf);
    }
    if (!orig_lstat) orig_lstat = *(int (*)(const char *, struct stat *))dlsym(RTLD_NEXT, "lstat");
    return orig_lstat(path, buf);
}

ssize_t read(int fd, void *buf, size_t count)
{
    if (__mocking_io) {
        prepare_std();
        //if (fd < 3) return 0;

        if (mock_ids.size() <= (unsigned int)fd) return -1;

        return fread(buf, count, 1, (FILE*)mock_ids[fd]);
    }
    if (!orig_read) orig_read = *(ssize_t (*)(int, void*, size_t))dlsym(RTLD_NEXT, "read");
    return orig_read(fd, buf, count);
}

ssize_t write(int fd, const void *buf, size_t count)
{
    if (__mocking_io) {
        prepare_std();
        //if (fd < 3) return 0;

        if (mock_ids.size() <= (unsigned int)fd) return -1;

        return fwrite(buf, count, 1, (FILE*)mock_ids[fd]);
    }
    if (!orig_write) orig_write = *(ssize_t (*)(int, const void*, size_t))dlsym(RTLD_NEXT, "write");
    return orig_write(fd, buf, count);
}

FILE* fopen(const char *path, const char *mode)
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        std::string smode = mode;
        if ((smode == "r" || smode == "r+") && !mock_is_file(path)) {
            errno = ENOENT;
            return NULL;
        }
        if (!mock_is_file(path)) {
            mock_set_file(path, "");
        }

        MockFile *f = new MockFile(path, mode);
        mock_ids.push_back(f);
        return (FILE*)f;
    }
    if (!orig_fopen) orig_fopen = *(FILE*(*)(const char *path, const char *mode))dlsym(RTLD_NEXT, "fopen");
    return orig_fopen(path, mode);
}

FILE* fopen64(const char *path, const char *mode)
{
    return fopen(path, mode);
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (stream == NULL) return -1;
        if (stream == stdin) stream = (FILE*)mock_ids[0];
        VALIDATE(stream);

        std::string name = MF(stream)->name;
        if (name == "/dev/urandom" || name == "/dev/random") {
            ssize_t genrand = size * nmemb;
            static int val = 123;
            char *res = (char*)ptr;
            while (genrand) {
                *(res++) = (val % 0xFF);
                val += 999;
                val ^= 0x7FF7;
                val %= 0xAFFF;
                --genrand;
            }
            return (size * nmemb);
        }
        if (!mock_is_file(name)) return 0;

        std::string data = mock_get_data(name);
        size_t pos = MF(stream)->pos;
        if (pos >= data.size()) {
            MF(stream)->eof = true;
            return 0;
        }

        size_t res_cnt = 0;
        if (ptr) {
            char *dat = (char*)ptr;
            for (size_t ss = pos; ss < data.size() && res_cnt < (size*nmemb); ++ss) {
                *(dat++) = data[ss];
                ++res_cnt;
            }
            MF(stream)->pos += res_cnt;
        }

        return res_cnt;
    }
    if (!orig_fread) orig_fread = (size_t (*)(void *ptr, size_t size, size_t nmemb, FILE *stream))dlsym(RTLD_NEXT, "fread");
    return orig_fread(ptr, size, nmemb, stream);
}

int fflush(FILE *stream)
{
    if (__mocking_io) return 0;

    if (!orig_fflush) orig_fflush = (int (*)(FILE *stream))dlsym(RTLD_NEXT, "fflush");
    return orig_fflush(stream);
}

int ferror(FILE *stream) throw ()
{
    if (__mocking_io) {
        if (stream == NULL) return -1;
        prepare_std();

        if (stream == stdout) return 0;
        else if (stream == stderr) return 0;
        else if (stream == stdin) return 0;

        VALIDATE(stream);

        return MF(stream)->error;
    }

    if (!orig_ferror) orig_ferror = (int (*)(FILE *stream))dlsym(RTLD_NEXT, "ferror");
    return orig_ferror(stream);
}

void clearerr(FILE *stream) throw()
{
    if (__mocking_io) {
        if (stream == NULL) return;
        prepare_std();

        if (stream == stdout) return;
        else if (stream == stderr) return;
        else if (stream == stdin) return;

        VALIDATE(stream);

        MF(stream)->error = 0;
        return;
    }
    if (!orig_clearerr) orig_clearerr = (void (*)(FILE *stream))dlsym(RTLD_NEXT, "clearerr");
    orig_clearerr(stream);
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (stream == NULL) return -1;
        prepare_std();

        if (stream == stdout) stream = (FILE*)mock_ids[1];
        else if (stream == stderr) stream = (FILE*)mock_ids[2];
        VALIDATE(stream);

        std::string name = MF(stream)->name;
        if (!mock_is_file(name) && MF(stream)->readonly()) return 0;
        if (name == "__stdout" || name == "__stderr") {
            if (!orig_fwrite) orig_fwrite = (size_t(*)(const void *ptr, size_t size, size_t nmemb, FILE *stream))dlsym(RTLD_NEXT, "fwrite");
            orig_fwrite(ptr, size, nmemb, name=="__stdout"?stdout:stderr);
            return size*nmemb;
        }

        std::string data = mock_get_data(name);
        while (data.size() < MF(stream)->pos) {
            data.append(1, ' ');
        }

        size_t cnt = 0;
        if (size * nmemb > 0) {
            data.append((const char*)ptr, size * nmemb);
            MF(stream)->pos += size * nmemb;
            cnt = size * nmemb;
            mock_set_file(MF(stream)->name, data);
        }

        return cnt;
    }
    if (!orig_fwrite) orig_fwrite = (size_t(*)(const void *ptr, size_t size, size_t nmemb, FILE *stream))dlsym(RTLD_NEXT, "fwrite");
    return orig_fwrite(ptr, size, nmemb, stream);
}

int fseek(FILE *stream, long offset, int whence)
{
    if (__mocking_io) {
        if (stream == NULL) return -1;

        if (whence == SEEK_SET) MF(stream)->pos = offset;
        else if (whence == SEEK_CUR) MF(stream)->pos += offset;
        else if (whence == SEEK_END) {
            std::string data = mock_get_data(MF(stream)->name);
            MF(stream)->pos = data.size() + offset;
        }
        return 0;
    }
    if (!orig_fseek) orig_fseek = (int (*)(FILE *stream, long offset, int whence))dlsym(RTLD_NEXT, "fseek");
    return orig_fseek(stream, offset, whence);
}

off_t lseek(int fd, off_t offset, int whence) throw()
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (fd < 3) return 0;
        if (fd >= (int)mock_ids.size()) return -1;

        return fseek((FILE*)mock_ids[fd], offset, whence);
    }
    if (!orig_lseek) orig_lseek = (int (*)(int stream, off_t offset, int whence))dlsym(RTLD_NEXT, "lseek");
    return orig_lseek(fd, offset, whence);
}

int feof(FILE *stream) throw ()
{
    if (__mocking_io) {
        return MF(stream)->eof;
    }
    if (!orig_feof) orig_feof = (int (*)(FILE *stream))dlsym(RTLD_NEXT, "feof");
    return orig_feof(stream);
}

int fileno(FILE *stream) throw ()
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__mockid_mutex);
        if (stream == stdout) return 1;
        else if (stream == stderr) return 2;
        else if (stream == stdin) return 0;

        VALIDATE(stream);

        if (MF(stream)->name == "__stdout") return 1;
        else if (MF(stream)->name == "__stderr") return 2;
        else if (MF(stream)->name == "__stdin") return 0;

        int num = 0;
        std::vector<MockFile*>::iterator it = mock_ids.begin();
        while (it != mock_ids.end()) {
            if (*it == MF(stream)) return num;
            //if ((*it)->name == MF(stream)->name) return num;
            ++it;
            ++num;
        }
        if (MF(stream)->open) {
            mock_ids.push_back(MF(stream));
            if (!mock_is_file(MF(stream)->name)) {
                mock_set_file(MF(stream)->name, "");
            }
            return mock_ids.size() - 1;
        }

        return -1;
    }
    if (!orig_fileno) orig_fileno = (int (*)(FILE *stream))dlsym(RTLD_NEXT, "fileno");
    return orig_fileno(stream);
}

long ftell(FILE *stream)
{
    if (__mocking_io) {
        if (stream == NULL) return -1;

        return MF(stream)->pos;
    }
    if (!orig_ftell) orig_ftell = (long (*)(FILE *stream))dlsym(RTLD_NEXT, "ftell");
    return orig_ftell(stream);
}

void rewind(FILE *stream)
{
    if (__mocking_io) {
        if (stream == NULL) return;

        MF(stream)->pos = 0;
        return;
    }
    if (!orig_rewind) orig_rewind = (void (*)(FILE *stream))dlsym(RTLD_NEXT, "rewind");
    return orig_rewind(stream);
}

int fgetpos(FILE *stream, fpos_t *pos)
{
    if (__mocking_io) {
        if (stream == NULL) return -1;

        if (pos) {
            *((size_t*)pos) = MF(stream)->pos;
        }
        return 0;
    }
    if (!orig_fgetpos) orig_fgetpos = (int (*)(FILE *stream, fpos_t *pos))dlsym(RTLD_NEXT, "fgetpos");
    return orig_fgetpos(stream, pos);
}

int fsetpos(FILE *stream, const fpos_t *pos)
{
    if (__mocking_io) {
        if (stream == NULL) return -1;

        if (pos) {
            MF(stream)->pos = *((size_t*)pos);
        }
        return 0;
    }
    if (!orig_fsetpos) orig_fsetpos = (int (*)(FILE *stream, const fpos_t *pos))dlsym(RTLD_NEXT, "fsetpos");
    return orig_fsetpos(stream, pos);
}

int fclose(FILE *fp)
{
    if (__mocking_io) {
        if (fp) {
            //((MockFile*)fp)->open = false;
            delete fp;
        }
        return 0;
    }
    if (!orig_fclose) orig_fclose = (int (*)(FILE *fp)) dlsym(RTLD_NEXT, "fclose");
    return orig_fclose(fp);
}


//int openat(int dirfd, const char *pathname, int flags, ...)
#if 0
int openat(int dirfd, const char *pathname, int flags)
{
#if 0
    mode_t mode;
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);
#endif

    if (__mocking_io) {
        if (dirfd < 3) return 0;
        //if (dirfd >= (int)mock_ids.size()) return -1;

        std::string prefix = "";

        //FIXME, directory fd support
        if (dirfd != AT_FDCWD) {
            prefix = "";
        }

        std::map<std::string, std::string>::iterator it = mock_files.begin();
        while (it != mock_files.end()) {
            try {
                if (boost::starts_with(it->first, pathname)) {
                    if ((flags & O_DIRECTORY) > 0) return 999;
                }
            } catch (boost::bad_lexical_cast) {
            }
            ++it;
        }
        //return -ENOENT;
        return 666;
    }
    if (!orig_openat) orig_openat = (int (*)(int dirfd, const char*pathname, int, ...))dlsym(RTLD_NEXT, "openat");
    return orig_openat(dirfd, pathname, flags);
}
#endif

long fpathconf(int fd, int name) throw()
{
    if (__mocking_io) {
        errno = 0;
        return 255;
    }

    if (!orig_fpathconf) orig_fpathconf = (long (*)(int,int))dlsym(RTLD_NEXT, "fpathconf");
    return orig_fpathconf(fd, name);
}

long pathconf(char *path, int name) throw()
{
    if (__mocking_io) {
        errno = 0;
        return 255;
    }

    if (!orig_pathconf) orig_pathconf = (long (*)(char*,int))dlsym(RTLD_NEXT, "pathconf");
    return orig_pathconf(path, name);
}

DIR *opendir(const char *name)
{
    if (__mocking_io) {
    	//boost::lock_guard<boost::mutex> keylock(__fold_mutex);
        std::string fname = name;
        BOOST_FOREACH(std::string f, mock_folders) {
            if (f == fname) {
                return (DIR*)new MockDir(fname);
            }
        }
        errno = ENOENT;
        return NULL;
    }
    if (!orig_opendir) orig_opendir = (DIR* (*)(const char*))dlsym(RTLD_NEXT, "opendir");
    return orig_opendir(name);
}

struct dirent *readdir(DIR *dirp)
{
    if (__mocking_io) {
        struct dirent res;
        struct dirent *res2 = NULL;
        int val = readdir_r(dirp, &res, &res2);
        if (val == 0) return res2;
        else {
            errno = ENOENT;
            return NULL;
        }

        errno = 0;
        return NULL;
    }
    if (!orig_readdir) orig_readdir = (struct dirent* (*)(DIR*))dlsym(RTLD_NEXT, "readdir");
    return orig_readdir(dirp);
}

int readdir64_r(DIR *dirp, struct dirent64 *entry, struct dirent64 **result)
{
    if (__mocking_io) {
        VALIDATE_DIR(dirp);

        unsigned int cnt = 0;

    	//boost::lock_guard<boost::mutex> keylock(__data_mutex);
        auto it = mock_files.begin();
        while (it != mock_files.end()) {
            if (boost::starts_with(it->first, MD(dirp)->name)) {
                if (MD(dirp)->entry == cnt) {
                    ++(MD(dirp)->entry);

                    unsigned int prefix_len = MD(dirp)->name.length() + 1;
                    strncpy(entry->d_name, it->first.c_str() + prefix_len, 254);
                    entry->d_name[254] = 0;
                    entry->d_ino = 0;
                    entry->d_type = DT_REG;
                    entry->d_reclen = 0;
                    entry->d_off = 0;

                    *result = entry;
                    return 0;
                }
                ++cnt;
            }
            ++it;
        }
#if 1
    	//boost::lock_guard<boost::mutex> keylock_fold(__fold_mutex);
        BOOST_FOREACH(std::string f, mock_folders) {
            if (boost::starts_with(f, MD(dirp)->name) && f != MD(dirp)->name) {
                if (MD(dirp)->entry == cnt) {
                    ++(MD(dirp)->entry);
                    unsigned int prefix_len = MD(dirp)->name.length() + 1;
                    strncpy(entry->d_name, f.c_str()+ prefix_len, 254);
                    entry->d_name[254] = 0;
                    entry->d_ino = 900 + cnt;
                    entry->d_type = DT_DIR;
                    entry->d_reclen = 0;
                    entry->d_off = 0;

                    *result = entry;
                    return 0;
                }
                ++cnt;
            }
        }
#endif

            *result = NULL;
        if (cnt>0) return 0;
        return ENOENT;
    }
    if (!orig_readdir_r) orig_readdir_r = (int (*)(DIR*,struct dirent64*, struct dirent64**))dlsym(RTLD_NEXT, "readdir64_r");
    return orig_readdir_r(dirp, entry, result);
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
    return readdir64_r(dirp, (struct dirent64*)entry, (struct dirent64**)result);
}

struct dirent64 *readdir64(DIR *dirp)
{
    return (struct dirent64*)readdir(dirp);
}

int closedir(DIR *dirp)
{
    if (__mocking_io) {
        VALIDATE_DIR(dirp);

        delete MD(dirp);
        return 0;
    }
    if (!orig_closedir) orig_closedir = (int (*)(DIR*))dlsym(RTLD_NEXT, "closedir");
    return orig_closedir(dirp);
}

#undef MF
#undef MD
#undef VALIDATE
#undef VALIDATE_DIR
