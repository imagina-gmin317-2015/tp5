#QMAKE_MAC_SDK = macosx10.11

#QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11 -fopenmp
QMAKE_CXXFLAGS += -std=c++11 -fopenmp

INCLUDEPATH += /opt/local/include
INCLUDEPATH += /usr/local/include/libiomp

#LIBS += -stdlib=libc++

SOURCES += $$PWD/openglwindow.cpp \
    main.cpp \
    trianglewindow.cpp
HEADERS += $$PWD/openglwindow.h \
    trianglewindow.h

target.path = .

INSTALLS += target

RESOURCES += gestionnaire.qrc
