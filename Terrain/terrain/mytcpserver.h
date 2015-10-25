#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

#include <QTimer>

#include "camera.h"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(Camera* cam);
    void start(int timeBetweenSeason);

signals:

public slots:
    void newConnection();
    void sendSeason();
    void saveGame();

private:
    Camera* camera;

    QTcpServer* server;
    QTcpSocket* clients[4];
    int id;
    int season;

    QTimer* timer, *saveTimer;
};

#endif // MYTCPSERVER_H
