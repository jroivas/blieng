TEMPLATE = app
TARGET = zombiebli
DEPENDPATH += . blieng ui
INCLUDEPATH += . blieng ui
LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += jsoncpp
}

HEADERS += blieng/character.h blieng/data.h blieng/player_character.h \
           blieng/wallclock.h \
           blieng/zombie_character.h blieng/item.h ui/generate_character.h
SOURCES += main.cpp \
           blieng/character.cpp \
           blieng/data.cpp \
           blieng/player_character.cpp \
           blieng/zombie_character.cpp \
           blieng/wallclock.cpp \
           blieng/item.cpp \
           ui/generate_character.cpp
