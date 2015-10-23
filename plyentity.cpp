#include "plyentity.h"

PlyEntity *PlyEntity::load(QString filePath)
{
    qDebug() << "loading " << filePath;
    QFile file( filePath );
    if(file.size() < 10) {
        qDebug() << "file not found : " << filePath;
        return nullptr;
    }
    if ( file.open(QIODevice::ReadOnly) )
    {
        QTextStream stream( &file );
        return new PlyEntity(stream.readAll().split("\n"));
    }
    qDebug() << "could not create entity";
}

PlyEntity *PlyEntity::copy(PlyEntity *entity)
{
    PlyEntity *newEntity = new PlyEntity();
    newEntity->indexes = entity->indexes;
    newEntity->vertices = entity->vertices;
    newEntity->verticesArray = entity->verticesArray;
    newEntity->normalsArray = entity->normalsArray;
    newEntity->colorsArray = entity->colorsArray;
    newEntity->scaledVerticesArray = entity->scaledVerticesArray;
    return newEntity;
}

void PlyEntity::draw(float delta)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0, 0, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, scaledVerticesArray.constData());
    glNormalPointer(GL_FLOAT, 0, normalsArray.constData());
    glColorPointer(3, GL_FLOAT, 0, colorsArray.constData());
    glDrawArrays(GL_TRIANGLES, 0, verticesArray.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    //            glBegin(GL_TRIANGLES);
    //            for (int i = 0; i < indexes.size(); ++i) {
    //                QVector<int> v = indexes[i];
    //        #pragma omp for schedule(dynamic)
    //                for (int j = 0; j < v.size(); ++j) {
    //                    point p = vertices[v[j] * 3];
    //                    point n = vertices[v[j] * 3 + 1];
    //                    point c = vertices[v[j] * 3 + 2];
    //                    glColor3f(c.x, c.y, c.z);
    //                    glNormal3f(n.y, n.z, n.x);
    //                    glVertex3f(p.x * scale, p.y * scale, p.z * scale);
    //                }

    //            }
    //            glEnd();
    glPopMatrix();
}

void PlyEntity::update(float delta)
{

}

void PlyEntity::setPosition(float x, float y, float z)
{
    this->x = x; this->y = y; this->z = z;
}

void PlyEntity::setScale(float scale)
{
    this->scale = scale;

    for (int i = 0; i < verticesArray.size(); ++i) {
        QVector3D v = verticesArray[i];
        v *= scale;
        scaledVerticesArray.push_back(v);
    }

}

void PlyEntity::setAngle(float angle)
{
    this->angle = angle;
}

float PlyEntity::getX() const
{
    return this->x;
}

float PlyEntity::getY() const
{
    return this->y;
}

float PlyEntity::getZ() const
{
    return this->z;
}

float PlyEntity::getAngle() const
{
    return this->angle;
}

PlyEntity::PlyEntity(QStringList list)
{
    qDebug() << list.at(0);
    this->scale = 0.01;
    if(!list.at(0).contains("ply")) {
        qDebug() << "bad file";
        return;
    }
    int vertexCount = 0, faceCount = 0;
    int cpt = 0;

    while(vertexCount == 0) {
        if(list.at(cpt).startsWith("element vertex")) {
            vertexCount = list.at(cpt).split(" ").last().toInt();
        }
        cpt++;
    }

    while(faceCount == 0) {
        if(list.at(cpt).startsWith("element face")) {
            faceCount = list.at(cpt).split(" ").last().toInt();
        }
        cpt++;
    }

    while(list.at(cpt++).compare("end_header") != 0);

    qDebug() << "vertex count = " << vertexCount;
    qDebug() << "cpt = " << cpt;
    for (int i = cpt; i < vertexCount + cpt; ++i) {
        QStringList l = list.at(i).split(" ");
        point p, n, c;
        p.x = l.at(0).toFloat();
        p.y = l.at(1).toFloat();
        p.z = l.at(2).toFloat();
        vertices.push_back(p);
        if(l.size() > 5) {
            n.x = l.at(3).toFloat();
            n.y = l.at(4).toFloat();
            n.z = l.at(5).toFloat();
        } else {
            n.x = 0; n.y = 0; n.z = 0;
        }
        vertices.push_back(n);
        if(l.size() > 8) {
            c.x = l.at(6).toInt() / 255.0f;
            c.y = l.at(7).toInt() / 255.0f;
            c.z = l.at(8).toInt() / 255.0f;
        } else {
            c.x = 1; c.y = 1; c.z = 1;
        }
        vertices.push_back(c);

    }

    qDebug() << "vertices ok";

    for (int i = cpt + vertexCount; i < cpt + vertexCount + faceCount; ++i) {
        QVector<int> v;
        QStringList faces = list.at(i).split(" ");
        if(faces.size() <= 4) {
            for (int j = 1; j < faces.size(); ++j) {
                v.push_back(faces.at(j).toInt());
            }
        } else {
            v.push_back(faces.at(1).toInt());
            v.push_back(faces.at(2).toInt());
            v.push_back(faces.at(3).toInt());

            v.push_back(faces.at(1).toInt());
            v.push_back(faces.at(3).toInt());
            v.push_back(faces.at(4).toInt());
        }
        indexes.push_back(v);
    }


    for (int i = 0; i < indexes.size(); ++i) {
        QVector<int> v = indexes[i];
        for (int j = 0; j < v.size(); ++j) {
            point p = vertices[v[j] * 3];
            point n = vertices[v[j] * 3 + 1];
            point c = vertices[v[j] * 3 + 2];

            QVector3D v3d;
            v3d.setX(p.x);
            v3d.setY(p.y);
            v3d.setZ(p.z);
            verticesArray.push_back(v3d);

            QVector3D n3d;
            n3d.setX(n.x);
            n3d.setY(n.y);
            n3d.setZ(n.z);
            normalsArray.push_back(n3d);

            QVector3D c3d;
            c3d.setX(c.x);
            c3d.setY(c.y);
            c3d.setZ(c.z);
            colorsArray.push_back(c3d);
        }
    }
    qDebug() << "vertices array = " << verticesArray.size();
    qDebug() << "normals array = " << normalsArray.size();
    qDebug() << "colors array = " << colorsArray.size();
}

PlyEntity::PlyEntity()
{
    this->scale = 1;
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->angle = 0;
}
