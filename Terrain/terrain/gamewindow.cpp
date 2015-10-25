#include "gamewindow.h"
#include "terrain.h"
#include "camera.h"

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

#include <QTimer>

using namespace std;

static bool animate = true; //permet de savoir si on anime les terrain des différentes fenêtre GameWindow en les faisant tourner selon l'axe y

static const char *vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"

        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"

        "float rand(vec2 co){\n"
        "   return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n"
        "}\n"

        "void main() {\n"
        "   gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"

        "   col = colAttr;\n"

        "   vec3 normals = gl_Normal;\n"
        "   //normals /= rand(posAttr.xy);\n"

        "   gl_Position = matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"

        "uniform sampler2D tex;\n"
        "void main() {\n"
        "   gl_FragColor = texture2D(tex,gl_TexCoord[0].st) * col;\n"
        "}\n";

/**
 * @brief GameWindow::GameWindow, constructeur de la classe GameWindow.
 * @param refresh_rate, taux de rafraîchissement de la fenêtre
 * @param c, paramètre facultatif qui permet d'avoir une caméra partagée par plusieurs fenêtres
 */
GameWindow::GameWindow(int refresh_rate, Camera* c) : carte(1), m_refresh_rate(refresh_rate), speed(0.5f)
{
    srand(time(NULL));

    if(c != 0){
        share_cam = true;
        m_camera = c;
    }else{
        share_cam = false;
        m_camera = new Camera();
    }

    season = "NONE";
    saison = Saison::NONE;
    nb_vertex_width = nb_vertex_height = 0;

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this, SLOT(renderNow()));

    restartTimer();
    doConnect();
}

GLuint GameWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

/**
 * @brief GameWindow::~GameWindow, destructeur de la classe GameWindow.
 */
GameWindow::~GameWindow(){
    delete p;

    if(!share_cam)
        delete m_camera;

    delete m_timer;
    delete texture;
}

/**
 * @brief GameWindow::initialize, initialise les fonctionnalités OpenGL et charge une heightmap.
 */
void GameWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    texture = new Texture();
    texture->charger("./grass.tga");
    texture->definit_bouclage(GL_REPEAT, GL_REPEAT);
    texture->definit_filtrage(GL_LINEAR, GL_LINEAR);
    texture->definit_melange(GL_MODULATE);

    const qreal retinaScale = devicePixelRatio();

    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);

    glEnable(GL_DEPTH_TEST);    // Active le Z-Buffer
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    //Add ambient light
    GLfloat ambientColor[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add directionnal light
    GLfloat lightColor0[] = {1.f, 1.f, 1.f, 1.0f};
    GLfloat lightPos0[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    Terrain* T = FileManager::Instance().getTerrain();
    if(T->nb_vertex_width != 0)
    {
        season = T->saison;
        updateEnumSaison();
        nb_vertex_width = T->nb_vertex_width;
        nb_vertex_height = T->nb_vertex_height;
        p = new point[nb_vertex_width * nb_vertex_height];
        for(int i = 0 ; i < nb_vertex_width * nb_vertex_height ; i++)
        {
            p[i].x = T->vertex[i].x();
            p[i].y = T->vertex[i].y();
            p[i].z = T->vertex[i].z();
        }

        initTrees(T->tree);
        updateTitle();
    }else{
        loadMap(":/heightmap-2.png");
        initTrees();
    }

    createParticles();
}

/**
 * @brief GameWindow::loadMap, permet de charger une heightmap.
 * @param localPath, chemin vers la heightmap
 */
void GameWindow::loadMap(QString localPath)
{

    if (QFile::exists(localPath)) {
        m_image = QImage(localPath);
    }

    uint id = 0;
    nb_vertex_width = m_image.width();
    nb_vertex_height = m_image.height();
    p = new point[nb_vertex_width * nb_vertex_height];
    QRgb pixel;
    for(int i = 0; i < nb_vertex_width; i++)
    {
        for(int j = 0; j < nb_vertex_height; j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*nb_vertex_width +j;

            p[id].x = (float)i/(nb_vertex_width) - ((float)nb_vertex_width/2.0)/nb_vertex_width;
            p[id].y = (float)j/(nb_vertex_width) - ((float)nb_vertex_height/2.0)/nb_vertex_height;
            p[id].z = 0.001f * (float)(qRed(pixel));
        }
    }
}

/**
 * @brief GameWindow::render, fonction de rendu OpenGL contenant plusieurs type de rendu.
 */
void GameWindow::render()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setAttributeValue(m_colAttr, QVector4D(1,1,1,1));

    glLoadIdentity();
    glScalef(m_camera->ss,m_camera->ss,m_camera->ss);
    glRotatef(m_camera->rotX,1.0f,0.0f,0.0f);
    glRotatef(m_camera->rotY,0.0f,0.0f,1.0f);

    switch(m_camera->etat)
    {
    case 0:
        displayPoints();
        break;
    case 1:
        displayLines();
        break;
    case 2:
        displayTriangles();
        break;
    case 3:
        displayTrianglesC();
        break;
    case 4:
        displayTrianglesTexture();
        break;
    case 5:

        displayTrianglesTexture();
        displayLines();
        break;
    default:
        displayPoints();
        break;
    }

    displayParticles();
    displayTree();

    if(animate){
        animWindow();
    }

    m_program->release();
    ++m_frame;
}

