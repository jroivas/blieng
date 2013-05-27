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
    LIBS += -ljsoncpp
    LIBS += -lblieng
}

# Input
HEADERS += editscreen.h \
    ../../src/ui/mapscreen.h \
    ../../src/zomb/player_character.h \
    ../../src/zomb/zombie_character.h \
    ../../src/zomb/configure.h \
    ../../src/zomb/createworld.h
SOURCES += main.cpp \
    editscreen.cpp \
    ../../src/ui/mapscreen.cpp \
    ../../src/zomb/player_character.cpp \
    ../../src/zomb/zombie_character.cpp \
    ../../src/zomb/configure.cpp \
    ../../src/zomb/createworld.cpp
