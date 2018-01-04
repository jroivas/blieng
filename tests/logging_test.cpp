#include "logging_test.h"
#include "test_tools.h"
#include "logging.h"
#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( LoggingTest );

void LoggingTest::basic()
{
    CPPUNIT_ASSERT(blieng::getLogLevel() == blieng::log::Error);
    CPPUNIT_ASSERT(blieng::getLogLevel() != blieng::log::Info);
    blieng::setLogLevel(blieng::log::Info);
    CPPUNIT_ASSERT(blieng::getLogLevel() == blieng::log::Info);

    blieng::setLogLevel(blieng::log::Error);
}

void LoggingTest::levels()
{
    mock_io_start();

    mock_clear_stdout();
    blieng::setLogLevel(blieng::log::Error);

    mock_clear_stdout();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") == std::string::npos );

    mock_clear_stdout();
    LOG_ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stderr().find("ERROR: Got error") != std::string::npos );

    blieng::setLogLevel(blieng::log::Info);
    mock_clear_stdout();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") != std::string::npos );

    blieng::setLogLevel(blieng::log::Critical);
    mock_clear_stderr();
    LOG_ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stderr().find("ERROR: Got error") == std::string::npos );

    blieng::setLogLevel(blieng::log::Warning);
    mock_clear_stderr();
    LOG_WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stderr().find("WARNING: Warn") != std::string::npos );

    blieng::setLogLevel(blieng::log::Error);
    mock_clear_stderr();
    LOG_WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stderr().find("WARNING: Warn") == std::string::npos );

    blieng::setLogLevel(blieng::log::Debug);
    mock_clear_stderr();
    LOG_WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stderr().find("WARNING: Warn") != std::string::npos );

    mock_clear_stderr();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") != std::string::npos );

    mock_clear_stderr();
    LOG_DEBUG("Most verbosal");
    CPPUNIT_ASSERT( mock_get_stderr().find("DEBUG: Most verbosal") != std::string::npos );

    mock_clear_stderr();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") != std::string::npos );

    mock_clear_stderr();
    LOG_ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stderr().find("ERROR: Got error") != std::string::npos );

    mock_clear_stderr();
    LOG_CRITICAL("Critical happened");
    CPPUNIT_ASSERT( mock_get_stderr().find("CRITICAL: Critical happened") != std::string::npos );
    mock_clear_stderr();

    mock_io_stop();
}