/**
 * @brief GameWindow::event, fonction permettant la gestion d'évenements.
 * @param event, QEvent
 * @return
 */
bool GameWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

/**
 * @brief GameWindow::keyPressEvent, permet de gérer les évenements de type clavier.
 * @param event, évenement de type clavier
 */
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case 'Z':
        m_camera->ss += 0.10f;
        break;
    case 'S':
        m_camera->ss -= 0.10f;
        break;
    case 'A':
        m_camera->rotX += 1.0f;
        break;
    case 'E':
        m_camera->rotX -= 1.0f;
        break;
    case 'Q':
        m_camera->rotY += 1.0f;
        break;
    case 'D':
        m_camera->rotY -= 1.0f;
        break;
    case 'W':
        m_camera->etat ++;
        if(m_camera->etat > 5)
            m_camera->etat = 0;
        break;
    case 'C':
        animate = !animate;
        break;
    case 'P':
        m_refresh_rate *= 2;
        if(m_refresh_rate > 120)
            m_refresh_rate = 120;

        restartTimer();
        break;
    case 'M':
        m_refresh_rate /= 2;
        if(m_refresh_rate < 1)
            m_refresh_rate = 1;

        restartTimer();
        break;
    case 'X':
        carte ++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;
    }
}

/**
 * @brief GameWindow::displayPoints, fonction d'affichage du terrain avec des points.
 */
void GameWindow::displayPoints()
{
    seasonColor();

    glBegin(GL_POINTS);

    uint id = 0;

#pragma omp for schedule(dynamic)
    for(int i = 0; i < nb_vertex_width; i++)
    {
        for(int j = 0; j < nb_vertex_height; j++)
        {
            id = i*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

        }
    }
    glEnd();
}

/**
 * @brief GameWindow::displayTriangles, fonction d'affichage du terrain avec des triangles.
 */
