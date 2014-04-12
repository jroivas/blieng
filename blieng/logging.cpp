#include <map>
#include <vector>
#include "logging.h"

static std::string __log_level = "ERROR";

static std::map<std::string, int> __log_levels = {
    {"CRITICAL", 0},
    {"ERROR", 1},
    {"WARNING", 2},
    {"INFO", 3},
    {"DEBUG", 5}
    };

const std::vector<std::string> blieng::logLevels()
{
    std::vector<std::string> res;
    auto iter = __log_levels.begin();
    while (iter != __log_levels.end()) {
        res.push_back(iter->first);
        ++iter;
    }
    return res;
}

void blieng::__do_log(std::string level, std::string msg)
{
    if (__log_levels[level] <= __log_levels[__log_level] ) {
        std::cout << level << ": " << msg << std::endl;
    }
}

void blieng::setLogLevel(std::string level)
{
    if (__log_levels.find(level) == __log_levels.end()) throw std::string("Invalid log level!");
     __log_level = level;
}

const std::string blieng::getLogLevel()
{
     return __log_level;
}
