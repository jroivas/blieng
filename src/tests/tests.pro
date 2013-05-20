TEMPLATE = app
TARGET = testrunner
DEPENDPATH += . ../blieng
INCLUDEPATH += . ../blieng

LIBS += -lcpptest

# Input
HEADERS += point_test.h ../blieng/point.h
SOURCES += point_test.cpp testrunner.cpp ../blieng/point.cpp
