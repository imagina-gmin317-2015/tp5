#include <QtCore>
#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLTexture>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize() Q_DECL_OVERRIDE;
    void initializeShaders();
    void initializeTextures();
    void initializeVBO();
    void initializeTriangle();
    void initializeLighting();


    void oldWay();
    void drawTriangle();
    void render() Q_DECL_OVERRIDE;

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_normalAttr;
    GLuint m_texAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_color_program;
    QOpenGLShaderProgram *m_texture_program;

    QOpenGLShader *m_geometry_shader;
    QOpenGLShader *m_vertex_shader;
    QOpenGLShader *m_fragment_shader;

    QOpenGLTexture* m_grass_texture;
    QOpenGLTexture** m_textures;

    QOpenGLBuffer* m_vertex_buffer;
    QOpenGLBuffer* m_normal_buffer;
    QOpenGLBuffer* m_index_buffer;


    int m_frame;
};
