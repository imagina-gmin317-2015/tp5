#include "mytcpserver.h"

#include <QFile>
#include <QDataStream>

/**
 * @brief MyTcpServer::MyTcpServer, Constructeur du TcpServer
 * @param cam, pointeur vers la caméra (servant pour la sauvegarde)
 */
MyTcpServer::MyTcpServer()
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
