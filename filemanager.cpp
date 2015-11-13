#include "filemanager.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

using namespace std;

FileManager::FileManager(GameWindow * gameWindow, QString nameFile)
{
    QFile file("..\\tp4\\save\\"+nameFile+".save");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);

    flux.setCodec("UTF-8");

    flux << gameWindow->getCarte() << endl;
    flux << gameWindow->getSeason() << endl;
    flux << gameWindow->getDay() << endl;
    flux << gameWindow->getCamera()->getEtat() << endl;
    flux << gameWindow->getCamera()->getRotX() << endl;
    flux << gameWindow->getCamera()->getRotY() << endl;
    flux << gameWindow->getCamera()->getSS() << endl;

    if(gameWindow->getObjectLoaded() > 0) // ON A DES OBJET DANS LA SCENE
    {
        flux << gameWindow->getObjectLoaded() << endl;
        flux << gameWindow->getNameObject() << endl;
        flux << gameWindow->getNbObject() << endl;
        flux << gameWindow->getTailleObject() << endl;

        point *posObject = gameWindow->getPosObject();
        for(int i = 0; i < gameWindow->getNbObject(); ++i)
        {
            flux << posObject[i].x << " " << posObject[i].y << " " << posObject[i].z << endl;
        }
    }
}

FileManager::FileManager()
{

}

void FileManager::readFile(GameWindow * gameWindow, QString nameFile)
{
//    glPushMatrix();
//    glRotatef(45, 0, 0, 1);
//    glPopMatrix(); A LA FIN

    QFile fichier("..\\tp4\\save\\"+nameFile);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);

    QString ligne;
    ligne = flux.readLine();
    int carte = ligne.toInt();

    ligne = flux.readLine();
    int season = ligne.toInt();

    ligne = flux.readLine();
    int day = ligne.toInt();

    ligne = flux.readLine();
    int etat = ligne.toInt();

    ligne = flux.readLine();
    float rotX = ligne.toFloat();

    ligne = flux.readLine();
    float rotY = ligne.toFloat();

    ligne = flux.readLine();
    float SS = ligne.toFloat();

    Camera camera;
    camera.setEtat(etat);
    camera.setRotX(rotX);
    camera.setRotY(rotY);
    camera.setSS(SS);


    gameWindow->getCamera()->setEtat(etat);
    gameWindow->getCamera()->setRotX(rotX);
    gameWindow->getCamera()->setRotY(rotY);
    gameWindow->getCamera()->setSS(SS);
    gameWindow->setCarte(carte);
    gameWindow->setSeason(season);
    gameWindow->setDay(day);

    if(!flux.atEnd())
    {
        gameWindow->setObjectLoaded(flux.readLine().toInt());
        gameWindow->setNameObject(flux.readLine());

        int nbObject = flux.readLine().toInt();
        gameWindow->setNbObject(nbObject);
        gameWindow->setTailleObject(flux.readLine().toFloat());

        point posObject[nbObject];
        QString mot;
        for(int i = 0; i < nbObject; ++i)
        {
            flux >> mot;
            posObject[i].x = mot.toFloat();
            flux >> mot;
            posObject[i].y = mot.toFloat();
            flux >> mot;
            posObject[i].z = mot.toFloat();
        }

        gameWindow->setPosObject(posObject);

        gameWindow->loadObjectFile();
    }
    else
    {
        gameWindow->setObjectLoaded(0);
    }

}

void FileManager::loadObjectMap(GameWindow * gameWindow, QString nameFile)
{
    QFile fichier("..\\tp4\\"+nameFile);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);

    for(int i = 0; i < 3; ++i)
        flux.readLine(); // INUTILE

    QString mot;
    for(int i = 0; i < 3; ++i)
    {
        flux >> mot;
    }
    int nbSommets = mot.toInt();
    point allSommets[nbSommets];

    flux.readLine();


    for(int i = 0; i < 6; ++i)
        flux.readLine(); // INUTILE

    for(int i = 0; i < 3; ++i)
    {
        flux >> mot;
    }
    int nbFaces = mot.toInt();

    face allFaces[nbFaces];

    for(int i = 0; i < 3; ++i)
        flux.readLine(); // INUTILE

    for(int i = 0; i < nbSommets; ++i)
    {
        point pt;
        flux >> mot;
        pt.x = mot.toFloat();
        flux >> mot;
        pt.y = mot.toFloat();
        flux >> mot;
        pt.z = mot.toFloat();

        allSommets[i] = pt;

        flux.readLine();
    }

    glColor3f(1.0f, 0.0f, 0.0f);

    for(int i = 0; i < nbFaces; ++i)
    {
        flux >> mot;

        int nbPointsFace = mot.toInt();

        flux >> mot;
        allFaces[i].a = mot.toInt();
        flux >> mot;
        allFaces[i].b = mot.toInt();
        flux >> mot;
        allFaces[i].c = mot.toInt();

        if(nbPointsFace > 3)
        {
            flux >> mot;
            allFaces[i].d = mot.toInt();
        }
        else
        {
            allFaces[i].d = -1;
        }


        flux.readLine();
    }

    gameWindow->setNbSommets(nbSommets);
    gameWindow->setNbFaces(nbFaces);
    gameWindow->setAllSommets(allSommets);
    gameWindow->setAllFaces(allFaces);

}

