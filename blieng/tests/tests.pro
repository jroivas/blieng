TEMPLATE = app
TARGET = tests
DEPENDPATH += . ..
INCLUDEPATH += . ..

LIBS += -lcppunit
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += -L../ -lblieng
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
    POST_TARGETDEPS += ../libblieng.a
}

# Input
HEADERS += point_test.h \
           path_test.h \
           bliobject_test.h \
           data_test.h \
           character_test.h
SOURCES += point_test.cpp \
           path_test.cpp \
           bliobject_test.cpp \
           character_test.cpp \
           data_test.cpp \
           testrunner.cpp
