#include "trianglewindow.h"


static const char *vertexsource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "attribute mediump vec2 Texcoord;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "varying vec2 texture_coordinate;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position =matrix*posAttr;\n"
    "   texture_coordinate = vec2(Texcoord);\n"
    "}\n";



static const char *fragmentsource =
    "varying vec2 texture_coordinate;\n"
     "uniform sampler2D Texture0;\n"
     "uniform sampler2D Texture1;\n"
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = texture2D(Texture1, texture_coordinate);;\n"
    "}\n";


//static const char *vertexsourcemap =
//        "attribute highp vec4 posAttr;\n"
//        "attribute vec4 normal;\n"
//        "varying lowp float col;\n"
//        "varying vec4 color;\n"
//        "uniform highp mat4 matrix;\n"
//        "uniform vec4 light_direction;\n"
//        "void main() {\n"
////        "   color = vec4(0.54,0.5,0.5,0);\n"
//       "if (posAttr.z<0.05) \n"
//        "{ \n"
//        "color = vec4(0.1,0.1,0.9,0);\n"
//        "}\n"
//        "else if (posAttr.z<0.1)\n"
//        "{ \n"
//        "color = vec4(0.054,0.85,0.05,0);\n "
//        "}\n"
//       "else if (posAttr.z<0.8)\n"
//       "{ \n"
//       "color = vec4(0.3,0.35,0.35,0);\n "
//       "}\n"
//       "else \n"
//       "{ \n"
//       "color = vec4(0.5,0.55,0.55,0);\n "
//       "}\n"
//        "   col = max(dot(normal, light_direction), 0.0);\n"
//        "   gl_Position = matrix * posAttr;\n"
//        "}\n";



//static const char *fragmentsourcemap =
//        "varying lowp float col;\n"
//        "varying vec4 color;\n"
//        "uniform vec4 ambiant_color;\n"
//        "void main() {\n"
//        "   gl_FragColor = color * (ambiant_color * col);\n"
////        "   gl_FragColor = color;\n"
//        "}\n";



static const char *vertexsourceparticle =
        "attribute highp vec4 posAttr;\n"
        "uniform highp mat4 matrix;\n"
                "varying vec4 color;\n"
        "void main() {\n"
        "   color = vec4(0.54,0.5,0.5,0);\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";



static const char *fragmentsourceparticle =
        "varying vec4 color;\n"
        "void main() {\n"
        "   gl_FragColor = color;\n"
        "}\n";



TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}

TriangleWindow::TriangleWindow(Camera *cam)
    : m_program(0)
    , m_frame(0)
{

    this->cam=cam;
    qDebug()<<"cam persp :"<<cam->anglePerspective;
}

TriangleWindow::~TriangleWindow(){}

GLuint TriangleWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}



void TriangleWindow::render()
{
    glClearDepth(1.0f);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_LIGHT0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);



      QMatrix4x4 matrix;
      matrix.perspective(cam->anglePerspective, 16.0f/9.0f, 0.1f, 100.0f);
      matrix.translate(cam->transX, cam->transY, cam->transZ);
    matrix.rotate(cam->rotX, 1, 0, 0);
    matrix.rotate(cam->rotY, 0, 1, 0);
    matrix.rotate(cam->rotZ, 0, 0, 1);

    QVector4D light(1.0,1.0,1.0,1.0);
    QVector4D ambient(0.5,0.5,0.5,1.0);


      m_program->bind();
      m_program->setUniformValue( m_matrixUniform, matrix);
      //qDebug()<<"token";
      vao.bind();


      glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]->textureId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]->textureId());

        glUniform1i(t1, 0);
        glUniform1i(t2, 1);



      glEnable(GL_DEPTH_TEST);
      glDrawArrays(GL_TRIANGLES, 0, 18);

      vao.release();

      m_program->release();



      map_program->bind();
      map_program->setUniformValue( map_matrixUniform, matrix);
      map_program->setUniformValue( map_light, light);
      map_program->setUniformValue( map_ambient, ambient);
