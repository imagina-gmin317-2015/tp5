#include "plyloader.h"



PlyLoader::PlyLoader(QString fileName)
{
    this->file = new QFile(fileName);
    this->x=0;
    this->y=0;
    this->z=0;
    this->size = 0.1;
    this->ori = 0;
}

void PlyLoader::load()
{
    this->file->open(QIODevice::ReadOnly);
    QTextStream flux(this->file);
    QString fichierString = flux.readAll();
    QStringList list = fichierString.split("\n");

    // Verifiaction du format
    if(!list.at(0).contains("ply")){
        qDebug()<<"Erreur : format du fichier '"<<this->file->fileName()<<"' incorrect";
        return;
    }

    // Lecture du nombre de vertices
    int nbVertices =0;
    int cpt =0;
    while (nbVertices == 0) {
        if(list.at(cpt).contains("element vertex")){
            nbVertices = list.at(cpt).split(" ").last().toInt();
        }
        cpt++;
    }

    // Lecture du nombre de faces
    int nbFaces = 0;
    while (nbFaces == 0) {
        if(list.at(cpt).contains("element face")){
            nbFaces = list.at(cpt).split(" ").last().toInt();
        }
        cpt++;
    }

    // On avance jusqu'à la fin du header
    while(list.at(cpt++).contains("end_header") != 0);


    QStringList li;
    // Lectures des vertices
    for (int i = cpt+1; i < cpt + nbVertices +1 ; ++i) {
        li = list.at(i).split(" ");
        point p,q;
        //point
        p.x = li.at(0).toFloat();
        p.y = li.at(1).toFloat();
        p.z = li.at(2).toFloat();
        vertices.push_back(p);
        //nx,ny,nz ?
        q.x = li.at(3).toFloat();
        q.y = li.at(4).toFloat();
        q.z = li.at(5).toFloat();
        vertices.push_back(q);
    }


    int val = cpt + nbVertices + 1;
    // Lecture des faces
    for (int i = val; i <val + nbFaces; ++i) {
        QVector<int> face;
        li = list.at(i).split(" ");
        for (int j = 0; j < li.size(); ++j) {
            face.push_back(li.at(j).toInt());
        }
        faces.push_back(face);
    }


}

void PlyLoader::draw()
{
    /*glPushMatrix();
       glTranslatef(this->x, this->y, this->z);
       glRotatef(this->ori, 0, 0, 1);*/
       glColor3f(0, 1, 0);

   #pragma omp for schedule(dynamic)
       for (int i = 0; i < faces.size(); ++i) {
           QVector<int> temp = faces.at(i);
           if(temp.at(0) == 3) {
               glBegin(GL_TRIANGLES);
           } else {
               glBegin(GL_QUADS);
           }

           for (int j = 1; j < temp.size(); ++j) {
               point p = vertices.at(temp.at(j) * 2);
               point n = vertices.at(temp.at(j) * 2 + 1);
               glNormal3f(n.y, n.z, n.x);
               glVertex3f(p.x * size, p.y * size, p.z * size);
           }

           glEnd();
       }

     //  glPopMatrix();
}
