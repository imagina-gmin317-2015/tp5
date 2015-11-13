INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    gamewindow.cpp \
    Camera.cpp \
    filemanager.cpp \
    objetply3d.cpp \
    gamewindowshader.cpp \
    trianglewindow.cpp
HEADERS += $$PWD/openglwindow.h \
    Camera.h \
    gamewindow.h \
    filemanager.h \
    objetply3d.h \
    gamewindowshader.h \
    trianglewindow.h

SOURCES += \
    main.cpp

target.path = .
INSTALLS += target

RESOURCES += \
    gestionnaire.qrc

QT += network
QT += opengl

DISTFILES += \
    Shaders/vertexShader.vsh \
    Shaders/fragmentShader.fsh \
    Shaders/SEM_fragmentShader.fsh \
    Shaders/SEM_vertexShader.vsh \
    Shaders/NM_fragmentShader.fsh \
    Shaders/NM_vertexShader.vsh \
    Shaders/DEF_vertexShader.vsh \
    Shaders/DEF_fragmentShader.fsh
