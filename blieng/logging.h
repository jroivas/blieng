/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_LOGGING_H
#define __BLIENG_LOGGING_H

#include <iostream>
#include <string>
#include <vector>

/**
 * Master blieng namespace
 * Everything on this lib should be under this namespace.
 */
namespace blieng
{

/**
 * Get list of log levels
 *
 * \returns Vector of strings containing valid log levels
 */
const std::vector<std::string> logLevels();
/**
 * Set log level
 * Messages with same or lesser logging level will be shown.
 *
 * \param level Logging level to be set
 */
void setLogLevel(std::string level);
/**
 * Get current logging level
 * To dynamically detect current logging level.
 *
 * \returns Current logging level
 */
const std::string getLogLevel();

/**
 * Logs message to stream
 *
 * \param level Logging level to put the message
 * \param msg The message to put in log stream
 */
void __do_log(std::string level, std::string msg, std::string function="");

#ifdef RELEASE

// Disable logging for release
#define LOG_LEVEL(LEVEL)
#define LOG_CRITICAL(X)
#define LOG_ERROR(X)
#define LOG_WARNING(X)
#define LOG_INFO(X)
#define LOG_DEBUG(X)

#else

// Dynamic log levels
//#define __LOG(LEVEL, X) blieng::__do_log(LEVEL, X, __PRETTY_FUNCTION__)
#define __LOG(LEVEL, X) blieng::__do_log(LEVEL, X)
#define LOG_LEVEL(LEVEL) blieng::setLogLevel(LEVEL)
#define LOG_CRITICAL(X) __LOG("CRITICAL", X);
#define LOG_ERROR(X) __LOG("ERROR", X);
#define LOG_WARNING(X) __LOG("WARNING", X);
#define LOG_INFO(X) __LOG("INFO", X);
#define LOG_DEBUG(X) __LOG("DEBUG", X);

#endif

}  // namespace blieng

#endif  // __BLIENG_LOGGING_H
