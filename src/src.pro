TEMPLATE = app
TARGET = 
DEPENDPATH += . blieng ui
INCLUDEPATH += . blieng ui
LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem

# Input
HEADERS += blieng/character.h blieng/data.h blieng/player_character.h ui/generate_character.h
SOURCES += main.cpp \
           blieng/character.cpp \
           blieng/data.cpp \
           blieng/player_character.cpp \
           ui/generate_character.cpp
