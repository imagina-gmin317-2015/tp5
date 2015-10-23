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
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Active la correction de perspective (pour ombrage, texture, ...)

    snow = new SnowParticles(1000, 1000, &this->m_image);
    rain = new RainParticles(&this->m_image);
    drought = new Drought();
    spring = new Spring(&this->m_image);
    this->season = firstSeason++;
    this->windowId = this->season;
    this->onSeasonChange();
    this->displayNormals = false;
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
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

    int countX = m_image.width();
    int countY = m_image.height();
    int count = countX * countY * 3 * 2 + countX * 3 + 3;
    glBegin(GL_TRIANGLE_STRIP);
#pragma omp for schedule(dynamic)
    for (int var = 0; var < count - 9; var += 3) {
        if(vertices[var + 2] < 0.08) {
            glColor3f(vertices[var + 2] + drought->getYellow(), 0.4, 0);
        } else if (vertices[var + 2] > 0.08 && vertices[var + 2] < 0.15 + drought->getSnowHeightModifier()) {
            glColor3f(0.54, 0.27 + vertices[var + 2], 0.07);
        } else {
            glColor3f(0.9, 0.8, 0.9);
        }
        glNormal3f(normals[var / 3]->x, normals[var / 3]->y, normals[var / 3]->z);
        glVertex3f(vertices[var], vertices[var + 1], vertices[var + 2]);
    }
    glEnd();
}

void GameWindow::drawNormals()
{
    if(displayNormals) {
        glColor3f(1, 0, 0);
        int countX = m_image.width();
        int countY = m_image.height();
        int count = countX * countY * 3 * 2 + countX * 3 + 3;
        glBegin(GL_LINES);
        float n = 0.01;
#pragma omp for schedule(dynamic)
        for (int var = 0; var < count - 9; var += 3) {

            glVertex3f(vertices[var], vertices[var + 1], vertices[var + 2]);
            glVertex3f((vertices[var] + normals[var / 3]->x*n),
                    (vertices[var + 1] + normals[var / 3]->y*n),
                    (vertices[var + 2] + normals[var / 3]->z*n));
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
    int count = countX * countY * 3 * 2 + countX * 3 + 3;
    GLfloat *array = new GLfloat[count];
    GLfloat stepX = 1.0 / (countX);
    GLfloat stepY = 1.0 / (countY);
    int cpt = 0;

    float posX = -0.5f;
    float posY = -0.5f;

    int flop = 1;
#pragma omp for schedule(dynamic)
    for (int i = 0; i < countX; ++i) {
        for (int j = 0; j < countY; ++j) {
            array[cpt++] = posX;
            array[cpt++] = posY;
            array[cpt++] = getRandomZ(posX, posY);

            array[cpt++] = posX + stepX;
            array[cpt++] = posY;
            array[cpt++] = getRandomZ(posX + stepX, posY);

            posY += stepY * flop;
        }

        array[cpt++] = posX;
        array[cpt++] = posY;
        array[cpt++] = getRandomZ(posX, posY);

        flop *= -1;
        posX += stepX;
    }

    array[cpt++] = posX;
    array[cpt++] = posY;
    array[cpt++] = getRandomZ(posX, posY);
#pragma omp for schedule(dynamic)
    for (int var = 0; var < cpt - 9; var += 3) {
        point p1, p2, p3;
        p1.x = array[var+0]; p1.y = array[var+1]; p1.z = array[var+2];
        p2.x = array[var+3]; p2.y = array[var+4]; p2.z = array[var+5];
        p3.x = array[var+6]; p3.y = array[var+7]; p3.z = array[var+8];
        std::vector<float> n;
        point *p = new point();
        n = Utils::getNormal(p1, p2, p3);
        p->x = n[0]; p->y = n[1]; p->z = n[2];

        if(p->z < 0) {
            p->x *= -1;
            p->y *= -1;
            p->z *= -1;
        }

        normals.push_back(p);
    }
    return array;
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
