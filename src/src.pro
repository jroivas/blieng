TEMPLATE = app
TARGET = zombiebli
DEPENDPATH += . .. ../blieng ui
INCLUDEPATH += . .. ../blieng ui

TRANSLATIONS = zombiebli.ts

QT += core gui
# For Qt5
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += ../blieng/libblieng.a
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
    POST_TARGETDEPS += ../blieng/libblieng.a

    ### For dynamic linking
    #LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
    #LIBS += -L../blieng -lblieng
}

windows {

    CONFIG(debug, debug|release) {
        LIBS += ../blieng/debug/libblieng.a
        POST_TARGETDEPS += ../blieng/debug/libblieng.a
    } else {
        LIBS += ../blieng/release/libblieng.a
        POST_TARGETDEPS += ../blieng/release/libblieng.a
    }
    LIBS += -lboost_system-mt
    LIBS += -lboost_filesystem-mt
    LIBS += -lboost_random-mt
    LIBS += -ljsoncpp
}

HEADERS += \
    zomb/player_character.h \
    zomb/zombie_character.h \
    zomb/createworld.h \
    zomb/worldclock.h \
    zomb/configure.h \
    ui/generate_character.h \
    ui/loadingscreen.h \
    ui/mapscreen.h \
    ui/gamescreen.h \
    ui/fightscreen.h \
    ui/gamelayout.h \
    ui/characterview.h \
    ui/zombpushbutton.h \
    ui/zombcombobox.h \
    ui/zombstyles.h \
    ui/imageloader.h \
    ui/startscreen.h
SOURCES += \
    main.cpp \
    zomb/player_character.cpp \
    zomb/zombie_character.cpp \
    zomb/createworld.cpp \
    zomb/worldclock.cpp \
    zomb/configure.cpp \
    ui/generate_character.cpp \
    ui/loadingscreen.cpp \
    ui/mapscreen.cpp \
    ui/gamescreen.cpp \
    ui/gamelayout.cpp \
    ui/fightscreen.cpp \
    ui/characterview.cpp \
    ui/zombpushbutton.cpp \
    ui/zombcombobox.cpp \
    ui/zombstyles.cpp \
    ui/imageloader.cpp \
    ui/startscreen.cpp
