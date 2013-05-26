TEMPLATE = lib
TARGET = blieng
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += staticlib

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
}

windows {
    INCLUDEPATH += C:\boost_lib
    INCLUDEPATH += C:\build\jsoncpp-src-0.6.0-rc2\include
}

# Input
HEADERS += \
    bliobject.h \
    character.h \
    configure.h \
    data.h \
    eventlog.h \
    item.h \
    maps.h \
    path.h \
    point.h \
    town.h \
    wallclock.h
SOURCES += \
    bliobject.cpp \
    character.cpp \
    configure.cpp \
    data.cpp \
    eventlog.cpp \
    item.cpp \
    maps.cpp \
    path.cpp \
    point.cpp \
    town.cpp \
    wallclock.cpp
