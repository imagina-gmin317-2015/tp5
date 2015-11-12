#include "filemanager.h"

FileManager::FileManager(){

}


void FileManager::saveCustomMap(QString localPath, TriangleWindow** fenetres, int nbFenetres){

    /* Structure du fichier
     *
     * nbFenetres
     *
     * numéro de fenetres
     * etat camera
     * anim camera
     * rotX camera
     * rotY camera
     * ss camera
     * saison fenetre
     * nbPoint fenetre
     * tous les points
     * nbPolygones
     * particules
     * ...
     * numéro de fenetres
     * ...
     *
     *
     */

    // Utilisation de qint32 pour préserver la portabilité (window mac linux)

    QFile file(localPath);
    // Ouverture du fichier en écriture seule
    if (!file.open(QIODevice::WriteOnly))
        return;
    // Création de notre QDataStream à partir de notre fichier précédemment ouvert
    QDataStream binStream(&file);
    //QTextStream binStream(&file);
    // On spécifie la version de l'encodage qui va être utilisé par notre objet QDataStream
    binStream.setVersion(QDataStream::Qt_5_5);

    //Ecriture du nombre de fenetres
    binStream<<(qint32)nbFenetres;
    //qDebug()<<"Il y a "<<nbFenetres<<" fenetres"<<endl;

    for(qint32 i = 0; i < nbFenetres; i++){

        binStream<<i;
        //Camera
        paramCamera *c = fenetres[i]->getCamera();
        binStream<<(qint32)c->etat;
        binStream<<(float)c->anim;
        binStream<<(float)c->rotX;
        binStream<<(float)c->rotY;
        binStream<<(float)c->ss;
        //qDebug()<<c->etat;
        //qDebug()<<c->anim;
        //qDebug()<<c->rotX;
        //qDebug()<<c->rotY;
        //qDebug()<<c->ss<<endl<<endl;

        //Feenetre
        binStream<<(qint32)fenetres[i]->getSeason();
        //qDebug()<<fenetres[i]->getSeason()<<endl<<endl;

        //Terrain
        binStream<<(qint32)fenetres[i]->getNbPoints();
        binStream<<(qint32)fenetres[i]->getWidthPoint();
        binStream<<(qint32)fenetres[i]->getHeightPoint();
        //qDebug()<<"Il y a "<<fenetres[i]->getNbPoints()<<" points"<<endl;
        for(int j = 0; j < fenetres[i]->getNbPoints(); j++){
            Point* p = fenetres[i]->getPoints()[j];
            binStream<<(float)p->getX();
            binStream<<(float)p->getY();
            binStream<<(float)p->getZ();
            binStream<<(float)p->getSpeed();
        }


        //Couleurs
        for(int j = 0; j < fenetres[i]->getNbPoints(); j++){
            GLfloat* p = fenetres[i]->getColors()[j];
            binStream<<(float)p[0];
            binStream<<(float)p[1];
            binStream<<(float)p[2];
        }



        //Polygones
        if(fenetres[i]->getNbPolygones() > 0){
            binStream<<(qint32)fenetres[i]->getNbPolygones();
            for(int j = 0; j < fenetres[i]->getNbPolygones(); j++){
                binStream<<(QString)fenetres[i]->getPolygones()[j]->getNom();
                binStream<<(qint32)fenetres[i]->getPolygones()[j]->getNbPoints();
                for(int k = 0; k < fenetres[i]->getPolygones()[j]->getNbPoints(); k++){
                    binStream<<(float)fenetres[i]->getPolygones()[j]->getPoints()[k]->getX();
                    binStream<<(float)fenetres[i]->getPolygones()[j]->getPoints()[k]->getY();
                    binStream<<(float)fenetres[i]->getPolygones()[j]->getPoints()[k]->getZ();

                    binStream<<(float)fenetres[i]->getPolygones()[j]->getNormales()[k]->getX();
                    binStream<<(float)fenetres[i]->getPolygones()[j]->getNormales()[k]->getY();
                    binStream<<(float)fenetres[i]->getPolygones()[j]->getNormales()[k]->getZ();

                }
                binStream<<(qint32)fenetres[i]->getPolygones()[j]->getNbFaces();
                for(int k = 0; k < fenetres[i]->getPolygones()[j]->getNbFaces(); k++){
                    binStream<<(qint32)fenetres[i]->getPolygones()[j]->getFaces()[k]->getNbPoints();
                    for(int l = 0; l < fenetres[i]->getPolygones()[j]->getFaces()[k]->getNbPoints(); l++){
                        binStream<<(float)fenetres[i]->getPolygones()[j]->getFaces()[k]->getPoints()[l]->getX();
                        binStream<<(float)fenetres[i]->getPolygones()[j]->getFaces()[k]->getPoints()[l]->getY();
                        binStream<<(float)fenetres[i]->getPolygones()[j]->getFaces()[k]->getPoints()[l]->getZ();
                  }
                }

                binStream<<(float)fenetres[i]->getPolygones()[j]->getTaille();
                binStream<<(float)fenetres[i]->getPolygones()[j]->getPosX();
                binStream<<(float)fenetres[i]->getPolygones()[j]->getPosY();
                binStream<<(float)fenetres[i]->getPolygones()[j]->getPosZ();
                binStream<<(float)fenetres[i]->getPolygones()[j]->getRotX();
                binStream<<(float)fenetres[i]->getPolygones()[j]->getRotY();
                binStream<<(float)fenetres[i]->getPolygones()[j]->getRotZ();

            }
        }else{
            binStream<<(qint32)0;
        }

        //Particules
        if(fenetres[i]->getParticules() != NULL){
            binStream<<(qint32)fenetres[i]->getParticules()->getNbParticules();
            for(int j = 0; j < fenetres[i]->getParticules()->getNbParticules(); j++){
                binStream<<(float)fenetres[i]->getParticules()->getParticules()[j]->getX();
                binStream<<(float)fenetres[i]->getParticules()->getParticules()[j]->getY();
                binStream<<(float)fenetres[i]->getParticules()->getParticules()[j]->getZ();
                binStream<<(float)fenetres[i]->getParticules()->getParticules()[j]->getSpeed();
                binStream<<(float)fenetres[i]->getParticules()->getSol()[j];
            }
            binStream<<(qint32)fenetres[i]->getParticules()->getNbPointsBase();
            for(int j = 0; j < fenetres[i]->getParticules()->getNbPointsBase(); j++){
                binStream<<(float)fenetres[i]->getParticules()->getPointsBase()[j]->getX();
                binStream<<(float)fenetres[i]->getParticules()->getPointsBase()[j]->getY();
                binStream<<(float)fenetres[i]->getParticules()->getPointsBase()[j]->getZ();
            }
           binStream<<(qint32)fenetres[i]->getParticules()->getParticuleSize();

           binStream<<(GLfloat)fenetres[i]->getParticules()->getParticuleColor()[0];
           binStream<<(GLfloat)fenetres[i]->getParticules()->getParticuleColor()[1];
           binStream<<(GLfloat)fenetres[i]->getParticules()->getParticuleColor()[2];

        }else{
           binStream<<(qint32)0;
        }

    }
    file.close();
}


