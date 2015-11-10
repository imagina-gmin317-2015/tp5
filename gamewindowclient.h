#ifndef GAMEWINDOWCLIENT_H
#define GAMEWINDOWCLIENT_H


#include <QTcpSocket>
#include <QAbstractSocket>
#include <QObject>

#include "gamewindow.h"


class GameWindowClient : public GameWindow
{
    Q_OBJECT


public:
    GameWindowClient(int fps, Camera* camera, char* name);

private:
    char * m_name;
    QTcpSocket *m_socket;

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
};

#endif // GAMEWINDOWCLIENT_H
