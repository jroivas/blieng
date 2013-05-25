TEMPLATE = app
TARGET = tests
DEPENDPATH += . ..
INCLUDEPATH += . ..

LIBS += -lcpptest

# Input
HEADERS += point_test.h \
           path_test.h \
           ../blieng/point.h \
           ../blieng/path.h
SOURCES += point_test.cpp \
           path_test.cpp \
           testrunner.cpp \
           ../blieng/point.cpp \
           ../blieng/path.cpp
