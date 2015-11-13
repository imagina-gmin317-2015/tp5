#ifndef PLYOBJECT_H
#define PLYOBJECT_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class PlyObject
{
public:
    PlyObject(QString filename);
    PlyObject();

    void readPlyFile();

    float* getTabPoints() const;
    int* getTabFaces() const;
    int getNbFaces() const;

    QString getFilename() const;

    float getAngle() const;
    void setAngle(float angle);

private:
    QString m_filename;
    int m_nbVertex;
    int m_nbFaces;

    float* m_tabPoints;
    int* m_tabFaces;
    float m_angle = 0;
};

#endif // PLYOBJECT_H
