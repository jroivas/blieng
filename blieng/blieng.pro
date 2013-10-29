TEMPLATE = lib
TARGET = blieng
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += staticlib
#QMAKE_CXXFLAGS += -std=c++11 #In newer compilers
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -g
#QMAKE_CXXFLAGS += -Wall -Werror -Wextra
QMAKE_CXXFLAGS += -Wall -Werror -pedantic -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused


unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    #LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem -lboost_date_time
    LIBS += -L/usr/lib -L/usr/lib -Wl,-Bstatic  -lboost_system -lboost_random -lboost_filesystem -lboost_date_time -Wl,-Bdynamic
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
    bliany.h \
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
