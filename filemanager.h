#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include "gamewindow.h"


class FileManager
{    
public:
    FileManager(GameWindow* gamewindow,QString filename);
    FileManager();
    void loadFile(GameWindow* gamewindow,QString filename);
};

#endif // FILEMANAGER_H
