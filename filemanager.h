#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "gamewindow.h"

class FileManager
{
public:
    FileManager(GameWindow * gameWindow, QString nameFile);
    FileManager();
    void readFile(GameWindow * gameWindow, QString nameFile);
    void loadObjectMap(GameWindow * gameWindow, QString nameFile);

signals:

public slots:
};

#endif // FILEMANAGER_H
