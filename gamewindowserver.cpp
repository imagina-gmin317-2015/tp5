#include "gamewindowserver.h"
#include <QTimer>

using namespace std;

GameWindowServer::GameWindowServer(int fps, Camera *camera)
    : GameWindow(fps, camera)
{
    server = new QTcpServer(this);
    numClient = 0;
    indiceSeason = 0;
    seasons[0] = "hiver";
    seasons[1] = "printemps";
    seasons[2] = "été";
    seasons[3] = "automne";

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(getPendingConnection()));

    if(!server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }

    m_timer = new QTimer(this);
    m_timer->connect(m_timer, SIGNAL(timeout()),this, SLOT(sendSeason()));

}

void GameWindowServer::getPendingConnection()
{
    qDebug() << "getPendingConnection";
    this->tabSocket[numClient%4] = server->nextPendingConnection();
    ++numClient;

    m_timer->start(5000);
}

void GameWindowServer::sendSeason()
{
    qDebug() << "\n SEND SEASON -> ";
    //socket->write("Hello client\r\n");
    for(int i = 0; i < numClient; ++i)
    {
       // qDebug() << "boucle" << seasons[indiceSeason%4];
        //tabSocket[i]->open(QIODevice::WriteOnly);
        tabSocket[i]->write(seasons[indiceSeason%4]);

        ++indiceSeason;
        tabSocket[i]->flush();
        //tabSocket[i]->waitForBytesWritten(5000);
        //tabSocket[i]->close();
    }

    ++indiceSeason;

    //socket->waitForBytesWritten(3000);
}

