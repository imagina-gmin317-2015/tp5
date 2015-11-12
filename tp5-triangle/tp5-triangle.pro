#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T12:11:49
#
#-------------------------------------------------

QT       += core gui network  opengl


QMAKE_CXXFLAGS += -std=c++11 -fopenmp -lGLU -lglut -lgfw -lglew

INCLUDEPATH += /opt/local/include
INCLUDEPATH += /usr/local/include/libiomp
LIBS+=-lglut -lGLU


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp5-2
TEMPLATE = app


SOURCES += main.cpp\
        openglwindow.cpp \
    trianglewindow.cpp

HEADERS  += openglwindow.h \
        trianglewindow.h


target.path = .

INSTALLS += target

RESOURCES += \
    gestionnaire.qrc

DISTFILES += \
    cell-shading-fragment.fsh \
    vdiffusereflection.vsh
