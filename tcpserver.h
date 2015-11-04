#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QTimer>
#include <iostream>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

signals:

public slots:
    void newConnection();
    void sendSeason();

private:
    QTcpServer *server;
    QTcpSocket **clients;
    QTimer *timer;
    int nbConnected;

};

#endif // TCPSERVER_H
