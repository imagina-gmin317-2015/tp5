#ifndef GAMEWINDOWSERVER_H
#define GAMEWINDOWSERVER_H

#include <QTcpSocket>
#include <QTcpServer>

#include "gamewindow.h"


class GameWindowServer : public GameWindow
{
    Q_OBJECT
public:
    GameWindowServer(int fps, Camera *camera);

private:
    QTcpServer * server;
    QTimer * m_timer;
    QTcpSocket  * tabSocket[4];
    int numClient;
    char * seasons[4];
    int indiceSeason;

signals:

public slots:
    void getPendingConnection();
    void sendSeason();

};

#endif // GAMEWINDOWSERVER_H
