#include "gamewindow.h"
#include "gamewindowshader.h"
#include "trianglewindow.h"
#include "Camera.h"

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

int main(int argc, char **argv)
{
    srand(time(NULL));
    QGuiApplication app(argc, argv);

    Camera* camera = new Camera();

    QSurfaceFormat format;
    format.setSamples(16);

    /** **/
    GameWindowShader ete (50,camera,180);
    ete.setFormat(format);
    ete.setTitle("ete");
    ete.setPosition(0,0);
    ete.resize(540, 380);
    ete.show();
    /** ** /
    GameWindowShader printemps(50,camera,90);
    printemps.setFormat(format);
    printemps.setTitle("printemps");
    printemps.setPosition(700,400);
    printemps.resize(540, 380);
    printemps.show();

    /** ** /
    GameWindowShader hiver (50,camera,0);
    hiver.setFormat(format);
    hiver.setPosition(700,0);
    hiver.setTitle("hiver");
    hiver.resize(540, 380);
    hiver.show();
    /** ** /
    GameWindowShader automne (50,camera,270);
    automne.setFormat(format);
    automne.setTitle("automne");
    automne.setPosition(0,400);
    automne.resize(540, 380);
    automne.show();

    /** **/


    // A décomenter si vous voulez tester la première partie du TP
    /** ** /

    //TriangleWindow window(PYRAMIDE);
    //TriangleWindow window(ECLAIRAGE_DIFFUS);
    //TriangleWindow window(SPHERICAL_ENVIRONMENT_MAPPING);
    //TriangleWindow window(NORMAL_MAPPING);
    TriangleWindow window(GEOMETRY_DEFORMATION);

    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    /** **/

    return app.exec();

}



