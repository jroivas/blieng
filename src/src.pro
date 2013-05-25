TEMPLATE = app
TARGET = zombiebli
DEPENDPATH += . .. ../blieng ui
INCLUDEPATH += . .. ../blieng ui
#LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
#LIBS += -L../blieng -lblieng
LIBS += ../blieng/libblieng.a
LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem 

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += jsoncpp
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
