#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QPushButton>
#include "serverthread.h"

class ControllerWindow : public QMainWindow {
    Q_OBJECT
public:
    ControllerWindow()
    {
        server = new ServerThread();
        button = new QPushButton();
        this->setCentralWidget(button);
        button->setText("Change season");
        button->resize(100, 60);
        connect(button, SIGNAL(clicked()), server, SLOT(onSeasonChangeRequest()));
    }

public slots:
    void onSaveRequest()
    {
        qDebug() << "sending save request";
        emit requestSave();
    }
    void onLoadRequest()
    {
        qDebug() << "sending load request";
        emit requestLoad();
    }

signals:
    void requestLoad();
    void requestSave();

private:
    ServerThread *server;
    QPushButton *button;
};

#endif // CONTROLLERWINDOW_H
