TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../.. ../../src

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += -L../../blieng -lblieng
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
}
windows {
    CONFIG(debug, debug|release) {
        LIBS += ../../blieng/debug/libblieng.a
    } else {
        LIBS += ../../blieng/release/libblieng.a
    }
    LIBS += -lboost_system-mt
    LIBS += -lboost_filesystem-mt
    LIBS += -lboost_random-mt
    LIBS += -ljsoncpp
}

# Input
HEADERS += editscreen.h \
    townproperties.h \
    ../../src/ui/mapscreen.h \
    ../../src/zomb/player_character.h \
    ../../src/zomb/zombie_character.h \
    ../../src/zomb/configure.h \
    ../../src/zomb/createworld.h
SOURCES += main.cpp \
    editscreen.cpp \
    townproperties.cpp \
    ../../src/ui/mapscreen.cpp \
    ../../src/ui/imageloader.cpp \
    ../../src/zomb/player_character.cpp \
    ../../src/zomb/zombie_character.cpp \
    ../../src/zomb/configure.cpp \
    ../../src/zomb/createworld.cpp
