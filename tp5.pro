INCLUDEPATH += /opt/local/include
INCLUDEPATH += /usr/local/include/libiomp

SOURCES += $$PWD/openglwindow.cpp \
    main.cpp \
    trianglewindow.cpp
HEADERS += $$PWD/openglwindow.h \
    trianglewindow.h

target.path = .

INSTALLS += target

RESOURCES += gestionnaire.qrc

QMAKE_CXXFLAGS += -fopenmp -std=c++11
LIBS += -fopenmp -std=c++11
