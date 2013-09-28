TEMPLATE = lib
TARGET = blieng
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += staticlib
#QMAKE_CXXFLAGS += -std=c++11 #In newer compilers
QMAKE_CXXFLAGS += -std=c++0x


unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
}

# Input
HEADERS += \
    bliobject.h \
    character.h \
    configure.h \
    data.h \
    datafile.h \
    eventlog.h \
    item.h \
    maps.h \
    path.h \
    point.h \
    town.h \
    card.h \
    auto_map.h \
    auto_vector.h \
    rijndael-alg-fst.h \
    wallclock.h
SOURCES += \
    bliobject.cpp \
    character.cpp \
    configure.cpp \
    data.cpp \
    datafile.cpp \
    eventlog.cpp \
    item.cpp \
    maps.cpp \
    path.cpp \
    point.cpp \
    town.cpp \
    card.cpp \
    rijndael-alg-fst.c \
    wallclock.cpp
