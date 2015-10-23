#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QObject>

class ClientThread : public QThread
{
    Q_OBJECT
public:
    ClientThread();
    void run();
    static void init(ClientThread *client);

public slots:
    void onData();
    void onError(QAbstractSocket::SocketError e);

signals:
    void seasonChangeSignal();

private:
    int blockSize;
    QTcpSocket socket;
};

#endif // CLIENTTHREAD_H
