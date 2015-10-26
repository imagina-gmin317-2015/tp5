#include "gamewindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <functional>

#include <QtCore>
#include <QtGui>
using namespace std;

static int firstSeason = 0;
GameWindow::GameWindow()
{
    camera = new Camera();
}

GameWindow::GameWindow(Camera *camera, float framerate)
{
    this->camera = camera;
    this->framerate = framerate;

    cthread = new ClientThread();
    connect(cthread, SIGNAL(seasonChangeSignal()), this, SLOT(onSeasonChange()));

    this->cursor = new QCursor(Qt::BlankCursor);
    this->cursor2 = new QCursor(Qt::ArrowCursor);
}

void GameWindow::initialize()
{
    timer.setInterval(framerate * 1000);
    this->camera->initialize(devicePixelRatio(), width(), height(), 0, 100.0);
    timer.start();
    this->connect(&timer, SIGNAL(timeout()), this, SLOT(renderNow()));

    imagePath = ":/heightmap-1.png";
    this->m_image = QImage(imagePath);

    this->vertices = initVertices(this->m_image.width(), this->m_image.height());

    //    entity = PlyEntity::load(":/gull.ply");
    //    entity->setPosition(0, 0, qGray(this->m_image.pixel((this->m_image.width() * (0 + 0.5f)), (this->m_image.height() * (0 + 0.5f)))) * 0.0008f);
    //    entity->setScale(0.1f);

    forest = new Forest(&m_image, 10);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Active la correction de perspective (pour ombrage, texture, ...)

    snow = new SnowParticles(m_image.width() * 2, m_image.height() * 2, &this->m_image);
    rain = new RainParticles(&this->m_image);
    drought = new Drought();
    spring = new Spring(&this->m_image);
    this->season = firstSeason++;
    this->windowId = this->season;
    this->onSeasonChange();
    this->displayNormals = false;

    QImage image(":/grass.jpg");

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, 0 );

}

void GameWindow::onSeasonChange()
{
    snow->reset();
    rain->reset();
    if(++season >= 4) season = 0;

    if(season == 0) {
        snow->setActive(true);
        rain->setActive(false);
        spring->setActive(false);
        drought->setActive(false);
    } else if (season == 3) {
        rain->setActive(true);
        snow->setActive(false);
        spring->setActive(false);
        drought->setActive(false);
    } else if (season == 2){
        drought->setActive(true);
        rain->setActive(false);
        snow->setActive(false);
        spring->setActive(false);
    } else {
        spring->setActive(true);
        drought->setActive(false);
        rain->setActive(false);
        snow->setActive(false);
    }
}

void GameWindow::onSaveRequest()
{
    ResourceManager::setSave(this->serialize(), windowId);
}

void GameWindow::onLoadRequest()
{
    this->load();
}

void GameWindow::render()
{
    this->render((float) timer.interval() * 0.001f);
}

void GameWindow::render(float delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(cursorCaptured) {
        this->cursor->setPos(this->position().x() + width() * 0.5f, this->position().y() + height() * 0.5f);
    }
    this->camera->update(delta);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -0.5f, 0.5f, 4.0f, 0.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    drawTriangles();
    drawNormals();

    snow->update(delta);
    snow->draw(delta);
    rain->update(delta);
    rain->draw(delta);
    drought->update(delta);
    drought->draw();
    spring->update(delta);
    spring->draw(delta);
    ++m_frame;

    //    entity->draw(delta);
    forest->draw(delta);
}

bool GameWindow::event(QEvent *event)
{
    QMouseEvent *mouseEvent;
    float deltaX = this->width() * 0.5f;
    float deltaY = this->height() * 0.5f;
    switch (event->type())
    {
    case QEvent::MouseMove:
        if(cursorCaptured) {
            mouseEvent = static_cast<QMouseEvent*>(event);
            camera->rotate(-(deltaY - mouseEvent->y()) * 0.1f,
                           0,
                           -(deltaX - mouseEvent->x()) * 0.1f
                           );
        }
        return true;
    case QEvent::MouseButtonPress:
        this->setCursor(*cursor);
        cursorCaptured = true;
        return true;
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        cursorCaptured = false;
        this->setCursor(*cursor2);
        //        qApp->exit();
        break;
    case Qt::Key_Tab:
        qApp->exit();
        break;
    case Qt::Key_Space:
        displayNormals = !displayNormals;
        if(displayNormals) {
            //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        break;
    case 'Z':
        camera->scale(0.10f, 0.10f, 0);
        break;
    case 'S':
        camera->scale(-0.10f, -0.10f, 0);
        break;
    case Qt::Key_F5:
        emit requestLoad();
        //        fill = !fill;
        break;
    case Qt::Key_F6:
        emit requestSave();
        break;
    case 'W':
        etat ++;
        if(etat > 5)
            etat = 0;
        break;
    case 'C':
        this->camera->setAnimated(!this->camera->isAnimated());
        break;
    }
    renderNow();
}

void GameWindow::drawTriangles()
{
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, textureId);

    glColor3f(1, 1, 1);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, verticesArray.constData());
    glNormalPointer(GL_FLOAT, 0, normalsArray.constData());
    glColorPointer(3, GL_FLOAT, 0, colorsArray.constData());
    glDrawArrays(GL_TRIANGLES, 0, verticesArray.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    //    glDisable(GL_TEXTURE_2D);
}

