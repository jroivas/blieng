TEMPLATE = lib
TARGET = blieng
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += staticlib
#QMAKE_CXXFLAGS += -std=c++11 #In newer compilers
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -Wall -Wextra

unix:!android {
    LIBS += -L/usr/lib -L/usr/lib -Wl,-Bstatic  -lboost_system -lboost_random -lboost_filesystem -lboost_date_time -Wl,-Bdynamic
    QMAKE_CXXFLAGS += -Werror -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
}
android {
        INCLUDEPATH += ../boost/include/boost-1_53/
        LIBS += ../boost/lib/
        QT += core
        CONFIG += rtti
        CONFIG += exceptions
        QMAKE_CXXFLAGS += -fexceptions -frtti
}
windows {
        QMAKE_CXXFLAGS += -fpermissive
}


# Input
HEADERS += \
    blieng.h \
    config.h \
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
    bliany.h \
    logging.h \
    auto_map.h \
    auto_vector.h \
    rijndael-alg-fst.h \
    json.h \
    wallclock.h
SOURCES += \
    blieng.cpp \
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
    logging.cpp \
    rijndael-alg-fst.c \
    json.c \
    wallclock.cpp
