#ifndef ARRAYTOOLS_H
#define ARRAYTOOLS_H
#include <QtGui/QVector3D>
#include <QVector>
#include <QtDebug>

enum  motif {triangle};

class ArrayTools
{
public:
    ArrayTools();
    ArrayTools(int nr,int nc, int squareEdge);
    ArrayTools(int nr,int nc);

    void randomMesh(int var);

    //void setTriangleMotif(QVector<int> motif);

    void setTriangleMotif(motif  motif);
    void makeIndex();
    void buildVertices();
    void buildNormals();
    void setPoints(QVector<QVector3D> p);

    QVector<QVector3D> getPoints();
    QVector<int> getIndex();
    QVector<QVector3D *> getVertices();
    QVector<QVector3D > getNormals();


private:
    int nRow,nCol;
    QVector<int> triangleMotif;
    motif mot;
    QVector<int> index;

    QVector<QVector3D> points;
    QVector<QVector3D *> vertices;
    QVector<QVector3D > normals;

};

#endif // ARRAYTOOLS_H
