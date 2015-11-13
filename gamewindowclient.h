#ifndef GAMEWINDOWCLIENT_H
#define GAMEWINDOWCLIENT_H

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

#include "gamewindow.h"

class GameWindowClient : public GameWindow
{
    Q_OBJECT
public:
    GameWindowClient(int fps, Camera * camera, char * name);
    void doConnect();

private:
    QTcpSocket *socket;
    char * name;

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
};

#endif // GAMEWINDOWCLIENT_H
