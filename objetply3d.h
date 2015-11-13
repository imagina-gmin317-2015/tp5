#ifndef OBJETPLY3D_H
#define OBJETPLY3D_H

#include <QString>

class ObjetPly3D
{   
private:

    QString m_pathToTheFile;

    float * m_points;
    int * m_faces;

    int m_nbPoints;
    int m_nbFace;

    float m_posX;
    float m_posY;
    float m_posZ;

public:
    ObjetPly3D(QString pathToTheFile);
    ObjetPly3D();

    void renderPoints(float posX, float posY, float posZ, float scale);
    void renderFace(float posX, float posY, float posZ, float scale);

    /** Getteurs et Setteurs **/

    QString pathToTheFile() const;
    void setPathToTheFile(const QString &pathToTheFile);

    float *points() const;
    void setPoints(float *points);

    int *faces() const;
    void setFaces(int *faces);

    int nbPoints() const;
    void setNbPoints(int nbPoints);

    int nbFace() const;
    void setNbFace(int nbFace);

    float posX() const;
    void setPosX(float posX);

    float posY() const;
    void setPosY(float posY);

    float posZ() const;
    void setPosZ(float posZ);


};

#endif // OBJETPLY3D_H
