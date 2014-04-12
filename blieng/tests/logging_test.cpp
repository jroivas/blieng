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
    INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stdout().find("INFO: Infoing something") == std::string::npos );

    mock_clear_stdout();
    ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stdout().find("ERROR: Got error") != std::string::npos );

    blieng::setLogLevel("INFO");
    mock_clear_stdout();
    INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stdout().find("INFO: Infoing something") != std::string::npos );

    blieng::setLogLevel("CRITICAL");
    mock_clear_stdout();
    ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stdout().find("ERROR: Got error") == std::string::npos );

    blieng::setLogLevel("WARNING");
    mock_clear_stdout();
    WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stdout().find("WARNING: Warn") != std::string::npos );

    blieng::setLogLevel("ERROR");
    mock_clear_stdout();
    WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stdout().find("WARNING: Warn") == std::string::npos );

    blieng::setLogLevel("DEBUG");
    mock_clear_stdout();
    WARNING("Warn");
    CPPUNIT_ASSERT( mock_get_stdout().find("WARNING: Warn") != std::string::npos );

    mock_clear_stdout();
    INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stdout().find("INFO: Infoing something") != std::string::npos );

    mock_clear_stdout();
    DEBUG("Most verbosal");
    CPPUNIT_ASSERT( mock_get_stdout().find("DEBUG: Most verbosal") != std::string::npos );

    mock_clear_stdout();
    INFO("Infoing something");
    CPPUNIT_ASSERT( mock_get_stdout().find("INFO: Infoing something") != std::string::npos );

    mock_clear_stdout();
    ERROR("Got error");
    CPPUNIT_ASSERT( mock_get_stdout().find("ERROR: Got error") != std::string::npos );

    mock_clear_stdout();
    CRITICAL("Critical happened");
    CPPUNIT_ASSERT( mock_get_stdout().find("CRITICAL: Critical happened") != std::string::npos );
    mock_clear_stdout();

    mock_io_stop();
}
