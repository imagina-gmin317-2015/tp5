INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    gamecamera.cpp \
    gamewindow.cpp \
    gameserver.cpp \
    gameclient.cpp \
    thread.cpp \
    filemanager.cpp \
    plyobject.cpp
HEADERS += $$PWD/openglwindow.h \
    gamecamera.h \
    gamewindow.h \
    gameserver.h \
    gameclient.h \
    thread.h \
    filemanager.h \
    plyobject.h

SOURCES += \
    main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
QT += network
QT += core gui widgets
QT += opengl
INSTALLS += target
QMAKE_MAC_SDK = macosx10.11

RESOURCES += \
    gestionnaire.qrc

DISTFILES += \
    shaders/SEV_fragment_shader.fsh \
    shaders/SEV_vertex_shader.vsh \
    shaders/diffus_vertex_shader.vsh \
    shaders/diffus_fragment_shader.fsh \
    shaders/NM_vertex_shader.vsh \
    shaders/NM_fragment_shader.fsh \
    shaders/deform_vertex_shader.vsh \
    shaders/deform_fragment_shader.fsh
