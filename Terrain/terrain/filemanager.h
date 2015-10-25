#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>

class Terrain;
class Camera;

class FileManager
{
public:
    static FileManager& Instance();

    void saveCustomMap(Terrain* T);
    void loadCustomMap(QString localPath);

    Terrain* getTerrain();
public:
    static const int NB_TERRAIN = 4;
    QString localPath;
    Camera* camera;

private:
    static FileManager m_instance;
    static int id;
    FileManager();
    ~FileManager();

    Terrain** T;

};

#endif // FILEMANAGER_H
