#ifndef PLYLOADER_H
#define PLYLOADER_H

#include <QtCore>
#include <QtGui>
#include <QtGui/QOpenGLFunctions>

#include <QVector>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDebug>

struct point{
    float x,y,z;
};

class PlyLoader
{
public:
    PlyLoader(QString fileName);
    void load();
    void draw();

private:
    float x, y, z, ori, size;
    QFile *file;
    QVector<point> vertices;
    QVector<QVector<int>> faces;

signals:

public slots:
};

#endif // PLYLOADER_H
