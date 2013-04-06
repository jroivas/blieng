TEMPLATE = app
TARGET = zombiebli
DEPENDPATH += . blieng ui
INCLUDEPATH += . blieng ui
LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += jsoncpp
}

HEADERS += blieng/bliobject.h \
	   blieng/character.h \
	   blieng/data.h \
           blieng/wallclock.h \
	   blieng/item.h \
	   blieng/eventlog.h \
	   blieng/town.h \
	   zomb/player_character.h \
           zomb/zombie_character.h \
	   ui/generate_character.h \
	   ui/startscreen.h
SOURCES += main.cpp \
	   blieng/bliobject.cpp \
           blieng/character.cpp \
           blieng/data.cpp \
           blieng/wallclock.cpp \
           blieng/item.cpp \
	   blieng/eventlog.cpp \
	   blieng/town.cpp \
           zomb/player_character.cpp \
           zomb/zombie_character.cpp \
           ui/generate_character.cpp \
	   ui/startscreen.cpp
