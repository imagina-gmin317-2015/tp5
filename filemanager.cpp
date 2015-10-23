#include "filemanager.h"

FileManager *FileManager::file_instance = new FileManager();

FileManager::FileManager()
{
    this->file = new QFile("save.txt");
    file->open(QIODevice::ReadWrite);

}
void FileManager::setTriangle(QVector<TriangleWindow *> tr)
{
    this->tr = tr;
}

FileManager *FileManager::Instance()
{
    return file_instance;
}

void FileManager::save()
{
    QTextStream temp(this->file);
    for (int i = 0; i < tr.size(); ++i) {
        if(i == tr.size()-1){
            temp <<tr.at(i)->toSave();
        }else{
            temp <<tr.at(i)->toSave()<< "|";
        }

    }
    temp.flush();
}

void FileManager::addWindow(TriangleWindow *tr)
{
    this->tr.push_back(tr);
}

/*
 * Format de sauvegarde :  anim;etat;rotX;rotY;ss;nbSaison;nbDay
 */
void FileManager::load()
{
    QFile fichier("save.txt");
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);

    QString save = flux.readAll();
    QStringList listFenetre = save.split("|");
    QStringList listParam;
    for(int i=0;i<listFenetre.size();i++){
        listParam = listFenetre.at(i).split(";");
        // TODO : charger les informations dans les tr
        tr.at(i)->loadWindow(listParam.at(0).toFloat(),
                             listParam.at(1).toInt(),
                             listParam.at(2).toFloat(),
                             listParam.at(3).toFloat(),
                             listParam.at(4).toFloat(),
                             listParam.at(5).toInt(),
                             listParam.at(6).toInt());
    }
}
