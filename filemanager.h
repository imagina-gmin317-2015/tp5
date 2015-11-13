#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include "gamewindow.h"
#include "gamewindowshader.h"


class FileManager
{    
public:
    FileManager(GameWindow* gamewindow,QString filename);
    FileManager();
    void loadFile(GameWindow* gamewindow,QString filename);
    FileManager(GameWindowShader* gamewindow,QString filename);
    void loadFile(GameWindowShader* gamewindow,QString filename);
};

#endif // FILEMANAGER_H
