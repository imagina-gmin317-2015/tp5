#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QTextStream>
#include <QFile>
#include <QDebug>

#include "point.h"

class FileManager
{
private:
    QFile file;
    Point *points;

    int** faces;
    int* nbPointPerFace;

    int nbFaces;
    int nbVertex;

public:
    FileManager(QString filePath);

    Point getPoint(int index);

    int getNbVertex();
    int getNbFaces();

    int* getNbPointPerFace();
    int** getFaces();
};

#endif // FILEMANAGER_H