void GameWindow::drawNormals()
{
    if(displayNormals) {
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        float x = 0.1;
        for (int i = 0; i < normalsArray.size(); i += 9) {
            QVector3D n = normalsArray[i];
            QVector3D v = verticesArray[i];
            glVertex3f(v.x(), v.y(), v.z());
            glVertex3f(v.x() + n.x() * x, v.y() + n.y() * x, v.z() + n.z() * x);
        }
        glEnd();
    }
}

QString GameWindow::serialize()
{
    QString s;
    s +=
            imagePath + ";" + ResourceManager::INT(&season)->toString() + "\n" +
            ResourceManager::serialize(camera->attributes()) + "\n" +
            ResourceManager::serialize(drought->attributes()) + "\n" +
            ResourceManager::serialize(rain->attributes()) + "\n" +
            ResourceManager::serialize(snow->attributes()) + "\n" +
            ResourceManager::serialize(spring->attributes()) + "\n";
    return s;
}

void GameWindow::load()
{
    QString s = ResourceManager::getSave(windowId);
    QStringList list = s.split("\n");

    QStringList l = list.at(0).split(";");
    this->imagePath = l.at(0);
    qDebug() << "loaded image" << this->imagePath;
    ResourceManager::assign(camera->attributes(), ResourceManager::parse(list.at(1)));
    ResourceManager::assign(drought->attributes(), ResourceManager::parse(list.at(2)));
    ResourceManager::assign(rain->attributes(), ResourceManager::parse(list.at(3)));
    ResourceManager::assign(snow->attributes(), ResourceManager::parse(list.at(4)));
    ResourceManager::assign(spring->attributes(), ResourceManager::parse(list.at(5)));
}

GLfloat *GameWindow::initVertices(GLint countX, GLint countY)
{
    GLfloat stepX = 1.0 / (countX);
    GLfloat stepY = 1.0 / (countY);

    float posX;
    float posY;

    std::function<QVector3D(QVector3D)> color = [](QVector3D v) {
        if(v.z() < 0.08) {
            return QVector3D(v.z(), 0.4, 0);
        } else if(v.z() > 0.08 && v.z() < 0.15) {
            return QVector3D(0.54, 0.27 + v.z(), 0.07);
        } else {
            return QVector3D(0.9, 0.8, 0.9);
        }
    };

    #pragma omp for schedule(dynamic)
    for (int i = 0 ; i < countX - 1; ++i) {
        for (int j = 0; j < countY - 1; ++j) {
            posX = i * stepX - 0.5; posY = (j + 1) * stepY - 0.5;
            QVector3D v2(posX,  posY, getRandomZ(posX, posY));
            verticesArray.push_back(v2);
            colorsArray.push_back(color(v2));

            posX = i * stepX - 0.5; posY = j * stepY - 0.5;
            QVector3D v1(posX,  posY, getRandomZ(posX, posY));
            verticesArray.push_back(v1);
            colorsArray.push_back(color(v1));

            posX = (i + 1) * stepX - 0.5; posY = j * stepY - 0.5;
            QVector3D v3(posX,  posY, getRandomZ(posX, posY));
            verticesArray.push_back(v3);
            colorsArray.push_back(color(v3));

            QVector3D n = QVector3D::normal(v3 - v1, v2 - v1);
            normalsArray.push_back(n);
            normalsArray.push_back(n);
            normalsArray.push_back(n);

            posX = i * stepX - 0.5; posY = (j + 1) * stepY - 0.5;
            QVector3D v4(posX, posY, getRandomZ(posX, posY));
            verticesArray.push_back(v4);
            colorsArray.push_back(color(v4));

            posX = (i + 1) * stepX - 0.5; posY = j * stepY - 0.5;
            QVector3D v6(posX, posY, getRandomZ(posX, posY));
            verticesArray.push_back(v6);
            colorsArray.push_back(color(v6));

            posX = (i + 1) * stepX - 0.5; posY = (j + 1) * stepY - 0.5;
            QVector3D v5(posX, posY, getRandomZ(posX, posY));
            verticesArray.push_back(v5);
            colorsArray.push_back(color(v5));

            QVector3D n1 = QVector3D::normal(v6 - v4, v5 - v4);
            normalsArray.push_back(n1);
            normalsArray.push_back(n1);
            normalsArray.push_back(n1);

        }
    }
    return 0;
}

QString GameWindow::serializeVertices() const
{
    int countX = m_image.width();
    int countY = m_image.height();
    int count = countX * countY * 3 * 2 + countX * 3 + 3;

    QString s = "";

    for (int i = 0; i < count; ++i) {
        s += QString::number(vertices[i]) + ";";
    }

    s += "\n";

    for (int i = 0; i < normals.size(); ++i) {
        s += QString::number(normals[i]->x) + ";";
        s += QString::number(normals[i]->y) + ";";
        s += QString::number(normals[i]->z) + ";";
    }
    return s;
}

float GameWindow::getRandomZ(float i, float j)
{
    return qGray(this->m_image.pixel((this->m_image.width() * (i + 0.5f)), (this->m_image.height() * (j + 0.5f)))) * 0.0008f;
}
