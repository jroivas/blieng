TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem

# Input
HEADERS += character.h player_character.h
SOURCES += character.cpp main.cpp player_character.cpp
