/*
 * Copyright 2014-2018 Jouni Roivas
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

class LogLevel
{
public:
    LogLevel(std::string name, int prio) : m_name(name), m_priority(prio) {}
    inline const std::string name() const
    {
        return m_name;
    }
    inline int priority() const
    {
        return m_priority;
    }
    bool operator==(const LogLevel &other) const
    {
        return other.m_name == m_name && other.m_priority == m_priority;
    }
    bool operator!=(const LogLevel &other) const
    {
        return other.m_name != m_name || other.m_priority != m_priority;
    }

protected:
    std::string m_name;
    int m_priority;
};

namespace log {
    static LogLevel Critical("CRITICAL", 0);
    static LogLevel Error("ERROR", 1);
    static LogLevel Warning("WARNING", 2);
    static LogLevel Info("INFO", 3);
    static LogLevel Debug("DEBUG", 5);
}

/**
 * Set log level
 * Messages with same or lesser logging level will be shown.
 *
 * \param level Logging level to be set
 */
void setLogLevel(const LogLevel &level);
/**
 * Get current logging level
 * To dynamically detect current logging level.
 *
 * \returns Current logging level
 */
const LogLevel getLogLevel();
/**
 * Get current logging level name
 * To dynamically detect current logging level.
 *
 * \returns Current logging level name
 */
const std::string getLogLevelName();

/**
 * Logs message to stream
 *
 * \param level Logging level to put the message
 * \param msg The message to put in log stream
 */
void __do_log(const LogLevel &level, std::string msg, std::string function="");

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
#define LOG_CRITICAL(X) __LOG(blieng::log::Critical, X);
#define LOG_ERROR(X) __LOG(blieng::log::Error, X);
#define LOG_WARNING(X) __LOG(blieng::log::Warning, X);
#define LOG_INFO(X) __LOG(blieng::log::Info, X);
#define LOG_DEBUG(X) __LOG(blieng::log::Debug, X);

#endif

}  // namespace blieng

#endif  // __BLIENG_LOGGING_H