//      qDebug()<<"token";
      map_vao.bind();

      glEnable(GL_DEPTH_TEST);
      glDrawArrays(GL_TRIANGLES, 0, mapLength);

      map_vao.release();

      map_program->release();
 //     qDebug()<<"tick";

      part_program->bind();

      part_program->setUniformValue( part_matrixUniform, matrix);
      part_vao.bind();

      part_vbo[0].bind();
      part_vbo[0].allocate(particles.size()*sizeof(particles[0]));
      QVector<GLfloat> out;
      for (int i= 0; i<particles.size();i++)
      {
          out<<particles[i].x()<<particles[i].y()<<particles[i].z();
      }
      part_vbo[0].write(0, out.constData(), particles.size()*sizeof(particles[0]));

      part_program->setAttributeBuffer(part_posAttr, GL_FLOAT, 0, 3, 0);


glDrawArrays(GL_POINTS, 0, particles.size()*sizeof(particles[0]));
      part_vao.release();

      part_program->release();

      ++m_frame;
}


void TriangleWindow::initialize(){
    qDebug()<<"initilize start";



    initPyramid();
    initMap(":/heightmap-2.png");
    initPart(25);

    qDebug()<<"initilisation succeed";
}


void TriangleWindow::initPyramid(){



    QVector<QVector3D> pos;

    QVector3D p;
    p.setX(-0.5);p.setY(0.3);p.setZ(0.5);pos<<p;
    p.setX(-0.5);p.setY(0.3);p.setZ(-0.5);pos<<p;
    p.setX(0.5);p.setY(0.3);p.setZ(-0.5);pos<<p;
    p.setX(0.5);p.setY(0.3);p.setZ(0.5);pos<<p;
    p.setX(0.0);p.setY(1.0);p.setZ(0.0);pos<<p;



    QVector<QVector2D> texCoord;
    QVector2D t;
    t.setX(0);t.setY(0);texCoord<<t;
    t.setX(0);t.setY(1);texCoord<<t;
    t.setX(1);t.setY(1);texCoord<<t;
    t.setX(1);t.setY(0);texCoord<<t;
    t.setX(0.5);t.setY(0.5);texCoord<<t;

    QVector<GLint> index;
        index<<0<<1<<2;
        index<<0<<2<<3;
        index<<0<<1<<4;
        index<<1<<2<<4;
        index<<2<<3<<4;
        index<<3<<0<<4;

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexsource);

    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentsource);
    m_program->link();

    m_program->bind();


    m_posAttr=m_program->attributeLocation("posAttr");
    m_colAttr=m_program->attributeLocation("colAttr");
    m_texAttr=m_program->attributeLocation("Texcoord");
    m_matrixUniform = m_program->uniformLocation("matrix");
    /* Allocate and assign a Vertex Array Object to our handle */

    vao.create();

    /* Bind our Vertex Array Object as the current used object */
    vao.bind();

    /* Allocate and assign two Vertex Buffer Objects to our handle */

    int posSize=sizeof(pos[0])*index.size(), texSize=sizeof(texCoord[0])*index.size();

    vbo[0].create();
    //vbo[1].create();
    vbo[2].create();

    const float* temp;

    vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo[0].bind();
    vbo[0].allocate(posSize);
    QVector<GLfloat> out;
    for (int i= 0; i<index.size();i++)
    {
        out<<pos[index[i]].x()<<pos[index[i]].y()<<pos[index[i]].z();
        //qDebug()<<obj[index[i]].x<<obj[index[i]].y<<obj[index[i]].z;
    }
    vbo[0].write(0, out.constData(), posSize);

    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3, 0);



    vbo[2].setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo[2].bind();
    vbo[2].allocate(texSize);
    for (int i= 0; i<index.size();i++)
    {
        out<<texCoord[index[i]].x()<<texCoord[index[i]].y();
    }
    vbo[2].write(0,out.constData(),texSize);
    m_program->setAttributeBuffer(m_texAttr, GL_FLOAT, 0, 2, 0);


        qDebug()<<"still no crash ..";





    m_program->enableAttributeArray(m_posAttr);
    //program->enableAttributeArray(col);
    m_program->enableAttributeArray(m_texAttr);
        qDebug()<<"still no crash ..";

 m_program->release();
    vao.release();

    QImage image(QString(":/grass_texture.jpg"));
    texture << new QOpenGLTexture(image);

    image.load(":/diamond_texture.jpg");
    texture << new QOpenGLTexture(image);

    t1 = glGetUniformLocation(m_program->programId(), "Texture0");
    t2 = glGetUniformLocation(m_program->programId(), "Texture1");
