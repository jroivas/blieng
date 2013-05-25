TEMPLATE = app
TARGET = zombiebli
DEPENDPATH += . .. ../blieng ui
INCLUDEPATH += . .. ../blieng ui
#LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
#LIBS += -L../blieng -lblieng

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
    LIBS += ../blieng/libblieng.a
}

windows {
    #INCLUDEPATH += C:\Users\Jouni\gits\boost_1_53_0
    INCLUDEPATH += C:/boost_lib
    CONFIG += static
    QMAKE_CXXFLAGS += -std=c++11
    LIBS += "C:\boost_lib\bin.v2\libs\system\build\gcc-mingw-4.7.2\release\link-static\threading-multi\libboost_system-mgw47-mt-1_53.a"
    LIBS += "C:\boost_lib\bin.v2\libs\filesystem\build\gcc-mingw-4.7.2\release\link-static\threading-multi\libboost_filesystem-mgw47-mt-1_53.a"
    LIBS += "C:\boost_lib\bin.v2\libs\random\build\gcc-mingw-4.7.2\release\link-static\threading-multi\libboost_random-mgw47-mt-1_53.a"
    #LIBS += "C:\boost_lib\bin.v2\libs\random\build\gcc-mingw-4.7.2\release\link-static\threading-multi\random_device.o"

    INCLUDEPATH += C:/Users/Jouni/gits/jsoncpp-src-0.6.0-rc2/include


    CONFIG(debug, debug|release) {
        LIBS += ../blieng/debug/libblieng.a
    } else {
        LIBS += ../blieng/release/libblieng.a
    }
    LIBS += "C:\boost_lib\bin.v2\libs\system\build\gcc-mingw-4.7.2\release\link-static\threading-multi\libboost_system-mgw47-mt-1_53.a"
    LIBS += "C:\boost_lib\bin.v2\libs\filesystem\build\gcc-mingw-4.7.2\release\link-static\threading-multi\libboost_filesystem-mgw47-mt-1_53.a"
    LIBS += "C:\boost_lib\bin.v2\libs\random\build\gcc-mingw-4.7.2\release\link-static\threading-multi\libboost_random-mgw47-mt-1_53.a"
    LIBS += C:/Users/Jouni/gits/jsoncpp-src-0.6.0-rc2/buildscons/mingw/src/lib_json/libjson_mingw_libmt.a
}

HEADERS += zomb/player_character.h \
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
	   ui/startscreen.h
SOURCES += main.cpp \
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
	   ui/startscreen.cpp
