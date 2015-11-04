#include "trianglewindow.h"

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "attribute mediump vec2 texCoord;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "varying vec2 texture_coordinate;\n"
    "void main() {\n"
    "   if(posAttr.y > 2.8){\n"
    "       col = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "   }\n"
    "   else if(posAttr.y > 2.0){\n"
    "       col = vec4(0.6, 0.4, 0.1, 1.0);\n"
    "   }\n"
    "   else if(posAttr.y > 0.5){\n"
    "       col = vec4(0.2, 0.5, 0.14, 1.0);\n"
    "   }\n"
    "   else if(posAttr.y > 0.2){\n"
    "       col = vec4(1.0, 0.8, 0.6, 1.0);\n"
    "   }\n"
    "   else if(posAttr.y > 0.1){\n"
    "       col = vec4(0.0, 1.0, 1.0, 1.0);\n"
    "   }\n"
    "   else{\n"
    "       col = vec4(0.0, 0.9, 0.9, 1.0);\n"
    "   }\n"
    "   gl_Position = matrix * posAttr;\n"
    "   texture_coordinate = texCoord;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "varying vec2 texture_coordinate;\n"
    "uniform sampler2D texture;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
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

    _camX = -51;
    _camY = -14;
    _camZ = -115;

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
    m_texCoord = m_program->attributeLocation("texCoord");
    m_matrixUniform = m_program->uniformLocation("matrix");
    m_texAttr = glGetUniformLocation(m_program->programId(), "texture");
    glUniform1i(m_texAttr, 0);

    vao.create();

    vao.bind();

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.bind();

    size_t verticesSize = _map.size()*sizeof(GLfloat), colorsSize = _color.size()*sizeof(GLfloat),
            texCoordSize = _texture.size()*sizeof(GLfloat);
    vbo.allocate(verticesSize + colorsSize + texCoordSize);

    vbo.bind();
    vbo.write(0, _map.constData(), verticesSize);
    vbo.write(verticesSize, _color.constData(), colorsSize);
    vbo.write(verticesSize + colorsSize, _texture.constData(), texCoordSize);

    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3, 0);
    m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, verticesSize, 3, 0);
    m_program->setAttributeBuffer(m_texCoord, GL_FLOAT, verticesSize + colorsSize, 2, 0);

    m_program->enableAttributeArray(m_posAttr);
    m_program->enableAttributeArray(m_colAttr);
    m_program->enableAttributeArray(m_texCoord);

    vao.release();

    QImage image(QString(":/heightmap-2.png"));
    texture = new QOpenGLTexture(image);

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
            y1 = (((GLfloat)line[x*4])/255)*5;
            y2 = (((GLfloat)line[(x*4)+4])/255)*5;
            y3 = (((GLfloat)line2[(x*4)])/255)*5;
            y4 = (((GLfloat)line2[(x*4)+4])/255)*5;

            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);

            _texture.push_back(x/(m_image.width()*1.0)); _texture.push_back(z/(m_image.height()*1.0));
            _texture.push_back((x+1)/(m_image.width()*1.0)); _texture.push_back(z/(m_image.height()*1.0));
            _texture.push_back(x/(m_image.width()*1.0)); _texture.push_back((z+1)/(m_image.height()*1.0));

            _texture.push_back((x+1)/(m_image.width()*1.0)); _texture.push_back(z/(m_image.height()*1.0));
            _texture.push_back(x/(m_image.width()*1.0)); _texture.push_back((z+1)/(m_image.height()*1.0));
            _texture.push_back((x+1)/(m_image.width()*1.0)); _texture.push_back((z+1)/(m_image.height()*1.0));

            _map.push_back(x*scale); _map.push_back(y1); _map.push_back(z*scale);
            _map.push_back((x+1)*scale); _map.push_back(y2); _map.push_back(z*scale);
            _map.push_back(x*scale); _map.push_back(y3); _map.push_back((z+1)*scale);

            _map.push_back((x+1)*scale); _map.push_back(y2); _map.push_back(z*scale);
            _map.push_back(x*scale); _map.push_back(y3); _map.push_back((z+1)*scale);
            _map.push_back((x+1)*scale); _map.push_back(y4); _map.push_back((z+1)*scale);
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
    matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
    matrix.translate(_camX, _camY, _camZ);
    //matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    vao.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->textureId());

    glDrawArrays(GL_TRIANGLES, 0, _width * _height * 6);

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
            _camX += 0.5;
            break;
        case Qt::Key_D:
            _camX -= 0.5;
            break;
        case Qt::Key_P:
            _camY += 0.1;
            break;
        case Qt::Key_M:
            _camY -= 0.1;
            break;
    }
    renderNow();
}
