#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

#include <vector>

#include "gameobject.h"

using namespace std;

/**
 * @brief GameObject::GameObject, Constructeur d'un GameObject.
 */
GameObject::GameObject()
{
    position = QVector3D(0.f,0.f,0.f);
    scale = QVector3D(0.f,0.f,0.f);
    rotation = QVector3D(0.f,0.f,0.f);

    nb_vertex = 0;
    vertex = new QVector3D[nb_vertex];
    normals = new QVector3D[nb_vertex];

    nb_ind = 0;
    index = new QVector3D[nb_ind];
}

/**
 * @brief GameObject::GameObject, Constructeur d'un GameObject.
 * @param pos, position de l'objet
 * @param rot, rotation de l'objet
 * @param _scale, échelle de l'objet
 */
GameObject::GameObject(QVector3D pos, QVector3D rot, QVector3D _scale)
{
    position = pos;
    rotation = rot;
    scale = _scale;

    nb_vertex = 0;
    vertex = new QVector3D[nb_vertex];
    normals = new QVector3D[nb_vertex];

    nb_ind = 0;
    index = new QVector3D[nb_ind];
}

/**
 * @brief GameObject::GameObject, Constructeur d'un GameObject.
 * @param pos, position de l'objet
 * @param rot, rotation de l'objet
 * @param _scale, échelle de l'objet
 * @param _localPath, chemin de l'objet à charger
 */
GameObject::GameObject(QVector3D pos, QVector3D rot, QVector3D _scale, QString _localPath)
{
    position = pos;
    rotation = rot;
    scale = _scale;

    open(_localPath);
}

/**
 * @brief GameObject::open, permet d'ouvrir l'objet dont le chemin est passé en paramètre.
 * @param _localPath, chemin de l'objet
 */
void GameObject::open(QString _localPath)
{

    QFile file(_localPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open " << _localPath;
        return;
    }

    QString line = file.readLine();

    if(line.contains("ply")){
        file.close();
        openPLY(_localPath);
    }else if(line.contains("solid")){
        file.close();
        openSTL(_localPath);
    }else if(line.contains("OBJ")){
        file.close();
        openOBJ(_localPath);
    }

}

/**
 * @brief GameObject::display, Affiche le GameObject en fonction de son type.
 */
void GameObject::display()
{
    glPushMatrix();

    glTranslatef(position.x(), position.y(), position.z());
    glScalef(scale.x(), scale.y(), scale.z());
    glRotatef(rotation.x(), 1.f, 0.f, 0.f);
    glRotatef(rotation.y(), 0.f, 0.f, 1.f);
    glRotatef(rotation.z(), 0.f, 1.f, 0.f);

    glColor3f(0.f,0.5f,0.f);

    if(format == Format::PLY){
        displayPLY();
    }else if(format == Format::STL){
        displaySTL();
    }else if(format == Format::OBJ){
        displayOBJ();
    }

    glPopMatrix();
}

/**
 * @brief GameObject::openPLY, permet d'ouvrir les objets de type PLY.
 * @param _localPath, chemin de l'objet
 */
void GameObject::openPLY(QString _localPath)
{
    QFile file(_localPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open " << _localPath;
        return;
    }

    localPath = _localPath;
    format = Format::PLY;

    // read whole content
    QString content = file.readAll();
    // extract words
    QStringList lines = content.split("\n");

    file.close();

    int ind = 0;

    for(int i = 0 ; i < lines.size() ; i++){
        if(lines[i].contains("element vertex")){
            QStringList words = lines[i].split(" ");
            nb_vertex = words[2].toInt();
        }else if(lines[i].contains("element face")){
            QStringList words = lines[i].split(" ");
            nb_ind = words[2].toInt();
        }

        if(lines[i].contains("end_header")){
            ind = i+1;
            break;
        }
    }

    vertex = new QVector3D[nb_vertex];
    normals = new QVector3D[nb_vertex];
    index = new QVector3D[nb_ind];

    int i = 0;
    while(i < nb_vertex){
        QStringList words = lines[ind + i].split(" ");

        vertex[i].setX(words[0].toFloat());
        vertex[i].setY(words[1].toFloat());
        vertex[i].setZ(words[2].toFloat());

        normals[i].setX(words[3].toFloat());
        normals[i].setY(words[4].toFloat());
        normals[i].setZ(words[5].toFloat());

        i++;
    }

    ind += i;
    i = 0;
    while(i < nb_ind){
        QStringList words = lines[ind + i].split(" ");

        index[i].setX(words[1].toInt());
        index[i].setY(words[2].toInt());
        index[i].setZ(words[3].toInt());

        i++;
    }

}

/**
 * @brief GameObject::openSTL, permet d'ouvrir les objets de type STL.
 * @param _localPath, chemin de l'objet
 */
