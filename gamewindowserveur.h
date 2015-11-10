#ifndef GAMESERVEUR_H
#define GAMESERVEUR_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

#include "gamewindow.h"


class GameWindowServeur : public GameWindow
{
    Q_OBJECT
public:
    GameWindowServeur(int fps, Camera* camera);

private:
    int m_indiceSeason;
    int m_numNextClient;
    char * m_seasons[4];
    QTcpSocket* m_clients[4];
    QTcpServer* m_server;
    QTimer * m_timer;

public slots:
    recupConnection();
    sendSeason();

};

#endif // GAMESERVEUR_H
