#include "ply.h"

Ply::Ply(){

}

Ply::Ply(QString localpath){

    nom = localpath.split(".").at(0);
    taille = 1;
    posX = 0.0f;
    posY = 0.0f;
    posZ = 0.0f;
    rotX = 0.0f;
    rotY = 0.0f;
    rotZ = 0.0f;
    QFile file(localpath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);
    QString line = flux.readLine();
    //Récupération du nombre de points
    while (!line.contains("element vertex ")){
        line = flux.readLine();

    }


    QString string = line.split("element vertex ").at(1);
    int nb = string.toInt();
    nbPoints = nb;
    //qDebug()<<"NbVertex : "<<nb;
    bool isNormales = false;
    //Récupération du nombre de faces
    while (!line.contains("element face ")){
        if(line.contains("nx")){
            isNormales = true;
        }
        line = flux.readLine();
    }
    string = line.split("element face ").at(1);
    nb = string.toInt();
    nbFaces = nb;
    //qDebug()<<"NbFaces : "<<nb;

    //On saute le header
    while (!line.contains("end_header")){
        line = flux.readLine();
    }

    points = (Point**)malloc(sizeof(Point*) * nbPoints);
    if(isNormales){
        normales = (Point**)malloc(sizeof(Point*) * nbPoints);
    }else{
        normales = NULL;
    }
    int i = 0;
    while(i < nbPoints){
        points[i] = new Point();
        if(isNormales){
            normales[i] = new Point();
        }
        line = flux.readLine();
        //qDebug()<<line.split(" ");
        points[i]->setX(line.split(" ").at(0).toFloat());
        points[i]->setY(line.split(" ").at(1).toFloat());
        points[i]->setZ(line.split(" ").at(2).toFloat());
        if(isNormales){
            normales[i]->setX(line.split(" ").at(3).toFloat());
            normales[i]->setY(line.split(" ").at(4).toFloat());
            normales[i]->setZ(line.split(" ").at(5).toFloat());
        }
        //qDebug()<<line;
        i++;
    }


    faces = (Face**)malloc(sizeof(Face*) * nbFaces);
    i = 0;
    while(i < nbFaces){
        faces[i] = new Face();
        line = flux.readLine();

        int nPts = line.split(" ").at(0).toInt();
        Point** pts = (Point**)malloc(sizeof(Point*) * nPts);
        for(int j = 1; j < nPts + 1; j++){
            pts[j-1] = points[line.split(" ").at(j).toInt()];
        }
        faces[i] = new Face(nPts, pts);

        i++;
    }

    //qDebug("fin lecture");
    file.close();
}


void Ply::changeModel(QString localpath){
    nom = localpath.split(".").at(0);
    QFile file(localpath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);
    QString line = flux.readLine();

    //Récupération du nombre de points
    while (!file.atEnd() && !line.contains("element vertex ")){
        line = flux.readLine();
    }
    QString string = line.split("element vertex ").at(1);
    int nb = string.toInt();
    nbPoints = nb;
    //qDebug()<<"NbVertex : "<<nb;

    //Récupération du nombre de faces
    while (!file.atEnd() && !line.contains("element face ")){
        line = flux.readLine();
    }
    string = line.split("element face ").at(1);
    nb = string.toInt();
    nbFaces = nb;
    //qDebug()<<"NbFaces : "<<nb;


    //On saute le header
    while (!file.atEnd() && !line.contains("end_header")){
        line = flux.readLine();
    }

    points = (Point**)malloc(sizeof(Point*) * nbPoints);
    normales = (Point**)malloc(sizeof(Point*) * nbPoints);
    int i = 0;
    while(i < nbPoints){
        points[i] = new Point();
        normales[i] = new Point();
        line = flux.readLine();
        //qDebug()<<line.split(" ");
        points[i]->setX(line.split(" ").at(0).toFloat());
        points[i]->setY(line.split(" ").at(1).toFloat());
        points[i]->setZ(line.split(" ").at(2).toFloat());

        normales[i]->setX(line.split(" ").at(3).toFloat());
        normales[i]->setY(line.split(" ").at(4).toFloat());
        normales[i]->setZ(line.split(" ").at(5).toFloat());
        //qDebug()<<line;
        i++;
    }


    faces = (Face**)malloc(sizeof(Face*) * nbFaces);
    i = 0;
    while(i < nbFaces){
        faces[i] = new Face();
        line = flux.readLine();
        int nPts = line.split(" ").at(0).toInt();
        Point** pts = (Point**)malloc(sizeof(Point*) * nPts);
        for(int j = 1; j < nPts + 1; j++){
            pts[j-1] = points[line.split(" ").at(j).toInt()];
        }
        faces[i] = new Face(nPts, pts);

        i++;
    }

    //qDebug("fin lecture");
    file.close();

}


void Ply::setPoints(Point** p){
    points = p;
}

Point** Ply::getPoints(){
    return points;
}

void Ply::setNbPoints(int n){
    nbPoints = n;
}

int Ply::getNbPoints(){
    return nbPoints;
}

void Ply::setNormales(Point** p){
    normales = p;
}

Point** Ply::getNormales(){
    return normales;
}

void Ply::setFaces(Face** f){
    faces = f;
}

Face** Ply::getFaces(){
    return faces;
}

void Ply::setNbFaces(int n){
    nbFaces = n;
}

int Ply::getNbFaces(){
    return nbFaces;
}

void Ply::setTaille(float n){
    taille = n;
}