void GameObject::openSTL(QString _localPath)
{
    QFile file(_localPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open " << _localPath;
        return;
    }

    localPath = _localPath;
    format = Format::STL;

    // read whole content
    QString content = file.readAll();
    // extract words
    QStringList lines = content.split("\n");

    file.close();

    vector<QVector3D> v_normals;
    vector<QVector3D> v_vertex;

    for(int i = 0 ; i < lines.size() ; i++){
        if(lines[i].contains("facet normals")){
            QStringList words = lines[i].split(" ");
            v_normals.push_back(QVector3D(words[2].toFloat(), words[3].toFloat(), words[4].toFloat()));
        }else if(lines[i].contains("vertex")){
            QStringList words = lines[i].split(" ");
            v_vertex.push_back(QVector3D(words[1].toFloat(), words[2].toFloat(), words[3].toFloat()));
        }
    }

    nb_vertex = v_vertex.size();

    vertex = new QVector3D[nb_vertex];
    normals = new QVector3D[v_normals.size()];

    for(unsigned int i = 0 ; i < v_vertex.size() ; i++){
        vertex[i].setX(v_vertex[i].x());
        vertex[i].setY(v_vertex[i].y());
        vertex[i].setZ(v_vertex[i].z());
    }

    for(unsigned int i = 0 ; i < v_normals.size() ; i++){
        normals[i] = v_normals[i];
    }
}

/**
 * @brief GameObject::openOBJ, permet d'ouvrir les objets de type OBJ.
 * @param _localPath, chemin de l'objet
 */
void GameObject::openOBJ(QString _localPath)
{
    QFile file(_localPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open " << _localPath;
        return;
    }

    localPath = _localPath;
    format = Format::OBJ;

    // read whole content
    QString content = file.readAll();
    // extract words
    QStringList lines = content.split("\n");

    file.close();

    vector<QVector3D> v_index;
    vector<QVector3D> v_vertex;

    for(int i = 4 ; i < lines.size() ; i++){
        if(lines[i].contains("v ")){
            QStringList words = lines[i].split(" ");
            v_vertex.push_back(QVector3D(words[1].toFloat(), words[2].toFloat(), words[3].toFloat()));
        }else if(lines[i].contains("f ")){
            QStringList words = lines[i].split(" ");
            v_index.push_back(QVector3D(words[1].toInt(), words[2].toInt(), words[3].toInt()));
        }
    }

    nb_vertex = v_vertex.size();
    nb_ind = v_index.size();

    vertex = new QVector3D[nb_vertex];
    index = new QVector3D[nb_ind];

    for(int i = 0 ; i < nb_vertex ; i++){
        vertex[i].setX(v_vertex[i].x());
        vertex[i].setY(v_vertex[i].y());
        vertex[i].setZ(v_vertex[i].z());
    }

    for(int i = 0 ; i < nb_ind ; i++){
        index[i].setX(v_index[i].x());
        index[i].setY(v_index[i].y());
        index[i].setZ(v_index[i].z());
    }
}

/**
 * @brief GameObject::displayPLY, permet d'afficher les objets de type PLY.
 */
void GameObject::displayPLY()
{
    glBegin(GL_TRIANGLES);
    #pragma omp for schedule(dynamic)
    for(int i = 0 ; i < nb_ind ; i++){
        glNormal3f(normals[(int)index[i].x()].x(), normals[(int)index[i].x()].y(), normals[(int)index[i].x()].z());
        glVertex3f(vertex[(int)index[i].x()].x(), vertex[(int)index[i].x()].y(), vertex[(int)index[i].x()].z());

        glNormal3f(normals[(int)index[i].y()].x(), normals[(int)index[i].y()].y(), normals[(int)index[i].y()].z());
        glVertex3f(vertex[(int)index[i].y()].x(), vertex[(int)index[i].y()].y(), vertex[(int)index[i].y()].z());

        glNormal3f(normals[(int)index[i].z()].x(), normals[(int)index[i].z()].y(), normals[(int)index[i].z()].z());
        glVertex3f(vertex[(int)index[i].z()].x(), vertex[(int)index[i].z()].y(), vertex[(int)index[i].z()].z());

    }
    glEnd();
}

/**
 * @brief GameObject::displaySTL, permet d'afficher les objets de type STL.
 */
void GameObject::displaySTL()
{
    unsigned int j = 0;

    glBegin(GL_TRIANGLES);
    #pragma omp for schedule(dynamic)
    for(int i = 0 ; i < nb_vertex ; i+=3){
        glNormal3f(normals[j].x(), normals[j].y(), normals[j].z());
        glVertex3f(vertex[i].x(), vertex[i].y(), vertex[i].z());
        glVertex3f(vertex[i+1].x(), vertex[i+1].y(), vertex[i+1].z());
        glVertex3f(vertex[i+2].x(), vertex[i+2].y(), vertex[i+2].z());
        j++;
    }
    glEnd();
}

/**
 * @brief GameObject::displayOBJ, permet d'afficher les objets de type OBJ.
 */
void GameObject::displayOBJ()
{
    glBegin(GL_TRIANGLES);
    #pragma omp for schedule(dynamic)
    for(int i = 0 ; i < nb_ind ; i++){
        glVertex3f(vertex[(int)index[i].x()].x(), vertex[(int)index[i].x()].y(), vertex[(int)index[i].x()].z());
        glVertex3f(vertex[(int)index[i].y()].x(), vertex[(int)index[i].y()].y(), vertex[(int)index[i].y()].z());
        glVertex3f(vertex[(int)index[i].z()].x(), vertex[(int)index[i].z()].y(), vertex[(int)index[i].z()].z());
    }
    glEnd();
}

