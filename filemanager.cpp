#include "filemanager.h"
#include "gamewindow.h"
#include "gamewindowshader.h"

#include <QFile>
#include <QStringList>
#include <QList>
#include <QTextStream>
#include <QString>


FileManager::FileManager(GameWindow* gamewindow, QString filename)
{
    QFile file ("..\\TP4\\Sauvegarde\\" + filename +".save");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);
    flux.setCodec("UTF-8");
    flux << gamewindow->getCarte() << endl ;
    flux << gamewindow->day() << endl;
    flux << gamewindow->camera()->get_etat() << endl;
    flux << gamewindow->camera()->get_rotX() << endl;
    flux << gamewindow->camera()->get_rotY() << endl;
    flux << gamewindow->camera()->get_ss() << endl;

    //on ecrit ensuite la liste des objets 3D ajouter dans la scene
    QList<float> list = gamewindow->getObjectList();

    //en premire on ecrit le nombre d'objet
    flux << list.size() / 5 << endl;

    //puis pour chaque objet on ecrit le type, la position en x,y et z et l'echelle
    for(int i = 0; i < list.size()/5; ++i)
        flux << list.at(i*5) << " " << list.at(i*5+1) << " "
             <<list.at(i*5+2) << " " << list.at(i*5+3) << " " << list.at(i*5+4) << endl;
}

FileManager::FileManager(){

}

void FileManager::loadFile(GameWindow* gamewindow,QString filename){

    QFile file ("..\\TP4\\Sauvegarde\\" + filename +".save");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);
    flux.setCodec("UTF-8");

    QString ligne;

    ligne = flux.readLine();
    int numCarte = ligne.toInt();
    gamewindow->setCarte(numCarte);

    ligne = flux.readLine();
    int day = ligne.toInt();
    gamewindow->setDay(day);

    ligne = flux.readLine();
    int etat = ligne.toInt();
    gamewindow->camera()->set_etat(etat);

    ligne = flux.readLine();
    int rotX = ligne.toFloat();
    gamewindow->camera()->set_rotX(rotX);

    ligne = flux.readLine();
    int rotY = ligne.toFloat();
    gamewindow->camera()->set_rotY(rotY);

    ligne = flux.readLine();
    int ss = ligne.toFloat();
    gamewindow->camera()->set_ss(ss);

    ligne = flux.readLine();
    int nbObjet = ligne.toInt();

    QList<float> list;
    QStringList strList;

    for(int i = 0; i < nbObjet; ++i){

        ligne = flux.readLine();
        strList = ligne.split(" ");

        for(int j = 0; j < strList.size(); ++j)
            list << strList.at(j).toFloat();

    }

    gamewindow->setObjectList(list);


}

FileManager::FileManager(GameWindowShader* gamewindow, QString filename)
{
    QFile file ("..\\TP4\\Sauvegarde\\" + filename +".save");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);
    flux.setCodec("UTF-8");
    flux << gamewindow->getCarte() << endl ;
    flux << gamewindow->day() << endl;
    flux << gamewindow->camera()->get_etat() << endl;
    flux << gamewindow->camera()->get_rotX() << endl;
    flux << gamewindow->camera()->get_rotY() << endl;
    flux << gamewindow->camera()->get_ss() << endl;

    //on ecrit ensuite la liste des objets 3D ajouter dans la scene
    QList<float> list = gamewindow->getObjectList();

    //en premire on ecrit le nombre d'objet
    flux << list.size() / 5 << endl;

    //puis pour chaque objet on ecrit le type, la position en x,y et z et l'echelle
    for(int i = 0; i < list.size()/5; ++i)
        flux << list.at(i*5) << " " << list.at(i*5+1) << " "
             <<list.at(i*5+2) << " " << list.at(i*5+3) << " " << list.at(i*5+4) << endl;
}


void FileManager::loadFile(GameWindowShader* gamewindow,QString filename){

    QFile file ("..\\TP4\\Sauvegarde\\" + filename +".save");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);
    flux.setCodec("UTF-8");

    QString ligne;

    ligne = flux.readLine();
    int numCarte = ligne.toInt();
    gamewindow->setCarte(numCarte);

    ligne = flux.readLine();
    int day = ligne.toInt();
    gamewindow->setDay(day);

    ligne = flux.readLine();
    int etat = ligne.toInt();
    gamewindow->camera()->set_etat(etat);

    ligne = flux.readLine();
    int rotX = ligne.toFloat();
    gamewindow->camera()->set_rotX(rotX);

    ligne = flux.readLine();
    int rotY = ligne.toFloat();
    gamewindow->camera()->set_rotY(rotY);

    ligne = flux.readLine();
    int ss = ligne.toFloat();
    gamewindow->camera()->set_ss(ss);

    ligne = flux.readLine();
    int nbObjet = ligne.toInt();

    QList<float> list;
    QStringList strList;

    for(int i = 0; i < nbObjet; ++i){

        ligne = flux.readLine();
        strList = ligne.split(" ");

        for(int j = 0; j < strList.size(); ++j)
            list << strList.at(j).toFloat();

    }

    gamewindow->setObjectList(list);


}
