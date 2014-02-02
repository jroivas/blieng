TEMPLATE = app
TARGET = tests
DEPENDPATH += . ..
INCLUDEPATH += . ..

LIBS += -lcppunit -ldl
#QMAKE_CXXFLAGS += -std=c++0x -g
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -O0
CONFIG += debug

INCLUDEPATH += ../

unix {
    LIBS += -L../ -lblieng
    LIBS += -L/usr/lib -L/usr/lib -Wl,-Bstatic  -lboost_system -lboost_random -lboost_filesystem -lboost_date_time -Wl,-Bdynamic
    POST_TARGETDEPS += ../libblieng.a
}

# Input
HEADERS += point_test.h \
           path_test.h \
           bliobject_test.h \
           auto_vector_test.h \
           auto_map_test.h \
           data_test.h \
           datafile_test.h \
           character_test.h \
           eventlog_test.h \
           item_test.h \
           bliany_test.h \
           card_test.h \
           town_test.h \
           maps_test.h \
           configure_test.h \
           test_tools.h
SOURCES += point_test.cpp \
           path_test.cpp \
           bliobject_test.cpp \
           auto_vector_test.cpp \
           auto_map_test.cpp \
           character_test.cpp \
           data_test.cpp \
           datafile_test.cpp \
           eventlog_test.cpp \
           item_test.cpp \
           bliany_test.cpp \
           card_test.cpp \
           town_test.cpp \
           maps_test.cpp \
           configure_test.cpp \
           testrunner.cpp \
           test_tools.cpp
