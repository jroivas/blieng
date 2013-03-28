TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src/blieng
INCLUDEPATH += . ../../src/blieng
LIBS += -L/usr/lib -lboost_system -lboost_filesystem -lboost_random

QT += declarative
unix {
        CONFIG += link_pkgconfig
	PKGCONFIG += jsoncpp
}

HEADERS += items.h myscene.h ../../src/blieng/item.h ../../src/blieng/data.h
SOURCES += main.cpp items.cpp myscene.cpp ../../src/blieng/item.cpp ../../src/blieng/data.cpp
