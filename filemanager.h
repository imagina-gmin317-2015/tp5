#ifndef FILEMANAGER
#define FILEMANAGER

#include <QtGui>

#include "trianglewindow.h"

class FileManager{
public:
    FileManager();

    void saveCustomMap(QString,TriangleWindow**,int);
    TriangleWindow** loadCustomMap(QString);

    int getNbFenetres();
private:
    int nbFenetres;
};

#endif // FILEMANAGER