qDebug()<<"pyramid init finished ..";
}

void TriangleWindow::initMap(QString localPath){

    if (QFile::exists(localPath)) {
        m_image = QImage(localPath);
    }


    uint id = 0;


    QVector3D p;
//    point3D *p = new point3D[m_image.width() * m_image.height()];
    QRgb pixel;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*m_image.width() +j;

//            p[id].x = (float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width();
//            p[id].y = (float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height();
//            p[id].z = 0.001f * (float)(qRed(pixel));
            p.setX((float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width());
            p.setY((float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height());
            p.setZ(0.001f * (float)(qRed(pixel)));
            position<<p;

        }
    }
    ArrayTools tool(m_image.height(),m_image.width());
    //ArrayTools tool(m_image.width(),m_image.height());
    tool.setTriangleMotif(triangle);
    tool.makeIndex();
    tool.setPoints(position);
    QVector<GLint> index = tool.getIndex();


    map_program = new QOpenGLShaderProgram(this);
//    map_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexsourcemap);

//    map_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentsourcemap);
    map_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex_map_shader_v1");

    map_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment_map_shader_v1");
    map_program->link();

    map_program->bind();


    map_posAttr=map_program->attributeLocation("posAttr");
    map_normals=map_program->attributeLocation("normal");
    map_matrixUniform = map_program->uniformLocation("matrix");
    map_light = map_program->uniformLocation("light_direction");
    map_ambient = map_program->uniformLocation("ambiant_color");
    /* Allocate and assign a Vertex Array Object to our handle */

    map_vao.create();

    /* Bind our Vertex Array Object as the current used object */
    map_vao.bind();

    /* Allocate and assign two Vertex Buffer Objects to our handle */

    int posSize=sizeof(position[0])*index.size();

    map_vbo[0].create();



    mapLength=posSize;

    map_vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
    map_vbo[0].bind();
    map_vbo[0].allocate(posSize);
    QVector<GLfloat> out;
    for (int i= 0; i<index.size();i++)
    {
        out<<position[index[i]].x()<<position[index[i]].y()<<position[index[i]].z();
        //qDebug()<<position[index[i]].x()<<position[index[i]].y()<<position[index[i]].z();
    }
    //qDebug()<<"out"<<out.size()*sizeof(GLfloat);

    //qDebug()<<"still no crash in map right here.. PosSize"<<posSize;
    map_vbo[0].write(0, out.constData(), posSize);
    map_program->setAttributeBuffer(map_posAttr, GL_FLOAT, 0, 3, 0);


    map_vbo[1].create();


    map_vbo[1].setUsagePattern(QOpenGLBuffer::StaticDraw);
    map_vbo[1].bind();
    map_vbo[1].allocate(posSize);

    tool.buildNormals();
    QVector<QVector3D> temp=tool.getNormals();
    for (int i= 0; i<temp.size();i++)
    {
        out<<temp[i].x()<<temp[i].y()<<temp[i].z();

    }
qDebug()<<"still no crash in map..";


    //qDebug()<<"still no crash in map right here.. PosSize"<<posSize;
    map_vbo[1].write(0, out.constData(), posSize);
    map_program->setAttributeBuffer(map_normals, GL_FLOAT, 0, 3, 0);






        qDebug()<<"still no crash in map..";

    map_program->enableAttributeArray(map_posAttr);
    map_program->enableAttributeArray(map_normals);

    map_program->release();
    map_vao.release();




}