float Ply::getTaille(){
    return taille;
}

void Ply::setPosX(float f){
    posX = f;
}

float Ply::getPosX(){
    return posX;
}

void Ply::setPosY(float f){
    posY = f;
}

float Ply::getPosY(){
    return posY;
}

void Ply::setPosZ(float f){
    posZ = f;
}

float Ply::getPosZ(){
    return posZ;
}

void Ply::setRotX(float f){
    rotX = f;
}

float Ply::getRotX(){
    return rotX;
}

void Ply::setRotY(float f){
    rotY = f;
}

float Ply::getRotY(){
    return rotY;
}

void Ply::setRotZ(float f){
    rotZ = f;
}

float Ply::getRotZ(){
    return rotZ;
}

QString Ply::getNom(){
    return nom;
}

void Ply::setNom(QString s){
    nom = s;
}


float Ply::moyX(){
    float moy = 0.0f;
    for(int i = 0; i < nbPoints; i++){
        moy += points[i]->getX();
    }
    moy /= nbPoints;

    return moy;
}

float Ply::moyY(){
    float moy = 0.0f;
    for(int i = 0; i < nbPoints; i++){
        moy += points[i]->getY();
    }
    moy /= nbPoints;

    return moy;
}

float Ply::moyZ(){
    float moy = 0.0f;
    for(int i = 0; i < nbPoints; i++){
        moy += points[i]->getZ();
    }
    moy /= nbPoints;

    return moy;
}


void Ply::initMatrix(){
    glPushMatrix();
    glScalef(taille,taille,taille);
    glTranslatef((posX/taille)-moyX() ,(posY/taille)-moyY(),(posZ/taille)-moyZ());
    glRotatef(rotX, 0, 0, 1);
    glRotatef(rotY, 0, 1, 0);
    glRotatef(rotZ, 1, 0, 0);
}

void Ply::delMatrix(){
    glPopMatrix();
    //glPushMatrix();
}

void Ply::drawPoints(){
    initMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(1);
    glBegin(GL_POINTS);

    for(int i = 0; i < nbPoints; i++){
        glVertex3f(points[i]->getX(),
                   points[i]->getY(),
                   points[i]->getZ());
    }
    glEnd();
    delMatrix();
}

void Ply::drawFaces(){

    initMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);

    for(int i = 0; i < nbFaces; i++){
        if(faces[i]->getNbPoints() == 3){
            for(int  j = 0; j < faces[i]->getNbPoints(); j++){
                glVertex3f((/*posX - moyX() + */faces[i]->getPoints()[j]->getX())/**taille*/,
                           (/*posY - moyY() + */faces[i]->getPoints()[j]->getY())/**taille*/,
                           (/*posZ - moyZ() + */faces[i]->getPoints()[j]->getZ())/**taille*/);
            }
        }else{
            for(int  j = 0; j < 3; j++){
                glVertex3f((/*posX - moyX() + */faces[i]->getPoints()[j]->getX())/**taille*/,
                           (/*posY - moyY() + */faces[i]->getPoints()[j]->getY())/**taille*/,
                           (/*posZ - moyZ() + */faces[i]->getPoints()[j]->getZ())/**taille*/);
            }
            glVertex3f((/*posX - moyX() + */faces[i]->getPoints()[2]->getX())/**taille*/,
                       (/*posY - moyY() + */faces[i]->getPoints()[2]->getY())/**taille*/,
                       (/*posZ - moyZ() + */faces[i]->getPoints()[2]->getZ())/**taille*/);
            glVertex3f((/*posX - moyX() + */faces[i]->getPoints()[3]->getX())/**taille*/,
                       (/*posY - moyY() + */faces[i]->getPoints()[3]->getY())/**taille*/,
                       (/*posZ - moyZ() + */faces[i]->getPoints()[3]->getZ())/**taille*/);
            glVertex3f((/*posX - moyX() + */faces[i]->getPoints()[0]->getX())/**taille*/,
                       (/*posY - moyY() + */faces[i]->getPoints()[0]->getY())/**taille*/,
                       (/*posZ - moyZ() + */faces[i]->getPoints()[0]->getZ())/**taille*/);

        }
    }
    glEnd();
    delMatrix();
}

void Ply::drawLines(){
    initMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for(int i = 0; i < nbFaces; i++){
        if(faces[i]->getNbPoints() == 3){
            for(int  j = 0; j < faces[i]->getNbPoints(); j++){
                glVertex3f(faces[i]->getPoints()[j]->getX(),
                           faces[i]->getPoints()[j]->getY(),
                           faces[i]->getPoints()[j]->getZ());
            }
        }else{
            for(int  j = 0; j < 3; j++){
                glVertex3f(faces[i]->getPoints()[j]->getX(),
                           faces[i]->getPoints()[j]->getY(),
                           faces[i]->getPoints()[j]->getZ());
            }
            glVertex3f(faces[i]->getPoints()[2]->getX(),
                       faces[i]->getPoints()[2]->getY(),
                       faces[i]->getPoints()[2]->getZ());
            glVertex3f(faces[i]->getPoints()[3]->getX(),
                       faces[i]->getPoints()[3]->getY(),
                       faces[i]->getPoints()[3]->getZ());
            glVertex3f(faces[i]->getPoints()[0]->getX(),
                       faces[i]->getPoints()[0]->getY(),
                       faces[i]->getPoints()[0]->getZ());

        }
    }

    glEnd();
    delMatrix();
}
