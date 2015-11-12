#include "trianglewindow.h"

using namespace std;



void TriangleWindow::changeSeason(){
    for(int i = 0 ; i < sockets.size() ; i++){
        sockets.at(i)->write("season");
    }
    season++;
    season %= 4;
    setSeason(season,false);
    //qDebug()<<"Je suis à la saison "<<season<<endl;
}

void TriangleWindow::newConnection(){

    socket = server->nextPendingConnection();
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        //qDebug()<<socket->peerAddress();
    }
    sockets.push_back(socket);
}

TriangleWindow::TriangleWindow(bool load){
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);

    timerFiveMinutes = new QTimer();
    timerFiveMinutes->connect(timerFiveMinutes, SIGNAL(timeout()),this,SLOT(changeSeason()));
    timerFiveMinutes->setInterval(1000*5);
    timerFiveMinutes->start();
    server = NULL;
    socket = NULL;

    particules = new Particules();
    nbPolygones = 0;
}

TriangleWindow::TriangleWindow()
{
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);

    timerFiveMinutes = new QTimer();
    timerFiveMinutes->connect(timerFiveMinutes, SIGNAL(timeout()),this,SLOT(changeSeason()));
    timerFiveMinutes->setInterval(1000*5);
    timerFiveMinutes->start();

     master = true;
     server = new QTcpServer(this);
     socket = NULL;

     connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
     server->listen(QHostAddress::Any, 4567);
     //sockets = new std::vector<QTcpSocket*>();

     loadMap(":/heightmap-1.png");
     nbPolygones = 0;
}

TriangleWindow::TriangleWindow(int _maj)
{

    maj = _maj;
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);

    socket = new QTcpSocket(this);
    server = NULL;

    socket->connectToHost(/*socket->localAddress()*/"127.0.0.1",4567);

    if(socket->waitForConnected(5000)){
        //qDebug("La connection est établie, bravo Vincent!");
    }else{
        //qDebug("La connection n'est pas établie, mais tu avance!");
    }

    loadMap(":/heightmap-1.png");
    nbPolygones = 0;

}


void TriangleWindow::initialize()
{
    const qreal retinaScale = devicePixelRatio();


    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);


    //loadMap(":/heightmap-1.png");

}

void TriangleWindow::loadMap(QString localPath)
{

    if (QFile::exists(localPath)) {
        m_image = QImage(localPath);
    }


    uint id = 0;
    widthPoint = m_image.width();
    heightPoint = m_image.height();
    nbPoints = widthPoint * heightPoint;
    p = (Point**)malloc(sizeof(Point*) * nbPoints);
    QRgb pixel;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*m_image.width() +j;

            p[id] = new Point();
            p[id]->setX((float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width());
            p[id]->setY((float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height());
            p[id]->setZ(0.001f * (float)(qRed(pixel)));
        }
    }
}

void TriangleWindow::render()
{

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glScalef(c->ss,c->ss,c->ss);

    glRotatef(c->rotX,1.0f,0.0f,0.0f);
    if(c->anim == 0.0f)
    {
        glRotatef(c->rotY,0.0f,0.0f,1.0f);
    }
    else
    {
        glRotatef(c->anim,0.0f,0.0f,1.0f);
        if(master)
            c->anim +=0.05f;
    }
    switch(c->etat)
    {
    case 0:
        displayPoints();
        displayPolygones(1);
        break;
    case 1:
        displayLines();
        displayPolygones(2);
        break;
    case 2:
        displayTriangles();
        displayPolygones(2);
        break;
    case 3:
        displayTrianglesC();
        displayPolygones(2);
        break;
    case 4:
        displayTrianglesTexture();
        displayPolygones(0);
        break;
    case 5:
        displayTrianglesTexture();
        displayLines();
        displayPolygones(0);
        break;
    default:
        displayPoints();
        displayPolygones(0);
        break;
    }
    ++m_frame;
    if(server == NULL && socket != NULL){
        if(socket->readAll() == "season"){
            season ++;
            season %= 4;
            setSeason(season,false);
        }
    }

}

bool TriangleWindow::event(QEvent *event)
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

void TriangleWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case 'C':
        if(c->anim == 0.0f)
            c->anim = c->rotY;
        else
            c->anim = 0.0f;
        break;
    case 'Z':
        c->ss += 0.10f;
        break;
    case 'S':
        c->ss -= 0.10f;
        break;
    case 'A':
        c->rotX += 1.0f;
        break;
    case 'E':
        c->rotX -= 1.0f;
        break;
    case 'Q':
        c->rotY += 1.0f;
        break;
    case 'D':
        c->rotY -= 1.0f;
        break;
    case 'W':
        c->etat ++;
        if(c->etat > 5)
            c->etat = 0;
        break;
    case 'P':
        maj++;
        timer->stop();
        timer->start(maj);
        break;
    case 'O':
        maj--;
        if(maj < 1)
            maj = 1;
        timer->stop();
        timer->start(maj);
        break;
    case 'L':
        maj = maj - 20;
        if(maj < 1)
            maj = 1;
        timer->stop();
        timer->start(maj);
        break;
    case 'M':
        maj = maj + 20;

        timer->stop();
        timer->start(maj);
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
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
}


void TriangleWindow::displayPoints()
{
    //glColor3f(1.0f, 1.0f, 1.0f);
    displaySeasons();
    glPointSize(1);
    glBegin(GL_POINTS);


/*
    uint id = 0;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());

        }
    }*/
    for(int i = 0; i < nbPoints; i++){
        glColor3f(colors[i][0],colors[i][1],colors[i][2]);
        glVertex3f(p[i]->getX(),
                   p[i]->getY(),
                   p[i]->getZ());
    }
    glEnd();
}


void TriangleWindow::displayTriangles()
{
    //glColor3f(1.0f, 1.0f, 1.0f);
    displaySeasons();
    glBegin(GL_TRIANGLES);
    uint id = 0;
    for(int i = 0; i < widthPoint-1; i++)
    {
        for(int j = 0; j < heightPoint-1; j++)
        {

            id = i*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = i*widthPoint +(j+1);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());



            id = i*widthPoint +(j+1);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j+1;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
        }
    }

    glEnd();
}

void TriangleWindow::displayTrianglesC()
{

    //glColor3f(1.0f, 1.0f, 1.0f);
    displaySeasons();
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < widthPoint-1; i++)
    {
        for(int j = 0; j < heightPoint-1; j++)
        {

            id = i*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = i*widthPoint +(j+1);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());



            id = i*widthPoint +(j+1);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j+1;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
        }
    }
    glEnd();
}


void TriangleWindow::displayLines()
{
    //glColor3f(1.0f, 1.0f, 1.0f);
    displaySeasons();
    glBegin(GL_LINES);
    uint id = 0;

    for(int i = 0; i < widthPoint-1; i++)
    {
        for(int j = 0; j < heightPoint-1; j++)
        {

            id = i*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = i*widthPoint +(j+1);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());

            id = (i+1)*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = i*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());

            id = (i+1)*widthPoint +j;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = i*widthPoint +(j+1);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());

            id = i*widthPoint +(j+1);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j+1;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());

            id = (i+1)*widthPoint +j+1;
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());

            id = (i+1)*widthPoint +(j);
            glColor3f(colors[id][0],colors[id][1],colors[id][2]);
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
        }
    }

    glEnd();
}

void TriangleWindow::displayTrianglesTexture()
{
    //glColor3f(1.0f, 1.0f, 1.0f);
    displaySeasons();
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < widthPoint-1; i++)
    {
        for(int j = 0; j < heightPoint-1; j++)
        {

            id = i*widthPoint +j;
            //displayColor(p[id]->getZ());
            displaySeasons();
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = i*widthPoint +(j+1);
           // displayColor(p[id]->getZ());
            displaySeasons();
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j;
            //displayColor(p[id]->getZ());
            displaySeasons();
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());



            id = i*widthPoint +(j+1);
            //displayColor(p[id]->getZ());
            displaySeasons();
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j+1;
           // displayColor(p[id]->getZ());
            displaySeasons();
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
            id = (i+1)*widthPoint +j;
            //displayColor(p[id]->getZ());
            displaySeasons();
            glVertex3f(
                        p[id]->getX(),
                        p[id]->getY(),
                        p[id]->getZ());
        }
    }
    glEnd();
}