void GameWindow::displayTriangles()
{
    seasonColor();
    glBegin(GL_TRIANGLES);
    uint id = 0;

#pragma omp for schedule(dynamic)
    for(int i = 0; i < nb_vertex_width-1; i++)
    {
        for(int j = 0; j < nb_vertex_height-1; j++)
        {

            id = i*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*nb_vertex_width +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*nb_vertex_width +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

/**
 * @brief GameWindow::displayTrianglesC, fonction d'affichage du terrain avec des triangles de couleurs différentes.
 */
void GameWindow::displayTrianglesC()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

#pragma omp for schedule(dynamic)
    for(int i = 0; i < nb_vertex_width-1; i++)
    {
        for(int j = 0; j < nb_vertex_height-1; j++)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            id = i*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*nb_vertex_width +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);


            glColor3f(1.0f, 1.0f, 1.0f);
            id = i*nb_vertex_width +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}

/**
 * @brief GameWindow::displayLines, fonction d'affichage du terrain avec des lignes.
 */
void GameWindow::displayLines()
{
    seasonColor();
    glBegin(GL_LINES);
    uint id = 0;

#pragma omp for schedule(dynamic)
    for(int i = 0; i < nb_vertex_width-1; i++)
    {
        for(int j = 0; j < nb_vertex_height-1; j++)
        {

            id = i*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*nb_vertex_width +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*nb_vertex_width +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*nb_vertex_width +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = i*nb_vertex_width +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*nb_vertex_width +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*nb_vertex_width +(j);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

/**
 * @brief GameWindow::displayTrianglesTexture, fonction d'affichage du terrain avec des triangles texturés en fonction de la hauteur des vertices.
 */
void GameWindow::displayTrianglesTexture()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

#pragma omp for schedule(dynamic)
    for(int i = 0; i < nb_vertex_width-1; i++)
    {
        for(int j = 0; j < nb_vertex_height-1; j++)
        {

            id = i*nb_vertex_width +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*nb_vertex_width +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*nb_vertex_width +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*nb_vertex_width +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}

/**
 * @brief GameWindow::displayColor, affiche une couleur sur les vertices en fonction de l'altitude de ceux-ci.
 * @param alt, altitude en z
 */
void GameWindow::displayColor(float alt)
{
    if (alt > 0.2)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else     if (alt > 0.1)
    {
        glColor3f(alt, 1.0f, 1.0f);
    }
    else     if (alt > 0.05f)
    {
        glColor3f(1.0f, alt, alt);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 1.0f);
    }

}

/**
 * @brief GameWindow::animWindow, fonction permettant la rotation automatique du terrain selon l'axe y.
 */
void GameWindow::animWindow(){
    m_camera->rotY += speed;
}

/**
 * @brief GameWindow::restartTimer, permet de relancer le timer lorsque celui-ci change.
 */
void GameWindow::restartTimer(){
    m_timer->stop();
    m_timer->start(1000.f / m_refresh_rate);

    updateTitle();
}

/**
 * @brief GameWindow::updateTitle, Met à jour le titre de la fenêtre en fonction de la saison et du framerate de celle-ci.
 */
void GameWindow::updateTitle(){
    QString fps = QString::number(m_refresh_rate);
    QString title = fps + "FPS - " + season;
    setTitle(title);
}

/**
 * @brief GameWindow::seasonColor, Affiche le terrain d'une couleur différente en fonction de la saison.
 */
color GameWindow::seasonColor(){
    if(saison == Saison::PRINTEMPS){
        glColor3f(0.f, 0.7f, 0.2f);
        return color{0.f, 0.7f, 0.2f};
    }
    else if(saison == Saison::ETE){
        glColor3f(1.f, 1.f, 0.f);
        return color{1.f, 1.f, 0.f};
    }
    else if(saison == Saison::AUTOMNE){
        glColor3f(0.86f, 0.4f, 0.f);
        return color{0.86f, 0.4f, 0.f};
    }
    else if(saison == Saison::HIVER){
        glColor3f(0.8f, 0.8f, 0.8f);
        return color{0.8f, 0.8f, 0.8f};
    }

    return color{0.f,0.f,0.f};
}

/**
 * @brief GameWindow::createParticles, Initialise le tableau de particules.
 */
void GameWindow::createParticles(){
    tab_particles = new particles[MAX_PARTICLES];

#pragma omp parallel for
    for(unsigned int i = 0 ; i < MAX_PARTICLES ; i++){
        tab_particles[i] = newParticle();
    }
}

/**
 * @brief GameWindow::newParticle, Créé et retourne une nouvelle particule à une position aléatoire.
 * @return nouvelle particule
 */
particles GameWindow::newParticle(){
    particles part;
    int rand_x, rand_y, id;

    rand_x = rand() % nb_vertex_width;
    rand_y = rand() % nb_vertex_height;
    id = rand_y * nb_vertex_width + rand_x;

    part.x = p[id].x;
    part.y = p[id].y;
    part.z = (rand() % 5 + 3) / 10.f;
    part.min_z = p[id].z;
    part.falling_speed = (rand() % 10 + 1) / 1000.f;

    return part;
}

/**
 * @brief GameWindow::displayParticles, Affiche le système de particules lorsque c'est nécessaire.
 */
void GameWindow::displayParticles(){
    if(season == "AUTOMNE"){
        glColor3f(0.0f, 0.0f, 1.0f);
    }else if(season == "HIVER"){
        glColor3f(1.0f, 1.0f, 1.0f);
    }else{
        return;
    }

    glBegin(GL_POINTS);

#pragma omp for schedule(dynamic)
    for(unsigned int i = 0 ; i < MAX_PARTICLES ; i++){
        glVertex3f(tab_particles[i].x, tab_particles[i].y, tab_particles[i].z);

        tab_particles[i].z -= tab_particles[i].falling_speed;

        if(tab_particles[i].z < tab_particles[i].min_z){
            tab_particles[i] = newParticle();
        }
    }
    glEnd();
}

/**
 * @brief GameWindow::displayTree, Affiche les différents arbres sur le terrain.
 */
void GameWindow::displayTree(){
    if(saison == Saison::NONE){
        return;
    }

    for(unsigned int i = 0 ; i < NB_ARBRES ; i++){
        tree[saison][i]->display();
    }
}

/**
 * @brief GameWindow::doConnect, Connecte le client au serveur.
 */
void GameWindow::doConnect(){
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << QString::number(m_refresh_rate) << "FPS window : connecting...";

    // this is not blocking call
    socket->connectToHost("127.0.0.1", 9999);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

/**
 * @brief GameWindow::save, envoi le terrain de la fenêtre au FileManager pour le sauvegarder.
 */
void GameWindow::save(){
    Terrain* T = new Terrain(season, seasonColor(), nb_vertex_width, nb_vertex_height, p, tree);
    FileManager::Instance().saveCustomMap(T);
}

/**
 * @brief GameWindow::updateEnumSaison, met à jour l'enum Saison en fonction de la saison courante.
 */
void GameWindow::updateEnumSaison()
{
    if(season == "PRINTEMPS"){
        saison = Saison::PRINTEMPS;
    }else if(season == "ETE"){
        saison = Saison::ETE;
    }else if(season == "AUTOMNE"){
        saison = Saison::AUTOMNE;
    }else if(season == "HIVER"){
        saison = Saison::HIVER;
    }
}

/**
 * @brief GameWindow::initTrees, Lorsque le fichier binaire est vide, initialise les arbres des différentes saisons pour la fenêtre.
 */
void GameWindow::initTrees()
{
    tree = new GameObject**[FileManager::NB_TERRAIN];
    tree[Saison::PRINTEMPS] = new GameObject*[NB_ARBRES];
    tree[Saison::ETE] = new GameObject*[NB_ARBRES];
    tree[Saison::AUTOMNE] = new GameObject*[NB_ARBRES];
    tree[Saison::HIVER] = new GameObject*[NB_ARBRES];

    for(unsigned int j = 0 ; j < FileManager::NB_TERRAIN ; j++){
        for(unsigned int i = 0 ; i < NB_ARBRES ; i++){
            QVector3D pos;

            int rand_x, rand_y, id;

            rand_x = rand() % nb_vertex_width;
            rand_y = rand() % nb_vertex_height;
            id = rand_y * nb_vertex_width + rand_x;

            pos.setX(p[id].x); pos.setY(p[id].y); pos.setZ(p[id].z);

            if(j == Saison::PRINTEMPS){
                tree[j][i] = new GameObject(pos, QVector3D(0.f,0.f,0.f), QVector3D(0.1f,0.1f,0.1f), ":/island_tree.ply");
            }else if(j == Saison::ETE){
                tree[j][i] = new GameObject(pos, QVector3D(0.f,0.f,0.f), QVector3D(0.1f,0.1f,0.1f), ":/summertree.ply");
            }else if(j == Saison::AUTOMNE){
                tree[j][i] = new GameObject(pos, QVector3D(0.f,0.f,0.f), QVector3D(0.1f,0.1f,0.1f), ":/autumntree.ply");
            }else if(j == Saison::HIVER){
                tree[j][i] = new GameObject(pos, QVector3D(0.f,0.f,0.f), QVector3D(0.1f,0.1f,0.1f), ":/wintertree.ply");
            }
        }
    }
}

/**
 * @brief GameWindow::initTrees, Initialise les arbres de la fenêtre en fonction des données contenu dans le fichier binaire.
 * @param t, tableau de GameObject contenant les différents arbres des différentes saisons
 */
void GameWindow::initTrees(GameObject ***t)
{
    tree = new GameObject**[FileManager::NB_TERRAIN];

    for(unsigned int j = 0 ; j < FileManager::NB_TERRAIN ; j++){
        tree[j] = new GameObject*[NB_ARBRES];

        for(unsigned int i = 0 ; i < NB_ARBRES ; i++){
            tree[j][i] = t[j][i];
            tree[j][i]->open(tree[j][i]->localPath);
        }
    }
}

/** SLOTS **/

/**
 * @brief GameWindow::connected, s'execute lorsque le client se connecte à un serveur.
 */
void GameWindow::connected()
{
    qDebug() << QString::number(m_refresh_rate) << "FPS window : connected...";
}

/**
 * @brief GameWindow::disconnected, s'éxecute lorsque le client est déconnecté.
 */
void GameWindow::disconnected()
{
    qDebug() << QString::number(m_refresh_rate) << "FPS window : disconnected...";
}

/**
 * @brief GameWindow::readyRead, permet de recevoir les données envoyées par le serveur.
 */
void GameWindow::readyRead()
{
    // read the data from the socket
    QString str = QString(socket->readAll());
    if(str == "SAVE"){
        save();
    }else{
        season = str;
        updateEnumSaison();
        updateTitle();
    }
}
