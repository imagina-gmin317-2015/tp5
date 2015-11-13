INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    gamewindow.cpp \
    camera.cpp \
    gamewindowserver.cpp \
    gamewindowclient.cpp \
    mythread.cpp \
    filemanager.cpp \
    point.cpp
HEADERS += $$PWD/openglwindow.h \
    gamewindow.h \
    camera.h \
    gamewindowserver.h \
    gamewindowclient.h \
    mythread.h \
    filemanager.h \
    point.h

SOURCES += \
    main.cpp

target.path = .
QT += network
QT += core gui widgets
QT += opengl
INSTALLS += target

RESOURCES += \
    gestionnaire.qrc

DISTFILES += \
    diffus_fragment_shader.fsh \
    diffus_vertex_shader.vsh \
    NM_fragment_shader.fsh \
    NM_vertex_shader.vsh \
    Deform_fragment_shader.fsh \
    Deform_vertex_shader.vsh \
    SEM_fragment_shader.fsh \
    SEM_vertex_shader.vsh
