#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTextStream>
#include <QFile>
#include <QVector>
#include "trianglewindow.h"
#include <QDebug>

class TriangleWindow;

class FileManager
{
public:
    static FileManager* Instance();
    void save();
    void addWindow(TriangleWindow* tr);
    void load();

private:
    static FileManager *file_instance;

    FileManager();
    void setTriangle(QVector<TriangleWindow*> tr);
    QFile* file;
    QTextStream flux;
    QVector<TriangleWindow*> tr;
};

#endif // FILEMANAGER_H
