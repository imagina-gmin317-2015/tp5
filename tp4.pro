QMAKE_MAC_SDK = macosx10.11


#QMAKE_CXXFLAGS += -fopenmp
#QMAKE_LFLAGS += -fopenmp

CONFIG += c++11

INCLUDEPATH += /opt/local/include

SOURCES += $$PWD/openglwindow.cpp \
    main.cpp \
    trianglewindow.cpp \
    filemanager.cpp \
    plyloader.cpp
HEADERS += $$PWD/openglwindow.h \
    trianglewindow.h \
    filemanager.h \
    plyloader.h

target.path = .

INSTALLS += target

RESOURCES += gestionnaire.qrc
