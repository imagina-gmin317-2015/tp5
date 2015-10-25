#include <QFile>
#include <QDataStream>
#include <QMap>
#include <iostream>
#include <QDebug>

#include "filemanager.h"
#include "gamewindow.h"
#include "terrain.h"
#include "camera.h"

using namespace std;

/**
 * @brief FileManager::m_instance, instance singleton de la classe FileManager.
 */
FileManager FileManager::m_instance = FileManager();

/**
 * @brief FileManager::id, identifiant des différents Terrains chargés.
 */
int FileManager::id = 0;

/**
 * @brief FileManager::FileManager, constructeur FileManager.
 */
FileManager::FileManager()
{
    T = new Terrain*[NB_TERRAIN];
    for(int i = 0 ; i < NB_TERRAIN ; i++){
        T[i] = new Terrain();
    }

    camera = nullptr;
    cout<<"Creation"<<endl;
}

FileManager::~FileManager()
{
    cout<<"Destruction"<<endl;
}

/**
 * @brief FileManager::Instance, permet d'accéder à l'instance de FileManager.
 * @return m_instance
 */
FileManager& FileManager::Instance()
{
    return m_instance;
}

/**
 * @brief FileManager::saveCustomMap, sauvegarde le Terrain passé en paramètre dans le fichier binaire unique.
 * @param T, Terrain permettant d'accéder aux différentes informations de celui-ci
 */
void FileManager::saveCustomMap(Terrain* T)
{
    QFile file(localPath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug() << "FileManager : Could not open " << localPath;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);

    out << T->saison << T->c.r << T->c.g << T->c.b << T->nb_vertex_width << T->nb_vertex_height;
    for(int i = 0 ; i < T->nb_vertex_width * T->nb_vertex_height ; i++){
        out << T->vertex[i].x() << T->vertex[i].y() << T->vertex[i].z();
    }

    out << FileManager::NB_TERRAIN << GameWindow::NB_ARBRES;

    for(unsigned int j = 0 ; j < NB_TERRAIN ; j++){
        for(unsigned int i = 0 ; i < GameWindow::NB_ARBRES ; i++){
            out << T->tree[j][i]->localPath;
            out << T->tree[j][i]->position.x() << T->tree[j][i]->position.y() << T->tree[j][i]->position.z();
            out << T->tree[j][i]->rotation.x() << T->tree[j][i]->rotation.y() << T->tree[j][i]->rotation.z();
            out << T->tree[j][i]->scale.x() << T->tree[j][i]->scale.y() << T->tree[j][i]->scale.z();
        }
    }

    file.flush();
    file.close();
}

/**
 * @brief FileManager::loadCustomMap, charge le fichier binaire à partir du chemin passé en paramètre et constitue les différents terrains.
 * @param _localPath, chemin vers le fichier binaire
 */
void FileManager::loadCustomMap(QString _localPath)
{
    localPath = _localPath;

    QFile file(localPath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "FileManager : Could not open " << localPath;
        return;
    }

    if(file.size() == 0){
        qDebug() << "FileManager : Empty file";
        file.close();
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);

    QString saison;
    color c;
    int nb_vertex_w;
    int nb_vertex_h;
    QVector3D* vertex;

    GameObject*** tree;

    camera = new Camera();
    in >> camera->etat >> camera->rotX >> camera->rotY >> camera->ss;

    for(int t = 0 ; t < NB_TERRAIN ; t++){

        in >> saison;
        in >> c.r >> c.g >> c.b;
        in >> nb_vertex_w >> nb_vertex_h;

        vertex = new QVector3D[nb_vertex_w * nb_vertex_h];
        for(int i = 0 ; i < nb_vertex_w * nb_vertex_h ; i++){
            float x,y,z;
            in >> x >> y >> z;
            vertex[i].setX(x);
            vertex[i].setY(y);
            vertex[i].setZ(z);
        }
        int nb_terrains, nb_arbres;
        in >> nb_terrains >> nb_arbres;

        tree = new GameObject**[nb_terrains];
        QString localPathTree;
        QVector3D position;
        QVector3D rotation;
        QVector3D scale;
        float x,y,z;

        for(int j = 0 ; j < nb_terrains ; j++){
            tree[j] = new GameObject*[nb_arbres];

            for(int i = 0 ; i < nb_arbres ; i++){

                in >> localPathTree;

                in >> x >> y >> z;
                position.setX(x);
                position.setY(y);
                position.setZ(z);

                in >> x >> y >> z;
                rotation.setX(x);
                rotation.setY(y);
                rotation.setZ(z);

                in >> x >> y >> z;
                scale.setX(x);
                scale.setY(y);
                scale.setZ(z);

                tree[j][i] = new GameObject(position, rotation, scale, localPathTree);
            }
        }

        T[t] = new Terrain(saison, c, nb_vertex_w, nb_vertex_h, vertex, tree);
    }
    file.flush();
    file.close();
}

/**
 * @brief FileManager::getTerrain, permet aux fenêtres de récupèrer le terrain en fonction de l'id courant
 * @return un Terrain
 */
Terrain *FileManager::getTerrain()
{
    if(FileManager::id > NB_TERRAIN)
        return new Terrain();

    FileManager::id++;
    return T[id-1];

}

