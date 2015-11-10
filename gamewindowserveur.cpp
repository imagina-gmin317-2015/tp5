#include "gamewindowserveur.h"

#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <QTimer>

using namespace std;

GameWindowServeur::GameWindowServeur(int fps, Camera* camera)
                    :GameWindow(fps,camera)
{
    m_numNextClient = 0;
    m_indiceSeason = 0;

    m_seasons[0] = "hiver";
    m_seasons[1] = "printemps";
    m_seasons[2] = "ete";
    m_seasons[3] = "automne";

    m_server = new QTcpServer(this);

    m_timer = new QTimer(this);
    m_timer->connect(m_timer, SIGNAL(timeout()),this, SLOT(sendSeason()));
    m_timer->start(5000);

    // whenever a user connects, it will receive sendSeason
    connect(m_server, SIGNAL(newConnection()),
            this, SLOT(recupConnection()));

    if(!m_server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }

}

GameWindowServeur::recupConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();

    socket->write("Hello client");
    socket->flush();

    m_clients[m_numNextClient++] = socket;

    //socket->close();
}

GameWindowServeur::sendSeason()
{
    /*
    m_clients[0]->open(QIODevice::WriteOnly);
    m_clients[0]->write(m_seasons[m_indiceSeason % 4]);
    m_clients[0]->close();
    m_clients[1]->open(QIODevice::WriteOnly);
    m_clients[1]->write(m_seasons[m_indiceSeason+1 % 4]);
    m_clients[1]->close();
    m_clients[2]->open(QIODevice::WriteOnly);
    m_clients[2]->write(m_seasons[m_indiceSeason+2 % 4]);
    m_clients[2]->close();
    m_clients[3]->open(QIODevice::WriteOnly);
    m_clients[3]->write(m_seasons[m_indiceSeason+3 % 4]);
    m_clients[3]->close();
    */

    for(int i = 0; i < m_numNextClient; ++i){

        //m_clients[i]->open(QIODevice::WriteOnly);
        m_clients[i]->write(m_seasons[((m_indiceSeason+i) % 4)]);
        m_clients[i]->flush();
        //m_clients[i]->close();
    }

    m_indiceSeason++;
}