void TriangleWindow::displayColor(float alt)
{
    if (alt > 0.2)
    {
        glColor3f(01.0f, 1.0f, 1.0f);
    }
    else     if (alt > 0.1)
    {
        glColor3f(alt, 1.0f, 1.0f);
    }
    else     if (alt > 0.05f)
    {
        glColor3f(01.0f, alt, alt);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 1.0f);
    }

}

//TP3
void TriangleWindow::setSeason(int i, bool charge){
    season = i;




    if(!charge){
        float min = p[0]->getZ();
        float max = p[0]->getZ();
        float moy = 0.0f;

        colors = (GLfloat**)malloc(sizeof(GLfloat*) * nbPoints);
        for(int i = 0; i < nbPoints; i++){
            colors[i] = (GLfloat*)malloc(sizeof(GLfloat) * 3);
            moy += p[i]->getZ();
            if(p[i]->getZ() > max){
                max = p[i]->getZ();
            }
            if(p[i]->getZ() < min){
                min = p[i]->getZ();
            }
        }
        moy /= nbPoints;
        float ecartBas;
        float ecartHaut;
        GLfloat p[3];
        switch(season){
        case 0:
            //Particules
            particules = new Particules(this->p, nbPoints);
            p[0] = 0.8f; p[1] = 0.8f; p[2] = 0.8f;
            particules->setParticuleColor(p);
            particules->setParticuleSize(2);
            particules->init(0.04,3,6);

            //Plys
            if(nbPolygones > 0){
                for(int j = 0; j < nbPolygones; j++){
                    polygones[j]->changeModel(":/wintertree.ply");
                }
            }

            //Couleurs
            ecartBas = (max - min ) / 10.0f;
            ecartHaut = (max - min) / 2.0f;
            for(int i = 0; i < nbPoints; i++){
                if(this->p[i]->getZ() < min + ecartBas){//partie basse
                    colors[i][0] = 127.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 255.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 212.0f / 255.0f + this->p[i]->getZ()/max;
                }else
                if(this->p[i]->getZ() > max - ecartHaut){//partie haute
                    colors[i][0] = 254.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 254.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 254.0f / 255.0f + this->p[i]->getZ()/max;
                }
                else{
                    colors[i][0] = 150.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 150.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 150.0f / 255.0f + this->p[i]->getZ()/max;
                }
            }
            break;
        case 1:
            //Particules
            particules = NULL;

            //Plys
            if(nbPolygones > 0){
                for(int j = 0; j < nbPolygones; j++){
                    polygones[j]->changeModel(":/springtree.ply");
                }
            }

            //Couleurs
            ecartBas = (max - min ) / 10.0f;
            ecartHaut = (max - min) / 2.0f;
            for(int i = 0; i < nbPoints; i++){
                if(this->p[i]->getZ() < min + ecartBas){//partie basse
                    colors[i][0] = 0.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 0.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 100.0f / 255.0f + this->p[i]->getZ()/max;
                }else
                if(this->p[i]->getZ() > max - ecartHaut){//partie haute
                    colors[i][0] = 200.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 200.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 200.0f / 255.0f + this->p[i]->getZ()/max;
                }
                else{
                    colors[i][0] = 0.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 150.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 0.0f / 255.0f + this->p[i]->getZ()/max;
                }
            }
            break;
        case 2:
            //Particules
            particules = NULL;

            //Plys
            if(nbPolygones > 0){
                for(int j = 0; j < nbPolygones; j++){
                    polygones[j]->changeModel(":/summertree.ply");
                }
            }

            //Couleurs
            ecartBas = (max - min ) / 20.0f;
            ecartHaut = (max - min) / 2.0f;
            for(int i = 0; i < nbPoints; i++){
                if(this->p[i]->getZ() < min + ecartBas){//partie basse
                    colors[i][0] = 0.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 238.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 238.0f / 255.0f + this->p[i]->getZ()/max;
                }else
                if(this->p[i]->getZ() > max - ecartHaut){//partie haute
                    colors[i][0] = 150.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 150.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 150.0f / 255.0f + this->p[i]->getZ()/max;
                }
                else{
                    colors[i][0] = 173.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 255.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 47.0f / 255.0f + this->p[i]->getZ()/max;
                }
            }
            break;
        case 3:
            //Particules
            particules = new Particules(this->p, nbPoints);
            p[0] = 0.0f; p[1] = 0.0f; p[2] = 0.8f;
            particules->setParticuleColor(p);
            particules->init(0.1,100,1000);

            //Plys
            if(nbPolygones > 0){
                for(int j = 0; j < nbPolygones; j++){
                    polygones[j]->changeModel(":/autumntree.ply");
                }
            }

            //Couleurs
            ecartBas = (max - min ) / 10.0f;
            ecartHaut = (max - min) / 2.0f;
            for(int i = 0; i < nbPoints; i++){
                if(this->p[i]->getZ() < min + ecartBas){//partie basse
                    colors[i][0] = 0.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 0.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 100.0f / 255.0f + this->p[i]->getZ()/max;
                }else
                if(this->p[i]->getZ() > max - ecartHaut){//partie haute
                    colors[i][0] = 200.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 200.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 200.0f / 255.0f + this->p[i]->getZ()/max;
                }
                else{
                    colors[i][0] = 139.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][1] = 90.0f / 255.0f + this->p[i]->getZ()/max;
                    colors[i][2] = 43.0f / 255.0f + this->p[i]->getZ()/max;
                }
            }
            break;
        }
    }
}

