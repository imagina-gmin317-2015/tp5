#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <functional>
#include <QDebug>

#include <iostream>

#include <QtCore>
#include <QtGui>

const int _int = 0;
const int _float = 1;
const int _bool = 2;

struct data {
    void *valuePtr;
    std::function<QString()> toString;
    int type;
};

class ResourceManager
{
public:
    static QString serialize(QVector<data*> vec);
    static QVector<data*> parse(QString s);
    static void assign(QVector<data*> toAssign, QVector<data*> values);

    static data *FLOAT(float *value);
    static data *INT(int *value);
    static data *BOOL(bool *value);

    static void loadSave();
    static QString getSave(int saveIndex);

    static void setSave(QString save, int saveIndex);

    static void init();

private:
    static data *dataFromType(QString s);
    static void saveSaves();

    static QVector<QString> loadedSaves;
    static QVector<QString> toSaveSaves;
};

#endif // RESOURCEMANAGER_H
