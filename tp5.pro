QMAKE_MAC_SDK = macosx10.11
QT       += network
QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11

INCLUDEPATH += /opt/local/include
LIBS += -stdlib=libc++

SOURCES += $$PWD/openglwindow.cpp \
    tcpserver.cpp \
    camera.cpp \
    point.cpp \
    particule.cpp \
    filemanager.cpp \
    gamewindow.cpp \
    trianglewindow.cpp
HEADERS += $$PWD/openglwindow.h \
    tcpserver.h \
    camera.h \
    point.h \
    particule.h \
    filemanager.h \
    gamewindow.h \
    trianglewindow.h

SOURCES += \
    main.cpp

target.path = .

INSTALLS += target

RESOURCES += gestionnaire.qrc