void TriangleWindow::initPart(int nPart){
    part_timer = new QTimer();

    QVector3D p;
    for (int i =0;i<position.size();i+=nPart)
    {
        p.setX(position[i].x());
        p.setY(position[i].y());
        p.setZ(1.5);
        particles<<p;
        floor<<position[i].z();
        speed<<(float) 0.2+(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) ) / 10;
    }



    part_program = new QOpenGLShaderProgram(this);
    part_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexsourceparticle);

    part_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentsourceparticle);
    part_program->link();

    part_program->bind();


    part_posAttr=part_program->attributeLocation("posAttr");
    part_matrixUniform = map_program->uniformLocation("matrix");


    part_vao.create();
    part_vao.bind();



    part_vbo[0].create();


    part_vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
    part_vbo[0].bind();
    part_vbo[0].allocate(particles.size()*sizeof(particles[0]));
    QVector<GLfloat> out;
    for (int i= 0; i<particles.size();i++)
    {
        out<<particles[i].x()<<particles[i].y()<<particles[i].z();
        //qDebug()<<obj[index[i]].x<<obj[index[i]].y<<obj[index[i]].z;
    }
    part_vbo[0].write(0, out.constData(), particles.size()*sizeof(particles[0]));

    part_program->setAttributeBuffer(part_posAttr, GL_FLOAT, 0, 3, 0);




        qDebug()<<"still no crash ..";





    part_program->enableAttributeArray(part_posAttr);

        qDebug()<<"still no crash ..";

    part_program->release();
    part_vao.release();

    connect(part_timer,SIGNAL(timeout()),this, SLOT(updatePart()));
    connect(part_timer,SIGNAL(timeout()),this, SLOT(renderNow()));
    part_timer->start(100);
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
float transFactor=0.3;
//matrix.translate(0, 0, -2);
//    matrix.rotate(cam->rotZ * m_frame / screen()->refreshRate(), 0, 0, 1);
    switch(event->key())
    {
    case 'Z':
        cam->anglePerspective += 1.0f;
//        cam->matrix.perspective(cam->anglePerspective, 16.0f/9.0f, 0.1f, 100.0f);
        break;
    case 'S':
        cam->anglePerspective -= 1.0f;
//        cam->matrix.perspective(cam->anglePerspective, 16.0f/9.0f, 0.1f, 100.0f);
        break;
    case 'A':
        cam->rotX += 1.0f;
//        cam->matrix.rotate(cam->rotX * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'E':
        cam->rotX -= 1.0f;
//        cam->matrix.rotate(cam->rotX * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'Q':
        cam->rotY += 1.0f;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'D':
        cam->rotY -= 1.0f;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'W':
        cam->rotZ += 1.0f;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'C':
        cam->rotZ -= 1.0f;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;

        // Translation
    case 'R':
        cam->transX += transFactor;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'T':
        cam->transX -= transFactor;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'F':
        cam->transY += transFactor;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'G':
        cam->transY -= transFactor;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'V':
        cam->transZ += transFactor;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;
    case 'B':
        cam->transZ -= transFactor;
//        cam->matrix.rotate(cam->rotY * m_frame / screen()->refreshRate(), 0, 1, 0);
        break;

    }
    renderNow();
}

void TriangleWindow::updatePart(){
    for (int i=0;i<particles.size();i++)
    {
        (particles[i].z()-speed[i] < floor[i]) ? particles[i].setZ(1.5) : particles[i].setZ(particles[i].z()-speed[i]);
    }

}

const float* TriangleWindow::buildArrayFromIndex(QVector<point2D> obj, QVector<GLint> index){
    QVector<GLfloat> out;
    for (int i= 0; i<index.size();i++)
    {
        out<<obj[index[i]].x<<obj[index[i]].y;
    }
    return out.constData();
}

const float* TriangleWindow::buildArrayFromIndex(QVector<point3D> obj, QVector<GLint> index){
    QVector<GLfloat> out;
//    qDebug()<<(int) sizeof(*index);
    for (int i= 0; i<index.size();i++)
    {
        out<<obj[index[i]].x<<obj[index[i]].y<<obj[index[i]].z;
        //qDebug()<<obj[index[i]].x<<obj[index[i]].y<<obj[index[i]].z;
    }
    qDebug()<<"out"<<out.size()*sizeof(GLfloat);
    qDebug()<<out.constData()[0];
    return out.constData();
}
