/*
 * Copyright 2014-2018 Jouni Roivas
 */

#include "blieng/logging.h"

#include <map>
#include <string>
#include <vector>

#ifdef ANDROID
#include <QDebug>
#endif

static blieng::LogLevel __log_level = blieng::log::Error;

void blieng::__do_log(const blieng::LogLevel &level, std::string msg, std::string function)
{
    if (level.priority() > __log_level.priority()) return;

    if (!function.empty()) function = "\n    @" + function;
#ifdef ANDROID
    qDebug() << level.name().c_str() << ": " << msg.c_str() << function.c_str();
#else
    std::cerr << level.name() << ": " << msg  << function << std::endl;
#endif
}

void blieng::setLogLevel(const blieng::LogLevel &level)
{
    __log_level = level;
}

const blieng::LogLevel blieng::getLogLevel()
{
    return __log_level;
}

const std::string blieng::getLogLevelName()
{
    return __log_level.name();
}
