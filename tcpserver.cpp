#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    clients = new QTcpSocket*[4];
    nbConnected = 0;

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSeason()));
    timer->start(20000);

    if(!server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void TcpServer::newConnection()
{
    if(nbConnected >= 4)
    {
        qDebug() << "Nombre max de connecté atteint";
        return;
    }

    // need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();

    clients[nbConnected] = socket;

    socket->write(QByteArray::number(nbConnected));
    socket->flush();

    nbConnected++;
}

void TcpServer::sendSeason()
{
    for(int i = 0; i < 4; i++)
    {
        clients[i]->write("nextSeason");
        clients[i]->flush();
    }
}

