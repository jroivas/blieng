TEMPLATE = app
TARGET = tests
DEPENDPATH += . ..
INCLUDEPATH += . ..

LIBS += -lcpptest

INCLUDEPATH += ../

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += jsoncpp
    LIBS += -L../ -lblieng
    LIBS += -L/usr/lib -lboost_system -lboost_random -lboost_filesystem
    POST_TARGETDEPS += ../libblieng.a
}

# Input
HEADERS += point_test.h \
           path_test.h \
           bliobject_test.h
#           ../blieng/point.h \
#           ../blieng/path.h
SOURCES += point_test.cpp \
           path_test.cpp \
           bliobject_test.cpp \
           testrunner.cpp
#           ../blieng/point.cpp \
#           ../blieng/path.cpp
