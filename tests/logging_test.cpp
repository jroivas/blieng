#include "logging_test.h"
#include "test_tools.h"
#include "logging.h"
#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( LoggingTest );

void LoggingTest::basic()
{
    CPPUNIT_ASSERT( "ERROR" == blieng::getLogLevel() );
    CPPUNIT_ASSERT( "INFO" != blieng::getLogLevel() );
    blieng::setLogLevel("INFO");
    CPPUNIT_ASSERT( "INFO" == blieng::getLogLevel() );

    CPPUNIT_ASSERT_THROW( blieng::setLogLevel("INVALID"), std::string );
    CPPUNIT_ASSERT( "INVALID" != blieng::getLogLevel() );
    blieng::setLogLevel("ERROR");
}

void LoggingTest::levels()
{
    mock_io_start();

    mock_clear_stdout();
    blieng::setLogLevel("ERROR");

    mock_clear_stdout();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") ==
        std::string::npos );

    mock_clear_stdout();
    LOG_ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stderr().find("ERROR: Got error") !=
        std::string::npos );

    blieng::setLogLevel("INFO");
    mock_clear_stdout();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") !=
        std::string::npos );

    blieng::setLogLevel("CRITICAL");
    mock_clear_stderr();
    LOG_ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stderr().find("ERROR: Got error") ==
        std::string::npos );

    blieng::setLogLevel("WARNING");
    mock_clear_stderr();
    LOG_WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stderr().find("WARNING: Warn") !=
        std::string::npos );

    blieng::setLogLevel("ERROR");
    mock_clear_stderr();
    LOG_WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stderr().find("WARNING: Warn") ==
        std::string::npos );

    blieng::setLogLevel("DEBUG");
    mock_clear_stderr();
    LOG_WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stderr().find("WARNING: Warn") !=
        std::string::npos );

    mock_clear_stderr();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") !=
        std::string::npos );

    mock_clear_stderr();
    LOG_DEBUG("Most verbosal");
    CPPUNIT_ASSERT( mock_get_stderr().find("DEBUG: Most verbosal") !=
        std::string::npos );

    mock_clear_stderr();
    LOG_INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stderr().find("INFO: Infoing something") !=
        std::string::npos );

    mock_clear_stderr();
    LOG_ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stderr().find("ERROR: Got error") !=
        std::string::npos );

    mock_clear_stderr();
    LOG_CRITICAL("Critical happened");
    CPPUNIT_ASSERT( mock_get_stderr().find("CRITICAL: Critical happened") !=
        std::string::npos );
    mock_clear_stderr();

    mock_io_stop();
}
