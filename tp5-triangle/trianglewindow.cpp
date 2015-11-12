#include "trianglewindow.h"


#include <QVector2D>
#include <QVector3D>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};


static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *vertexShaderSource2 =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "attribute mediump vec4 texAttr;\n"
    "varying lowp vec4 col;\n"
    "varying mediump vec4 texc;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   texc = texAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *vertexShaderCelShadingSource =
    "varying vec3 Normal;\n"
    "attribute lowp vec4 colAttr;\n"

    "void main(void){\n"
    "   col = colAttr;\n"
    "   gl_FrontColor = gl_Color;\n"
    "   Normal = gl_NormalMatrix * gl_Normal;\n"
    "   gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "   gl_Position = ftransform();\n"
    "}\n";


static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";


static const char *fragmentShaderSource2 =
    "uniform sampler2D texture;\n"
    "varying mediump vec4 texc;\n"
    "void main() {\n"
    "   gl_FragColor = texture2D(texture,texc.st);\n"
    "}\n";

static const char* fragmentShaderCelShadingSource =
        "uniform sampler2D Texture0;\n"
        "varying vec3 Normal;\n"
        "void main (void){\n"
        "vec4 color = texture2D( Texture0 , vec2( gl_TexCoord[0] ) );\n"
        "gl_FragColor = color;\n"
        "}\n";
    TriangleWindow::TriangleWindow()
        : m_program(0)
        , m_frame(0)
    {
    }

GLuint TriangleWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}


void TriangleWindow::initialize()
{
    initializeLighting();
    glClearColor(0.0f,0.21f,0.21f,1.0f);
    glColor3f(1.0f,1.0f,1.0f);
   glEnable(GL_DEPTH_TEST);    //Z-Buffer
   glDepthFunc(GL_LEQUAL); // Lesser or Equals
   glEnable(GL_CULL_FACE);
   glShadeModel(GL_SMOOTH);
   initializeShaders();
   initializeTextures();
   initializeVBO();

//   glEnable(GL_TEXTURE_2D);
}

void TriangleWindow::initializeShaders() {
    //Color Shader
    m_color_program = new QOpenGLShaderProgram(this);
    m_color_program->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSource);
    m_color_program->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSource);

    //Texture Shader
    m_program = new QOpenGLShaderProgram(this);
    m_vertex_shader = new QOpenGLShader(QOpenGLShader::Vertex);
    //Texture Vertex Shader
    //m_vertex_shader->compileSourceFile(":/shaders/simples/light/shaders/simples/texture/texture.vsh");
    //Diffuse Reflection Shader
    m_vertex_shader->compileSourceFile(":/shaders/simples/light/shaders/diffuse-reflection.vsh");


    m_fragment_shader = new QOpenGLShader(QOpenGLShader::Fragment);
    //m_fragment_shader->compileSourceFile(":/shaders/simples/light/shaders/simples/texture/texture.fsh");
     m_fragment_shader->compileSourceFile(":/shaders/simples/light/shaders/diffuse-reflection.fsh");

    //m_geometry_shader = new QOpenGLShader(QOpenGLShader::Geometry);
    m_program->addShader(m_fragment_shader);
    m_program->addShader(m_vertex_shader);
//    m_program->addShader(m_geometry_shader);
    //m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    // Link shader pipeline
    if (!m_program->link())
        close();
    if(!m_color_program->link())
        close();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_texAttr = m_program->attributeLocation("texAttr");
    m_normalAttr = m_program->attributeLocation("normalAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    m_colAttr = m_program->attributeLocation("colAttr");
    //m_color_program->set

    // Bind shader pipeline for use
    //if (!m_program->bind())
      //  close();
    const GLubyte *str;
    std::cout << "OpenGL infos with gl functions" << std::endl;
    str = glGetString(GL_RENDERER);
    std::cout << "Renderer : " << str << std::endl;
    str = glGetString(GL_VENDOR);
    std::cout << "Vendor : " << str << std::endl;
    str = glGetString(GL_VERSION);
    std::cout << "OpenGL Version : " << str << std::endl;
    str = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "GLSL Version : " << str << std::endl;
}

