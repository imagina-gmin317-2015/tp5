#include "gamewindowclient.h"


GameWindowClient::GameWindowClient(int fps, Camera* camera, char * name)
                    :GameWindow(fps,camera)
{
    m_name = name;
    m_socket = new QTcpSocket(this);

    connect(m_socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(m_socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(m_socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
    m_socket->connectToHost("127.0.0.1", 9999);

    // we need to wait...
    if(!m_socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << m_socket->errorString();
    }

    m_socket->waitForReadyRead(5000);

    m_socket->waitForDisconnected(300000);
}

void GameWindowClient::connected()
{
    qDebug() <<  m_name << "connected...";

    // Hey server, tell me about you.
    m_socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void GameWindowClient::disconnected()
{
    qDebug() <<  m_name << "disconnected...";
}

void GameWindowClient::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void GameWindowClient::readyRead()
{
    // read the data from the socket
    qDebug() << m_name << " reading : "<< m_socket->readAll() << "\n";

}
