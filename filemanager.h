#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "gamewindow.h"

class FileManager
{

public:
    FileManager(GameWindow* game);
    void saveData();
    void readData();

    void save3dObjects(PlyObject* tab3dObjects, int* tabNbArbresParType, int sizeOfTab3d, float* tabPos,
                       float* tabTaille, int nbArbreTotal, int* tabColor, float* tabRotation);
    PlyObject* read3dObjects();

    int carte() const;
    void setCarte(int carte);

    int day() const;
    void setDay(int day);

    int etat() const;
    void setEtat(int etat);

    float rotX() const;
    void setRotX(float rotX);

    float rotY() const;
    void setRotY(float rotY);

    float ss() const;
    void setSs(float ss);

    int         getNb3dObject() const;
    PlyObject*  getTab3dObjects() const;
    int*        getTabArbresParType() const;
    int         getSizeOfTab3d() const;
    float*      getTabPos() const;
    float*      getTabTaille() const;
    int*        getTabColor() const;
    float*      getTabRotation() const;

private:
    QString   m_fileName;
    int     m_carte;
    int     m_day;
    int     m_etat;
    float   m_rotX;
    float   m_rotY;
    float   m_ss;

    int         m_nb3dObject;
    PlyObject*  m_tab3dObjects;
    int*        m_tabArbresParType;
    int         m_sizeOfTab3d;
    float*      m_tabPos;
    int*        m_tabColor;
    float*      m_tabTaille;
    float*      m_tabRotation;
};

#endif // FILEMANAGER_H
