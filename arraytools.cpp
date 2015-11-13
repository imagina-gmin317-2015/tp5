#include "arraytools.h"

ArrayTools::ArrayTools()
{
}

ArrayTools::ArrayTools(int nr,int nc)
{
    this->nRow=nr;
    this->nCol=nc;
}

ArrayTools::ArrayTools(int nr,int nc, int squareEdge)
{
    nRow=nr;
    nCol=nc;
    for (int i=0; i<nr;i++)
    {
        for (int j=0; j<nc;j++)
        {
            points.push_back(QVector3D(i,j,0));
        }
    }
}

void ArrayTools::setPoints(QVector<QVector3D> p){
    this->points=p;
}

void ArrayTools::randomMesh(int var)
{
for (int i=0;i<points.size();i++)
    {
    points[i][2]= rand() % var;
    }

}

QVector<QVector3D> ArrayTools::getPoints()
{
    return points;
}

QVector<int> ArrayTools::getIndex()
{
    return index;
}

QVector<QVector3D *> ArrayTools::getVertices()
{
    return vertices;
}

QVector<QVector3D > ArrayTools::getNormals()
{
    return normals;
}

//void ArrayTools::setTriangleMotif(QVector<int> motif)
// {
//     triangleMotif=motif;
// }

void ArrayTools::setTriangleMotif(motif motif)
 {
      QVector<int>  shape;
    if (motif == triangle){
        shape.push_back(0);
        shape.push_back(1);
        shape.push_back(nRow);
        shape.push_back(1);
        shape.push_back(nRow);
        shape.push_back(nRow+1);
    }

    this->mot=motif;
    triangleMotif=shape;
 }

void ArrayTools::makeIndex()
{

    std::vector<float> line;
    //int lengthMotif = (sizeof(triangleMotif)/sizeof(*triangleMotif)); // taille du tableau

        // Duplication horizontal
        for (int i=0;i< (nCol-1);i++) {
            for (int k=0;k<triangleMotif.size();k++){

                // On agit moralement sur les coordonnées le long de l'axe x, le premier axe (sur deux)
                line.push_back(triangleMotif[k]+nRow*i);
//                qDebug()<<triangleMotif[k]+nRow*i;
            }
        }

        // Duplication vertical

           for (int i=0;i< (nRow-1);i++) {
            for (int k=0;k< (int) line.size();k++){
                // On agit moralement sur les coordonnées le long de l'axe y, le deuxième axe (sur deux)
                index.push_back(line.at(k)+i);
//                qDebug()<<(line.at(k)+i);
            }
        }

}

void ArrayTools::buildVertices()
{
    for (int i=0;i<index.size();i++){
        vertices.push_back(&points[index[i]]);

    }
}

void ArrayTools::buildNormals()
{
    if (mot==triangle)
    for (int i=0;i<(int) index.size()/3;i+=3){
//        normals.push_back(QVector3D::normal(points[index[i]],points[index[i+1]],points[index[i+2]]));
//        normals.push_back(QVector3D::normal(points[index[i+1]],points[index[i+2]],points[index[i]]));
//        normals.push_back(QVector3D::normal(points[index[i+2]],points[index[i]],points[index[i+1]]));
        normals.push_back(QVector3D::normal(points[index[i+2]],points[index[i+1]],points[index[i]]));
        normals.push_back(QVector3D::normal(points[index[i+1]],points[index[i]],points[index[i+2]]));
        normals.push_back(QVector3D::normal(points[index[i]],points[index[i+2]],points[index[i+1]]));

    }
}


//void GameWindow::getNormal(point O, point X, point Y)
//{


//    // vecteur OX
//    point vx;
//    vx.x=X.x-O.x;
//    vx.y=X.y-O.y;
//    vx.z=X.z-O.z;
//    // norme de OX
//    float length=sqrt(vx.x*vx.x+vx.y*vx.y+vx.z*vx.z);
//    // normalisation
//    vx.x/=length;
//    vx.y/=length;
//    vx.z/=length;

//    // vecteur OY
//    point vy;
//    vy.x=Y.x-O.x;
//    vy.y=Y.y-O.y;
//    vy.z=Y.z-O.z;
//    // norme de OX
//    length=sqrt(vy.x*vy.x+vy.y*vy.y+vy.z*vy.z);
//    // normalisation
//    vy.x/=length;
//    vy.y/=length;
//    vy.z/=length;

//    // Calcul de la normale

//    point normal;
//    normal.x=vx.y*vy.z-vy.y*vx.z;
//    normal.y=vx.z*vy.x-vx.x*vy.z;
//    normal.z=vx.x*vy.y-vx.y*vy.x;

//    glNormal3f(normal.x,normal.y,normal.z);
//}

