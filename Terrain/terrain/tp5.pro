INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    gamewindow.cpp \
    mytcpserver.cpp
HEADERS += $$PWD/openglwindow.h \
    gamewindow.h \
    mytcpserver.h \
    commonstruct.h

SOURCES += \
    main.cpp

target.path = .
INSTALLS += target

RESOURCES += \
    gestionnaire.qrc

QT += network opengl

QMAKE_CXXFLAGS += -fopenmp -std=c++11
LIBS += -fopenmp -std=c++11