void TriangleWindow::initializeTextures() {

    m_textures = new QOpenGLTexture*[5];
    m_textures[0]  = new QOpenGLTexture(QImage(":/grass.png").mirrored());
    m_textures[0]->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textures[0]->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textures[0]->setWrapMode(QOpenGLTexture::Repeat);

    m_textures[1] = new QOpenGLTexture(QImage(":/lava.jpg").mirrored());
    m_textures[1]->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textures[1]->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textures[1]->setWrapMode(QOpenGLTexture::Repeat);

    m_textures[2] = new QOpenGLTexture(QImage(":/sand.jpg").mirrored());
    m_textures[2]->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textures[2]->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textures[2]->setWrapMode(QOpenGLTexture::Repeat);

    m_textures[3] = new QOpenGLTexture(QImage(":/sand2.jpg").mirrored());
    m_textures[3]->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textures[3]->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textures[3]->setWrapMode(QOpenGLTexture::Repeat);

    m_textures[4] = new QOpenGLTexture(QImage(":/water.jpg").mirrored());
    m_textures[4]->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textures[4]->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textures[4]->setWrapMode(QOpenGLTexture::Repeat);

}

void TriangleWindow::initializeVBO() {
    m_vertex_buffer = new QOpenGLBuffer();
    m_index_buffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_vertex_buffer->create();
     m_vertex_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_index_buffer->create();
    m_index_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    //initializeTriangle();
    qDebug() << "after init Triangle";
}

void TriangleWindow::initializeLighting() {
    qDebug() << "start lumens";
    glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);

     // Light model parameters:
     // -------------------------------------------

     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

     // -------------------------------------------
     // Spotlight Attenuation

     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;

     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;

     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


     // -------------------------------------------
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
}


void TriangleWindow::initializeTriangle() {

    VertexData vertices[] = {
        {QVector3D(0.0f,0.707f,0),QVector2D(0.0f,0.0f)},
        {QVector3D(-0.5f,-.5f,0), QVector2D(0.5f,1.0f)},
        {QVector3D(.5,-0.5f,0), QVector2D(1.0f,.0f)}
    };

    GLushort indices[] = {
             0,  1,  2//Triangle ,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
//             4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
//             8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
//            12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
//            16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
//            20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
        };

    m_vertex_buffer->bind();
    m_vertex_buffer->allocate(vertices, 3 * sizeof(VertexData));
    m_index_buffer->bind();
    m_index_buffer->allocate(indices, 3 * sizeof(GLushort));

}

void TriangleWindow::oldWay() {
    GLfloat vertices[] = {
        -0.50f, -0.50f,-0.50f,
        0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,-0.50f
    };

    GLfloat normals[] = {
        0.5f,0.5,0.5f,
        0.0f,1.0f,0.0f,
        0.5f,-0.5,0.5f,
    };

    GLfloat textureVertices[] = {
        0.0f,0.0f,
        0.5f,1.0f,
        1.0f,0.0f
    };

    GLfloat colors[] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    };



    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_TRUE, 0, normals);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glVertexAttribPointer(m_texAttr, 2, GL_FLOAT, GL_FALSE, 0, textureVertices);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

}

void TriangleWindow::drawTriangle() {
    // Tell OpenGL which VBOs to use
       m_vertex_buffer->bind();
       m_index_buffer->bind();

       // Offset for position
       quintptr offset = 0;

       // Tell OpenGL programmable pipeline how to locate vertex position data
       int vertexLocation = m_program->attributeLocation("a_position");
       m_program->enableAttributeArray(vertexLocation);
       m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

       // Offset for texture coordinate
       offset += sizeof(QVector3D);
       // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
       int texcoordLocation = m_program->attributeLocation("a_texcoord");
       m_program->enableAttributeArray(texcoordLocation);
       m_program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

         // Draw cube geometry using indices from VBO 1
       glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Bind shading program
    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 1000.0f);

    matrix.translate(0, 0, -2);


    //
    for(int i=0;i<4;++i) {
        if(i==0) {
            matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
            //Set uniform matrix
        }
        else {
            matrix.rotate(90.0f,0,1,0);
        }
        m_textures[i]->bind();
        m_program->setUniformValue(m_matrixUniform, matrix);
        oldWay();
        m_textures[i]->release();
    }
    m_program->release();
    matrix = QMatrix4x4();
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 1, -5);
    matrix.rotate(45.0f,0,1,0);

//    m_color_program->bind();
//    m_program->setUniformValue(m_matrixUniform,matrix);

//    m_textures[4]->bind();

    glPushMatrix();
        glScalef(.30f,.30f,.30f);
        glTranslatef(-2.0f,0.,-2.0f);
    glRotatef(45* m_frame/screen()->refreshRate(),0,1,0);
//    glTranslatef(0.0f,0.,-2.0f);
    static GLUquadricObj *quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL );
//    m_textures[0]->bind();
    gluSphere( quadric , 1 , 36 , 18 );

    glPopMatrix();

    //gluDeleteQuadric(quadric);
    //m_textures[4]->release();


    //Relâche le programme
    //m_program->release();

    ++m_frame;
}