void TriangleWindow::displaySeasons(){



    switch (season){
    case 0://Hiver

        particules->update();
        //glColor3f(1.0f, 1.0f, 1.0f);
        glClearColor(0.5f,0.5f,0.5f,1.0f);
        break;
    case 1://Printemps
        glClearColor(0.75f,0.85f,1.0f,1.0f);
        //glColor3f(0.0f, 0.75f, 0.0f);

        break;
    case 2://Ete
        glClearColor(0.0f,0.5f,0.8f,1.0f);
        //glColor3f(1.0f, 1.0f, 0.0f);

        break;
    case 3://Automne
        glClearColor(0.92f,0.57f,0.13f,1.0f);
        particules->update();
        //glColor3f(0.37f, 0.2f, 0.07f);

        break;
    default:
        break;
    }
}

//TP4
paramCamera* TriangleWindow::getCamera(){
    return c;
}

int TriangleWindow::getSeason(){
    return season;
}

Point** TriangleWindow::getPoints(){
    return p;
}

void TriangleWindow::setPoints(Point** pt){
    p = pt;
}

int TriangleWindow::getNbPoints(){
    return nbPoints;
}

void TriangleWindow::setNbPoints(int i){
    nbPoints = i;
}

int TriangleWindow::getWidthPoint(){
    return widthPoint;
}

void TriangleWindow::setWidthPoint(int w){
    widthPoint = w;
}

int TriangleWindow::getHeightPoint(){
    return heightPoint;
}

void TriangleWindow::setHeightPoint(int h){
    heightPoint = h;
}


Particules* TriangleWindow::getParticules(){
    return particules;
}

void TriangleWindow::setParticules(Particules* p){
    particules = p;
}

Ply** TriangleWindow::getPolygones(){
    return polygones;
}

void TriangleWindow::setPolygones(Ply** p){
    polygones = p;
}

int TriangleWindow::getNbPolygones(){
    return nbPolygones;
}

void TriangleWindow::setNbPolygones(int n){
    nbPolygones = n;
}

void TriangleWindow::addPolygone(Ply* p){
    if(nbPolygones > 0){
        Ply** pBis = (Ply**)malloc(sizeof(Ply*) * (nbPolygones + 1));
        for(int i = 0; i < nbPolygones; i++){
            pBis[i] = polygones[i];
        }
        pBis[nbPolygones] = p;
        nbPolygones++;
        polygones = pBis;
    }else{
        nbPolygones = 1;
        polygones = (Ply**)malloc(sizeof(Ply*));
        polygones[0] = p;
    }
}

void TriangleWindow::displayPolygones(int choix){
    switch(choix){
    case 0:
        for(int i = 0; i < nbPolygones; i++){
            polygones[i]->drawPoints();
        }
        break;
    case 1:
        for(int i = 0; i < nbPolygones; i++){
            polygones[i]->drawLines();
        }
        break;
    case 2:
        for(int i = 0; i < nbPolygones; i++){
            polygones[i]->drawFaces();
        }
        break;
    default:
        for(int i = 0; i < nbPolygones; i++){
            polygones[i]->drawPoints();
        }
        break;
    }


}


void TriangleWindow::setColors(GLfloat** c){
    colors = c;
}

GLfloat** TriangleWindow::getColors(){
    return colors;
}
