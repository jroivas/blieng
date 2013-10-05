#include "test_tools.h"
#include <map>
#include <iostream>
#include <cstdio>
#include <dlfcn.h>
#include <boost/assert.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class MockFile
{
public:
    MockFile(std::string name) : magic(0x420012), name(name), open(true), mode(""), pos(0), eof(false) {
    }
    MockFile(std::string name, std::string mode) : magic(0x420012), name(name), open(true), mode(mode), pos(0), eof(false) {}

    bool readonly()
    {
        return (mode == "r");
    }

    bool writeable()
    {
        return (mode == "r+" || mode == "w" || mode == "rw" || mode == "w+" || mode == "a" || mode == "a+");
    }

    unsigned int magic;
    std::string name;
    bool open;
    std::string mode;
    size_t pos;
    bool eof;
};

#define VALIDATE(X) BOOST_ASSERT(X != NULL); BOOST_ASSERT(((MockFile*)X)->magic == 0x420012); 
#define MF(X) ((MockFile*)X)

static bool __mocking_io = false;
static std::map<std::string, std::string> mock_files;
static std::vector<MockFile*> mock_ids;

void mock_io_start()
{
    if (!mock_is_file("__stdin")) mock_set_file("__stdin", "");
    if (!mock_is_file("__stdout")) mock_set_file("__stdout", "");
    if (!mock_is_file("__stderr")) mock_set_file("__stderr", "");
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

void mock_set_file(std::string name, std::string data)
{
    mock_files[name] = data;
}

void mock_remove_file(std::string name)
{
    std::map<std::string, std::string>::iterator it = mock_files.find(name);
    if (it != mock_files.end()) {
        mock_files.erase(it);
    }
}

bool mock_is_file(std::string name)
{
    return (mock_files.find(name) != mock_files.end());
}

std::string mock_get_data(std::string name)
{
    BOOST_ASSERT( mock_is_file(name) );
    return mock_files[name];
}

std::vector<std::string> mock_list_files()
{
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
int (*orig_close)(int fd) = NULL;
int (*orig_stat)(const char *path, struct stat *buf) = NULL;
int (*orig_xstat)(int x, const char *path, struct stat *buf) = NULL;
int (*orig_xstat64)(int x, const char *path, struct stat64 *buf) = NULL;
int (*orig_fstat)(int fd, struct stat *buf) = NULL;
int (*orig_lstat)(const char *path, struct stat *buf) = NULL;
int (*orig_fflush)(FILE *stream) = NULL;
ssize_t (*orig_read)(int fd, void *buf, size_t count) = NULL;
ssize_t (*orig_write)(int fd, const void *buf, size_t count) = NULL;
size_t (*orig_fread)(void *ptr, size_t size, size_t nmemb, FILE *stream) = NULL;
size_t (*orig_fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream) = NULL;
int (*orig_fseek)(FILE *stream, long offset, int whence) = NULL;
long (*orig_ftell)(FILE *stream) = NULL;
int (*orig_feof)(FILE *stream) = NULL;
int (*orig_fileno)(FILE *stream) = NULL;
void (*orig_rewind)(FILE *stream) = NULL;
int (*orig_fgetpos)(FILE *stream, fpos_t *pos) = NULL;
int (*orig_fsetpos)(FILE *stream, const fpos_t *pos) = NULL;
int (*orig_fclose)(FILE *fp) = NULL;

void prepare_std()
{
    if (mock_ids.size() >= 3) return;

    mock_ids.push_back(new MockFile("__stdin", "r"));
    mock_ids.push_back(new MockFile("__stdout", "w"));
    mock_ids.push_back(new MockFile("__stderr", "w"));

    if (!mock_is_file("__stdin")) mock_set_file("__stdin", "");
    if (!mock_is_file("__stdout")) mock_set_file("__stdout", "");
    if (!mock_is_file("__stderr")) mock_set_file("__stderr", "");
}

int real_open(const char *pathname, int flags, mode_t mode)
{
    if (__mocking_io) {
        prepare_std();
    
        std::string amode = "";
        
        if (flags & O_RDONLY) amode = "r";
        else if (flags & O_TRUNC) amode = "w+";
        else if (flags & O_WRONLY) amode = "w";
        else amode = "w";

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

        if (mock_ids.size() <= (unsigned int)fd) return -1;

        mock_ids[fd]->open = false;
        delete mock_ids[fd];
        return 0;
    }

    if (!orig_close) orig_close = *(int (*)(int))dlsym(RTLD_NEXT, "close");
    return orig_close(fd);
}

int fstat(int fd, struct stat *buf)
{
    if (__mocking_io) {
        return 0;
    }
    if (!orig_fstat) orig_fstat = *(int (*)(int, struct stat *))dlsym(RTLD_NEXT, "fstat");
    return orig_fstat(fd, buf);
}

int stat(const char *path, struct stat *buf)
{
    if (__mocking_io) {
        return __xstat(0, path, buf);
    }
#if 0
    if (!orig_stat) orig_stat = *(int (*)(const char *, struct stat *))dlsym(RTLD_NEXT, "stat");
    return orig_stat(path, buf);
#else
    if (!orig_xstat) orig_xstat = *(int (*)(int, const char *, struct stat *))dlsym(RTLD_NEXT, "__xstat");
    return orig_xstat(0, path, buf);
#endif
}

int __xstat(int x, const char *path, struct stat *buf) throw ()
{
#if 0
    if (__mocking_io) {
        if (mock_is_file(path)) {
            if (buf) {
                buf->st_dev = 1;
                buf->st_ino = 2;
                buf->st_mode = 0664;
                buf->st_nlink = 1;
                buf->st_uid = 0;
                buf->st_gid = 0;
                buf->st_rdev = 0;
                std::string data = mock_get_data(path);
                buf->st_size = data.size();
                buf->st_blksize = 51;
                buf->st_blocks = data.size() / 512 + 1;
            }
            return 0;
        }
        return -1;
    }
    if (!orig_xstat) orig_xstat = *(int (*)(int, const char *, struct stat *))dlsym(RTLD_NEXT, "__xstat");
    return orig_xstat(x, path, buf);
#endif
    return __xstat64(x, path, (struct stat64*)buf);
}

int __xstat64(int x, const char *path, struct stat64 *buf) throw ()
{
    if (__mocking_io) {
        if (mock_is_file(path)) {
            if (buf) {
                buf->st_dev = 1;
                buf->st_ino = 2;
                buf->st_mode = 0664;
                buf->st_nlink = 1;
                buf->st_uid = 0;
                buf->st_gid = 0;
                buf->st_rdev = 0;
                std::string data = mock_get_data(path);
                buf->st_size = data.size();
                buf->st_blksize = 51;
                buf->st_blocks = data.size() / 512 + 1;
            }
            return 0;
        }
        return -1;
    }
    if (!orig_xstat64) orig_xstat64 = *(int (*)(int, const char *, struct stat64 *))dlsym(RTLD_NEXT, "__xstat64");
    return orig_xstat64(x, path, buf);
}

int statvfs64(const char *path, struct statvfs64 *buf) throw ()
{
    return 0;
}

int __lxstat64(int x, const char *path, struct stat64 *buf) throw ()
{
    return __xstat64(x, path, buf);
}

int lstat(const char *path, struct stat *buf)
{
    if (__mocking_io) {
        return 0;
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
        std::string smode = mode;
        if (smode == "r" && !mock_is_file(path)) {
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
        if (stream == NULL) return -1;
        if (stream == stdin) stream = (FILE*)mock_ids[0];
        VALIDATE(stream);

        std::string name = MF(stream)->name;
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

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if (__mocking_io) {
        if (stream == NULL) return -1;
        prepare_std();
        if (stream == stdout) stream = (FILE*)mock_ids[1];
        else if (stream == stderr) stream = (FILE*)mock_ids[2];
        VALIDATE(stream);

        std::string name = MF(stream)->name;
        if (!mock_is_file(name) && MF(stream)->readonly()) return 0;

        std::string data = mock_get_data(name);
        while (data.size() < MF(stream)->pos) {
            //data.append(1, 0);
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

#undef MF
