#include "gamewindowclient.h"
#include <iostream>

using namespace std;

GameWindowClient::GameWindowClient(int fps, Camera * camera, char * name)
    : GameWindow(fps, camera)
{
    this->name = name;
    this->doConnect();
}

void GameWindowClient::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost("127.0.0.1", 9999);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }

    socket->waitForReadyRead();
    socket->waitForDisconnected();
}

void GameWindowClient::connected()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void GameWindowClient::disconnected()
{
    qDebug() << "disconnected...";
}

void GameWindowClient::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void GameWindowClient::readyRead()
{
    //qDebug() << name;
    //qDebug() << "reading...";

    // read the data from the socket
    qDebug() << name << " : " << socket->readAll();
}

