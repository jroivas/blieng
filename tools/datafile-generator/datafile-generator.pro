TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../.. ../../src

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += -L../../blieng -lblieng
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
}
windows {
    CONFIG(debug, debug|release) {
        LIBS += ../../blieng/debug/libblieng.a
    } else {
        LIBS += ../../blieng/release/libblieng.a
    }
    LIBS += -lboost_system-mt
    LIBS += -lboost_filesystem-mt
    LIBS += -lboost_random-mt
    LIBS += -ljsoncpp
}

# Input
SOURCES += main.cpp
