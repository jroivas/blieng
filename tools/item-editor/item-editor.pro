TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

QT += declarative
unix {
        CONFIG += link_pkgconfig
	PKGCONFIG += QJson
}

HEADERS += datas.h
SOURCES += main.cpp datas.cpp
