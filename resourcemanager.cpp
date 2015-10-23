#include "resourcemanager.h"

QVector<QString> ResourceManager::toSaveSaves;
QVector<QString> ResourceManager::loadedSaves;

QString ResourceManager::serialize(QVector<data *> vec)
{
    QString s = "";
    for (int i = 0; i < vec.size(); ++i) {
        s += vec[i]->toString();
    }
    return s;
}

QVector<data *> ResourceManager::parse(QString s)
{
    qDebug() << "parsing " << s;
    QVector<data *> vec;
    QStringList list = s.split(";");
    for (int i = 0; i < list.size(); ++i) {
        vec.push_back(dataFromType(list.at(i)));
    }
    return vec;
}

void ResourceManager::assign(QVector<data *> toAssign, QVector<data *> values)
{
    for (int i = 0; i < toAssign.size(); ++i) {
        data *d = values[i];
        switch (d->type) {
        case _int:
            *((int*)(toAssign[i]->valuePtr)) = *((int*)(d->valuePtr));
            break;
        case _float:
            *((float*)(toAssign[i]->valuePtr)) = *((float*)(d->valuePtr));
            break;
        case _bool:
            *((bool*)(toAssign[i]->valuePtr)) = *((bool*)(d->valuePtr));
            break;
        default:
            break;
        }
    }
}

data *ResourceManager::FLOAT(float *value)
{
    data *d = new data();
    d->valuePtr = value;
    d->type = _float;
    d->toString = [=]() {
        QString s = "FLOAT:";
        return  s + QString::number(*value) + ";";
    };
    return d;
}

data *ResourceManager::INT(int *value)
{
    data *d = new data();
    d->valuePtr = value;
    d->type = _int;
    d->toString = [=]() {
        QString s = "INT:";
        return  s + QString::number(*value) + ";";
    };
    return d;
}

data *ResourceManager::BOOL(bool *value)
{
    data *d = new data();
    d->valuePtr = value;
    d->type = _bool;
    d->toString = [=]() {
        QString s = "BOOL:";
        return  s + ((*value) ? "true" : "false") + ";";
    };
    return d;
}

void ResourceManager::loadSave()
{
    QString filePath = "./save.dat";
    QFile file( filePath );
    if(file.size() < 3) return;
    if ( file.open(QIODevice::ReadOnly) )
    {
        QDataStream stream( &file );
        QString s;
        forever {
            stream >> s;
            if (s.size() < 3) return;
            loadedSaves.push_back(s);
        }
    }
}

QString ResourceManager::getSave(int saveIndex)
{
    return loadedSaves.at(saveIndex);
}

void ResourceManager::setSave(QString save, int saveIndex)
{
    qDebug() << "setting save";
    toSaveSaves[saveIndex] = save;

    for (int i = 0; i < toSaveSaves.size(); ++i) {
        if(toSaveSaves[i].size() < 3) return;
    }

    saveSaves();
    toSaveSaves.clear();
    for (int i = 0; i < 4; ++i) {
        toSaveSaves.push_back("");
    }
}

void ResourceManager::init()
{
    for (int i = 0; i < 4; ++i) {
        toSaveSaves.push_back("");
    }
    loadSave();
}

data *ResourceManager::dataFromType(QString s)
{
    QStringList l = s.split(":");
    if(l.at(0).compare("FLOAT") == 0) {
        float *f = new float[1];
        f[0] = l.at(1).toFloat();
        return FLOAT(f);
    } else if (l.at(0).compare("INT") == 0) {
        int *f = new int[1];
        f[0] = l.at(1).toInt();
        return INT(f);
    } else if (l.at(0).compare("BOOL") == 0) {
        bool *f = new bool[1];
        f[0] = (l.at(1).compare("true") == 0) ? true : false;
        return BOOL(f);
    }
}

void ResourceManager::saveSaves()
{
    QFile file("./save.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    for (int i = 0; i < toSaveSaves.size(); ++i) {
        qDebug() << toSaveSaves[i];
        out << toSaveSaves[i];
    }
}
