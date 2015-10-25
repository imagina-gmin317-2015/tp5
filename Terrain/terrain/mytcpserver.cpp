#include "mytcpserver.h"

#include <QFile>
#include <QDataStream>

/**
 * @brief MyTcpServer::MyTcpServer, Constructeur du TcpServer
 * @param cam, pointeur vers la caméra (servant pour la sauvegarde)
 */
MyTcpServer::MyTcpServer(Camera* cam) : camera(cam)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

/**
 * @brief MyTcpServer::start, permet de lancer le serveur.
 * @param timeBetweenSeason, temps entre chaque saison
 */
void MyTcpServer::start(int timeBetweenSeason){
    if(!server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";

        season = id = 0;

        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this, SLOT(sendSeason()));
        timer->start(timeBetweenSeason);

        saveTimer = new QTimer(this);
        connect(saveTimer,SIGNAL(timeout()),this, SLOT(saveGame()));
        saveTimer->start(timeBetweenSeason + 1000);
    }
}

/**
 * @brief MyTcpServer::newConnection, Se déclenche lorsque le serveur reçoit une nouvelle connexion.
 */
void MyTcpServer::newConnection()
{
    // need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();
    if(id < 4){
        clients[id] = socket;
        id++;
    }
}

/**
 * @brief MyTcpServer::sendSeason, envoi une nouvelle saison au fenêtre tout les timeBetweenSaison.
 */
void MyTcpServer::sendSeason()
{
    int tmpSeason = season;
    for(int i = 0 ; i < id ; i++){
        if(tmpSeason == 0)
            clients[i]->write("PRINTEMPS");
        else if(tmpSeason == 1)
            clients[i]->write("ETE");
        else if(tmpSeason == 2)
            clients[i]->write("AUTOMNE");
        else if(tmpSeason == 3)
            clients[i]->write("HIVER");

        clients[i]->flush();

        clients[i]->waitForBytesWritten(3000);

        tmpSeason = (tmpSeason + 1) % 4;
    }

    season++;
    if(season > 3)
        season = 0;
}

/**
 * @brief MyTcpServer::saveGame, sauvegarde la position de la caméra dans le fichier et demande aux fenêtres de sauvegarder leur terrain.
 */
void MyTcpServer::saveGame()
{
    qDebug() << "SAVE";
    bool exist = false;

    QString localPath = FileManager::Instance().localPath;
    QFile file(localPath);

    if(file.exists()){
        exist = true;
    }

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "MyTcpServer(saveGame) : Could not open " << localPath;
        return;
    }

    if(exist){
        QFile::remove("./game_save.txt");
        QFile save("./game_save.txt");
        save.open(QIODevice::ReadWrite);
        QDataStream in(&save);
        in.setVersion(QDataStream::Qt_5_1);
        in << file.readAll();
    }

    file.resize(0);

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);

    out << camera->etat << camera->rotX << camera->rotY << camera->ss;

    file.flush();
    file.close();

    for(int i = 0 ; i < id ; i++){
        clients[i]->write("SAVE");
        clients[i]->flush();
        clients[i]->waitForBytesWritten(3000);
    }
}
