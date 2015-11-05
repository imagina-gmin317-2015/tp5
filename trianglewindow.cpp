#include "trianglewindow.h"

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "attribute vec4 normal;\n"
    "varying lowp float col;\n"
    "varying vec4 color;\n"
    "uniform highp mat4 matrix;\n"
    "uniform vec4 light_direction;\n"
    "void main() {\n"
    "   color = colAttr;\n"
    "   col = max(dot(normal, light_direction), 0.0);\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp float col;\n"
    "varying vec4 color;\n"
    "uniform vec4 ambiant_color;\n"
    "void main() {\n"
    "   gl_FragColor = color * (ambiant_color * col);\n"
    "}\n";

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}

TriangleWindow::TriangleWindow(int w, int h) : m_program(0), m_frame(0)
{
    _width = w;
    _height = h;

    _camX = 0;
    _camY = -14;
    _camZ = -100;
    _angle = 100;

    _wireFrame = false;
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
    generateTerrain();

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_normal = m_program->attributeLocation("normal");
    m_texCoord = m_program->attributeLocation("texCoord");
    m_matrixUniform = m_program->uniformLocation("matrix");
    m_texAttr = glGetUniformLocation(m_program->programId(), "texture");
    glUniform1i(m_texAttr, 0);

    vao.create();

    vao.bind();

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.bind();

    size_t verticesSize = _map.size()*sizeof(QVector3D), colorsSize = _color.size()*sizeof(QVector3D),
            normalSize = _normal.size()*sizeof(QVector3D), texCoordSize = _texture.size()*sizeof(GLfloat);
    vbo.allocate(verticesSize + colorsSize + normalSize + texCoordSize);

    vbo.bind();
    vbo.write(0, _map.constData(), verticesSize);
    vbo.write(verticesSize, _color.constData(), colorsSize);
    vbo.write(verticesSize + colorsSize, _normal.constData(), normalSize);
    vbo.write(verticesSize + colorsSize + normalSize, _texture.constData(), texCoordSize);

    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3, 0);
    m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, verticesSize, 3, 0);
    m_program->setAttributeBuffer(m_normal, GL_FLOAT, verticesSize + colorsSize, 3, 0);
    m_program->setAttributeBuffer(m_texCoord, GL_FLOAT, verticesSize + colorsSize + normalSize, 2, 0);

    m_program->enableAttributeArray(m_posAttr);
    m_program->enableAttributeArray(m_colAttr);
    m_program->enableAttributeArray(m_normal);
    m_program->enableAttributeArray(m_texCoord);

    vao.release();

    QImage image(QString(":/heightmap-2.png"));
    texture = new QOpenGLTexture(image);

    m_program->bind();
    m_program->setUniformValue("ambiant_color", QVector4D(0.7, 0.7, 0.7, 1.0));
    m_program->setUniformValue("light_direction", QVector4D(1.0, 1.0, 1.0, 1.0));
    m_program->release();

    glEnable(GL_DEPTH_TEST);
}

void TriangleWindow::generateTerrain()
{
    float scale = .2f;

    GLfloat y1;
    GLfloat y2;
    GLfloat y3;
    GLfloat y4;

    QImage m_image;
    if (QFile::exists(":/heightmap-2.png")) {
        if(!m_image.load(":/heightmap-2.png"))
        {
            std::cout << "image non chargé ";
            exit(0);
        }
    }
    else
    {
        std::cout << "image not found ";
    }

    for(int x = 0; x < m_image.width() - 1; x++)
    {
        for(int z = 0; z < m_image.height() - 1; z++)
        {
            unsigned char* line = m_image.scanLine(z);
            unsigned char* line2 = m_image.scanLine(z+1);
            y1 = (((GLfloat)line[x*4])/255)*20;
            y2 = (((GLfloat)line[(x*4)+4])/255)*20;
            y3 = (((GLfloat)line2[(x*4)])/255)*20;
            y4 = (((GLfloat)line2[(x*4)+4])/255)*20;

            _texture.push_back(x/(m_image.width()*1.0)); _texture.push_back(z/(m_image.height()*1.0));
            _texture.push_back((x+1)/(m_image.width()*1.0)); _texture.push_back(z/(m_image.height()*1.0));
            _texture.push_back(x/(m_image.width()*1.0)); _texture.push_back((z+1)/(m_image.height()*1.0));

            _texture.push_back((x+1)/(m_image.width()*1.0)); _texture.push_back(z/(m_image.height()*1.0));
            _texture.push_back(x/(m_image.width()*1.0)); _texture.push_back((z+1)/(m_image.height()*1.0));
            _texture.push_back((x+1)/(m_image.width()*1.0)); _texture.push_back((z+1)/(m_image.height()*1.0));

            QVector3D vertex1(x*scale, y1, z*scale);
            _map.push_back(vertex1);
            _color.push_back(displayColor(y1));

            QVector3D vertex2((x+1)*scale, y2, z*scale);
            _map.push_back(vertex2);
            _color.push_back(displayColor(y2));

            QVector3D vertex3(x*scale, y3, (z+1)*scale);
            _map.push_back(vertex3);
            _color.push_back(displayColor(y3));

            QVector3D normal = QVector3D::normal(vertex1, vertex2, vertex3);
            normal.normalize();
            _normal.push_back(normal);
            _normal.push_back(normal);
            _normal.push_back(normal);

            _map.push_back(vertex2);
            _color.push_back(displayColor(y2));

            _map.push_back(vertex3);
            _color.push_back(displayColor(y3));

            QVector3D vertex4((x+1)*scale, y4, (z+1)*scale);
            _map.push_back(vertex4);
            _color.push_back(displayColor(y4));
            normal = QVector3D::normal(vertex2, vertex2, vertex4);
            normal.normalize();
            _normal.push_back(normal);
            _normal.push_back(normal);
            _normal.push_back(normal);
        }
    }
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 1.0 * _width/_height, 0.1f, 300.0f);
    matrix.translate(_camX, _camY, _camZ);
    matrix.rotate(20, 1, 0, 0);
    matrix.rotate(_angle, 0, 1, 0);
    matrix.translate(-70, 0, -40);

    m_program->setUniformValue(m_matrixUniform, matrix);

    vao.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->textureId());

    glDrawArrays(GL_TRIANGLES, 0, _map.size());

    vao.release();

    m_program->release();

    ++m_frame;
}

void TriangleWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_W:
            _wireFrame = !_wireFrame;
            break;
        case Qt::Key_Z:
            _camZ += 0.5;
            break;
        case Qt::Key_S:
            _camZ -= 0.5;
            break;
        case Qt::Key_Q:
            _camX -= 0.5;
            break;
        case Qt::Key_D:
            _camX += 0.5;
            break;
        case Qt::Key_P:
            _camY += 0.1;
            break;
        case Qt::Key_M:
            _camY -= 0.1;
            break;
        case Qt::Key_E:
            _angle += 5.0;
            break;
        case Qt::Key_A:
            _angle -= 5.0;
            break;
    }
    renderNow();
}

QVector3D TriangleWindow::displayColor(float alt)
{
    if (alt > 13.0)
    {
        return QVector3D(1.0f, 1.0f, 1.0f);
    }
    else if (alt > 10.0)
    {
        return QVector3D(0.6f, 0.4f, 0.1f);
    }
    else if (alt > 1.0)
    {
        return QVector3D(0.2, 0.5f, 0.14f);
    }
    else
    {
        return QVector3D(0.0f, 1.0f, 1.0f);
    }
}
