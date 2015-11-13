QT += core


TARGET = vao
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    trianglewindow.cpp \
    openglwindow.cpp \
    camera.cpp \
arraytools.cpp

HEADERS += \
    trianglewindow.h \
    openglwindow.h \
    camera.h \
arraytools.h

RESOURCES += gestionnaire.qrc

