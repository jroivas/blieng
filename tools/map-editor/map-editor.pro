TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../.. ../../src

include(../build.pri)

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
