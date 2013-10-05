#ifndef __TEST_TOOLS_H
#define __TEST_TOOLS_H

#include <sstream>

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

#endif