TriangleWindow** FileManager::loadCustomMap(QString localPath){
    QFile file(localPath);
    // Ouverture du fichier en lecture seule
    if (!file.open(QIODevice::ReadOnly))
        return NULL;
    // Création de notre QDataStream à partir de notre fichier précédemment ouvert
    QDataStream binStream(&file);
    // On spécifie exactement le même type d'encodage que celui utilisé lors de l'écriture
    binStream.setVersion(QDataStream::Qt_5_5);

    //Premiere ligne
    qint32 nbFenetres;
    binStream >> nbFenetres;
    this->nbFenetres = nbFenetres;
    TriangleWindow** fenetres = (TriangleWindow**)malloc(sizeof(TriangleWindow*) * nbFenetres);
    //qDebug()<<nbFenetres;
    for(int i = 0 ; i < nbFenetres; i++){
        qint32 numFenetre;
        binStream >> numFenetre;
        fenetres[numFenetre] = (TriangleWindow*)malloc(sizeof(TriangleWindow));
        fenetres[numFenetre] = new TriangleWindow(true);
        paramCamera *c = new paramCamera();

        binStream>>c->etat;
        binStream>>c->anim;
        binStream>>c->rotX;
        binStream>>c->rotY;
        binStream>>c->ss;
        //qDebug()<<c->etat;
        //qDebug()<<c->anim;
        //qDebug()<<c->rotX;
        //qDebug()<<c->rotY;
        //qDebug()<<c->ss;
        fenetres[numFenetre]->c = c;

        qint32 season;
        binStream>>season;
        //fenetres[numFenetre]->setSeason(season);
        //qDebug()<<season;

        qint32 nbPoints;
        qint32 width;
        qint32 height;
        binStream>>nbPoints;
        binStream>>width;
        binStream>>height;

        fenetres[numFenetre]->setNbPoints(nbPoints);
        fenetres[numFenetre]->setWidthPoint(width);
        fenetres[numFenetre]->setHeightPoint(height);
        Point** p = (Point**)malloc(sizeof(Point*) * nbPoints);
        for(int j = 0; j < nbPoints; j++){
            float x;
            float y;
            float z;
            float speed;
            binStream>>x;
            binStream>>y;
            binStream>>z;
            binStream>>speed;

            p[j] = new Point();

            p[j]->setX(x);
            p[j]->setY(y);
            p[j]->setZ(z);
            p[j]->setSpeed(speed);
        }
        fenetres[numFenetre]->setPoints(p);

        //Couleurs
        GLfloat** colors = (GLfloat**)malloc(sizeof(GLfloat*) * nbPoints);
        for(int j = 0; j < nbPoints; j++){
            colors[j] = (GLfloat*)malloc(sizeof(GLfloat) * 3);
            binStream>>colors[j][0];
            binStream>>colors[j][1];
            binStream>>colors[j][2];
        }
        fenetres[numFenetre]->setColors(colors);


        //Polygones
        int nbPoly;
        binStream>>nbPoly;
        //qDebug()<<"Nb Poly"<<nbPoly;
        fenetres[numFenetre]->setNbPolygones(nbPoly);
        if(nbPoly > 0){
            Ply** plys = (Ply**)malloc(sizeof(Ply*) * nbPoly);
            for(int j = 0; j < nbPoly; j++){
                plys[j] = new Ply();
                QString s;
                binStream>>s;
                //qDebug()<<s;
                plys[j]->setNom(s);
                int nbPtsPly;
                binStream>>nbPtsPly;
                plys[j]->setNbPoints(nbPtsPly);
                //qDebug()<<"Nbpoints "<<nbPtsPly;

                Point** ptsPlys = (Point**)malloc(sizeof(Point*) * nbPtsPly);
                Point** norPlys = (Point**)malloc(sizeof(Point*) * nbPtsPly);
                for(int k = 0; k < nbPtsPly; k++){
                    ptsPlys[k] = new Point();
                    norPlys[k] = new Point();
                    float ptsX;
                    float ptsY;
                    float ptsZ;
                    float norX;
                    float norY;
                    float norZ;
                    binStream>>ptsX;
                    binStream>>ptsY;
                    binStream>>ptsZ;

                    binStream>>norX;
                    binStream>>norY;
                    binStream>>norZ;

                    ptsPlys[k]->setX(ptsX);
                    ptsPlys[k]->setY(ptsY);
                    ptsPlys[k]->setZ(ptsZ);

                    norPlys[k]->setX(norX);
                    norPlys[k]->setY(norY);
                    norPlys[k]->setZ(norZ);
                }
                plys[j]->setPoints(ptsPlys);
                plys[j]->setNormales(norPlys);


                int nbFacesPly;
                binStream>>nbFacesPly;
                plys[j]->setNbFaces(nbFacesPly);
                //qDebug()<<"nbfaces "<<nbFacesPly;

                Face** faces = (Face**)malloc(sizeof(Face*) * nbFacesPly);
                for(int k = 0; k < nbFacesPly; k++){
                    faces[k] = new Face();
                    int nbPointsFaces;
                    binStream>>nbPointsFaces;
                    faces[k]->setNbPoints(nbPointsFaces);
                    Point** pointsFaces = (Point**)malloc(sizeof(Point*) * nbPointsFaces);
                    for(int l = 0; l < nbPointsFaces; l++){
                        pointsFaces[l] = new Point();
                        float ptsX;
                        float ptsY;
                        float ptsZ;

                        binStream>>ptsX;
                        binStream>>ptsY;
                        binStream>>ptsZ;

                        pointsFaces[l]->setX(ptsX);
                        pointsFaces[l]->setY(ptsY);
                        pointsFaces[l]->setZ(ptsZ);
                    }
                    faces[k]->setPoints(pointsFaces);
                }
                plys[j]->setFaces(faces);

                float taille;
                float posX;
                float posY;
                float posZ;
                float rotX;
                float rotY;
                float rotZ;

                binStream>>taille;
                binStream>>posX;
                binStream>>posY;
                binStream>>posZ;
                binStream>>rotX;
                binStream>>rotY;
                binStream>>rotZ;

                plys[j]->setTaille(taille);
                plys[j]->setPosX(posX);
                plys[j]->setPosY(posY);
                plys[j]->setPosZ(posZ);
                plys[j]->setRotX(rotX);
                plys[j]->setRotY(rotY);
                plys[j]->setRotZ(rotZ);

            }
            fenetres[numFenetre]->setPolygones(plys);
        }

        //Particules
        int nbParticules;
        binStream>>nbParticules;
        if(nbParticules > 0){
            Particules * parti = new Particules();
            Point** particules = (Point**)malloc(sizeof(Point*) * nbParticules);
            float* sol = (float*)malloc(sizeof(float) * nbParticules);
            for(int j = 0; j < nbParticules; j++){
                particules[j] = new Point();
                float pX;
                float pY;
                float pZ;
                float speed;
                float so;
                binStream>>pX;
                binStream>>pY;
                binStream>>pZ;
                binStream>>speed;
                binStream>>so;

                particules[j]->setX(pX);
                particules[j]->setY(pY);
                particules[j]->setZ(pZ);
                particules[j]->setSpeed(speed);
                sol[j] = so;
            }
            parti->setNbParticules(nbParticules);
            //qDebug()<<"NbParticules "<<nbParticules;
            parti->setParticules(particules);
            parti->setSol(sol);

            int nbPointsBases;
            binStream>>nbPointsBases;
            Point** pointsBases = (Point**)malloc(sizeof(Point*) * nbPointsBases);

            for(int j = 0; j < nbPointsBases; j++){
                pointsBases[j] = new Point();
                float x;
                float y;
                float z;
                binStream>>x;
                binStream>>y;
                binStream>>z;
                pointsBases[j]->setX(x);
                pointsBases[j]->setY(y);
                pointsBases[j]->setZ(z);
            }
            parti->setNbPointsBase(nbPointsBases);
            //qDebug()<<"NbPoints "<<nbPointsBases;
            parti->setPointsBase(pointsBases);

            int particuleSize;
            binStream>>particuleSize;
            GLfloat x;
            GLfloat y;
            GLfloat z;
            binStream>>x;
            binStream>>y;
            binStream>>z;
            GLfloat * couleur = (GLfloat*)malloc(sizeof(GLfloat) * 3);
            couleur[0] = x;
            couleur[1] = y;
            couleur[2] = z;

            parti->setParticuleColor(couleur);
            parti->setParticuleSize(particuleSize);
            //qDebug()<<"ParticuleSize "<<particuleSize;

            fenetres[numFenetre]->setParticules(parti);
            fenetres[numFenetre]->setSeason(season, true);

        }else{
            fenetres[numFenetre]->setSeason(season, false);
        }

        //qDebug()<<"Fin chargement fenetre "<<numFenetre;
    }
    file.close();

    return fenetres;
}

int FileManager::getNbFenetres(){
    return nbFenetres;
}
