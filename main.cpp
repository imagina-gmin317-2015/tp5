/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

#include <QMouseEvent>
#include <QKeyEvent>

#include <QOpenGLTexture>

#include "MeshParser.h"

//! [1]
class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;
protected:
	void generateGeometry( const char *heightMap_fileName, unsigned int sizeX, unsigned int sizeY, GLfloat *out_result, unsigned short *out_indices, unsigned int *nbIndices, GLfloat *out_uv, unsigned short *out_indices2, unsigned int *nbIndices2 );

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_uvAttr;
	GLuint m_matrixUniform; 
	GLuint m_CamPos;
	GLuint m_sphM;
	GLuint m_nmAttr;

    QOpenGLShaderProgram *m_program;
    int m_frame;

	// taille du terrain à générer, en nombre de faces par cote
	unsigned int squareSize;

	// donnees
    GLfloat *vertices;
    unsigned short *indices;
    unsigned int nbIndices;

    unsigned short *indices2;
    unsigned int nbIndices2;

    //GLfloat *color;
	GLfloat *uv;

	// textures
	QOpenGLTexture *tex1;
	QOpenGLTexture *tex2;

	// gestion de la camera
    GLfloat camPosX, camPosY, camPosZ;
    GLfloat camRotX, camRotY, camRotZ;

	// buffers
	GLuint vbo;
	GLuint ibo;

	// evenements souris/clavier
    void mouseMoveEvent ( QMouseEvent * event );
    void keyPressEvent(QKeyEvent *event);

	// sphere
	QOpenGLTexture *sphereTex;
	QOpenGLTexture *sphereNM;
	QOpenGLTexture *pyTexture;
	QOpenGLTexture *pyTexture2;
	QOpenGLTexture *pyTexture3;
	QOpenGLTexture *pyTexture4;
	MeshParser sphere;
	GLfloat *uvSphere;
};

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}
//! [1]

//! [2]
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
//! [2]


//! [3] GLSL vertex shader
static const char *vertexShaderSource =
	"attribute highp vec4 posAttr;\n"
	"attribute highp vec3 nmAttr;\n"
	"attribute lowp vec2 uvAttr;\n"
	"varying lowp vec2 uv;\n"
	"varying lowp vec4 p;\n"
	"varying lowp vec3 p2;\n"
	"varying lowp vec3 nm;\n"
	"uniform highp float sphM;\n"
	"uniform lowp mat4 matrix;\n"
	"uniform lowp vec3 CamPos;\n"
	"void main() {\n"
	"   uv = uvAttr;\n"
	"	p = posAttr;\n"
	"	vec4 wvp_pos = (matrix * posAttr);\n"
	"	p2 = normalize(wvp_pos.xyz - CamPos);\n"
	"	nm = nmAttr;\n"
	"   gl_Position = wvp_pos;\n"
	"}\n";

//! GLSL pixel shader
static const char *fragmentShaderSource =
	"varying lowp vec2 uv;\n"
	"varying lowp vec3 nm;\n"
	"varying lowp vec4 p;\n"
	"varying lowp vec3 p2;\n"
	"uniform highp float sphM;\n"
	"uniform sampler2D texSamp;\n"
	"uniform sampler2D nmSamp;\n"
	"void main() {\n"
	"	vec3 lightDir = vec3(0.5, 1.1, 0.5) - p.xyz;\n"
	"	vec3 n = texture2D(nmSamp, uv).rgb * 2.f - 1.f;\n"
	"	vec4 c;float i;\n"
	"	if(sphM < 0) {\n"
	"		vec3 n2 = normalize(nm + (n / 16.0f));\n"
	"		vec3 r = normalize(reflect(p2, n2));\n"
	"		float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );\n"
	"		float u1 = r.x/m + 0.5;\n"
	"		float v1 = r.y/m + 0.5;\n"
	"		c=texture2D(texSamp, vec2(u1, v1));i = 1;\n"
	"	} else {\n"
	"	c = texture2D(texSamp, uv);\n"
	"	i = dot(lightDir, n);\n"
	"	i = clamp(i, 0.2, 1);\n"
	"	c.rgb *= pow(clamp(p.y, 0, 1), 5);}\n"
	"   gl_FragColor = c * i;\n"
	"}\n";
//! [3]

//! [4] chargement des shader
GLuint TriangleWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

//! initialisation
void TriangleWindow::initialize()
{
	// Enable depth buffer
	glEnable( GL_DEPTH_TEST );

	// Enable back face culling
	glEnable( GL_CULL_FACE );

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
	m_uvAttr = m_program->attributeLocation( "uvAttr" );
	m_matrixUniform = m_program->uniformLocation( "matrix" );
	m_CamPos = m_program->uniformLocation( "CamPos" );
	m_sphM = m_program->uniformLocation( "sphM" );
	m_nmAttr = m_program->attributeLocation( "nmAttr" );

	//-----------------------------------------------------------------
	// texture
	tex1 = new QOpenGLTexture(QImage( "./ground.png" ));

	// Set nearest filtering mode for texture minification
	tex1->setMinificationFilter( QOpenGLTexture::NearestMipMapLinear );

	// Set bilinear filtering mode for texture magnification
	tex1->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	tex1->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	tex2 = new QOpenGLTexture( QImage( "./nm.png" ) );

	// Set nearest filtering mode for texture minification
	tex2->setMinificationFilter( QOpenGLTexture::Nearest );

	// Set bilinear filtering mode for texture magnification
	tex2->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	tex2->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	sphereTex = new QOpenGLTexture( QImage( "./reflection.png" ) );

	// Set nearest filtering mode for texture minification
	sphereTex->setMinificationFilter( QOpenGLTexture::Nearest );

	// Set bilinear filtering mode for texture magnification
	sphereTex->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	sphereTex->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	sphereNM = new QOpenGLTexture( QImage( "./ice.png" ) );

	// Set nearest filtering mode for texture minification
	sphereNM->setMinificationFilter( QOpenGLTexture::Nearest );

	// Set bilinear filtering mode for texture magnification
	sphereNM->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	sphereNM->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	pyTexture = new QOpenGLTexture( QImage( "./pyramide.png" ) );

	// Set nearest filtering mode for texture minification
	pyTexture->setMinificationFilter( QOpenGLTexture::Nearest );

	// Set bilinear filtering mode for texture magnification
	pyTexture->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	pyTexture->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	pyTexture2 = new QOpenGLTexture( QImage( "./pyramide2.png" ) );

	// Set nearest filtering mode for texture minification
	pyTexture->setMinificationFilter( QOpenGLTexture::Nearest );

	// Set bilinear filtering mode for texture magnification
	pyTexture->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	pyTexture->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	pyTexture3 = new QOpenGLTexture( QImage( "./pyramide3.png" ) );

	// Set nearest filtering mode for texture minification
	pyTexture->setMinificationFilter( QOpenGLTexture::Nearest );

	// Set bilinear filtering mode for texture magnification
	pyTexture->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	pyTexture->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	pyTexture4 = new QOpenGLTexture( QImage( "./pyramide4.png" ) );

	// Set nearest filtering mode for texture minification
	pyTexture->setMinificationFilter( QOpenGLTexture::Nearest );

	// Set bilinear filtering mode for texture magnification
	pyTexture->setMagnificationFilter( QOpenGLTexture::Linear );

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	pyTexture->setWrapMode( QOpenGLTexture::Repeat );

	//-----------------------------------------------------------------
	// on va generer un carre de 240 faces de cote
	squareSize = 240;

	// allocation memoire pour stocker les donnees
	vertices = (GLfloat *)malloc( ( squareSize + 1 ) * ( squareSize + 1 ) * 3 * sizeof( GLfloat ) );
	indices = (unsigned short *)malloc( squareSize * squareSize * 6 * sizeof( unsigned short ) );
	indices2 = (unsigned short *)malloc( squareSize * squareSize * 10 * sizeof( unsigned short ) );
	//color = (GLfloat *)malloc( ( squareSize + 1 ) * ( squareSize + 1 ) * 3 * sizeof( GLfloat ) );
	uv = (GLfloat *)malloc( ( squareSize + 1 ) * ( squareSize + 1 ) * 2 * sizeof( GLfloat ) );

	// generation de la geometrie
	generateGeometry( "heightmap-1.png", squareSize, squareSize, vertices, indices, &nbIndices, uv, indices2, &nbIndices2 );

	// initialisation de la camera
    camPosX = 0;
    camPosY = 0;
    camPosZ = 0;

    camRotX = 0;
    camRotY = 0;
    camRotZ = 0;

	sphere.from_PLY_file( "./sphere.ply" );
	uvSphere = (GLfloat *)malloc( sphere.nbVertex * 2 * sizeof( GLfloat ) );
	int idx = 0;
	for( int i = 0; i != sphere.nbVertex; ++i ) {
		float x = sphere.vertex[i * 3] * 2;
		float y = sphere.vertex[i * 3 + 1] * 2;
		float z = sphere.vertex[i * 3 + 2] * 2;
		uvSphere[idx++] = 0.5f + (atan2f(z, x) / (2.0f * 3.1415926535f));
		uvSphere[idx++] = 0.5f - (asinf(y) / 3.1415926535f);

	}

	// activation du Z-BUFFER, aussi appele depth-buffer permettant de choisir, et d'effectuer, un depth test lors du rendu
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
}

//! evenement mouvement de souris
void TriangleWindow::mouseMoveEvent ( QMouseEvent * event ){
    static const qreal retinaScale = devicePixelRatio();
    static float hei = height() * retinaScale;
    static float wid = width() * retinaScale;

	// recuperation des donnees de deplacement de la souris. Les donnees de position sont insuffisantes.
    static int mouseLastPosX = -1, mouseLastPosY = -1;

    int effectiveMoveX, effectiveMoveY;

    // init data
    if(mouseLastPosX == -1 || mouseLastPosY == -1){
        mouseLastPosX = event->x();
        mouseLastPosY = event->y();
        return;
    }

    effectiveMoveX = mouseLastPosX - event->x();
    effectiveMoveY = mouseLastPosY - event->y();

	// application de la rotation en fonction du mouvement de la souris
    camRotY -= (GLfloat)(((float)effectiveMoveX / wid) * 360.f);
    camRotY = fmodf(camRotY, 360.0f);

    camRotX += (GLfloat)(((float)effectiveMoveY / hei) * 360.f);
	if( camRotX > 80 ) camRotX = 80; else if( camRotX < -80 ) camRotX = -80;

	// sauvegarde pour calcul posterieur
    mouseLastPosX = event->x();
    mouseLastPosY = event->y();
}

//! gestion de l'evenement d'appui sur une touche du clavier
//! Z Q S D pour avancer (touches traditionnelles)
void TriangleWindow::keyPressEvent(QKeyEvent *event){
    // --> 6 DDL
	// on convertis rotation -> vecteur directeur
    float camRotX_rad = (camRotX / 360.0f) * 6.2831853f;
    float camRotY_rad = (camRotY / 360.0f) * 6.2831853f;
    QVector3D cameraDirection(cosf(camRotY_rad), sinf(camRotX_rad), sinf(camRotY_rad));
    QVector3D cameraDirection_left(cosf(camRotY_rad + 1.57079632675f), sinf(camRotX_rad), sinf(camRotY_rad + 1.57079632675f));
    float speed = 0.05f; // vitesse de la camera

    if(event->key() == Qt::Key_Z){
        camPosX += cameraDirection.x() * speed;
        camPosY += cameraDirection.y() * speed;
        camPosZ += cameraDirection.z() * speed;
    } else if(event->key() == Qt::Key_S){
        camPosX -= cameraDirection.x() * speed;
        camPosY -= cameraDirection.y() * speed;
        camPosZ -= cameraDirection.z() * speed;
    }

    if(event->key() == Qt::Key_Q){
        camPosX -= cameraDirection_left.x() * speed;
        camPosY -= cameraDirection_left.y() * speed;
        camPosZ -= cameraDirection_left.z() * speed;
    } else if(event->key() == Qt::Key_D){
        camPosX += cameraDirection_left.x() * speed;
        camPosY += cameraDirection_left.y() * speed;
        camPosZ += cameraDirection_left.z() * speed;
    }
}

//! [4] generation de la géométrie. Permet de generer un terrain à partir d'une image quelconque, codée en RGBA. Retourne, via pointeurs, le terrain en fil de fer, ainsi que le terrain "normal"
//! il est possible de rendre les donnees a l'ecran via VBO/IBO ou sans.
void TriangleWindow::generateGeometry(const char *heightMap_fileName, unsigned int sizeX, unsigned int sizeY, GLfloat *out_result, unsigned short *out_indices, unsigned int *nbIndices, GLfloat *out_uv, unsigned short *out_indices2, unsigned int *nbIndices2){
    int idx = 0;
    int idx2 = 0;
	int nbVertex;
	int nbUV = 0;

	// chargement de la height map
	QImage pic;
	bool b = pic.load( heightMap_fileName );

	// generation des couleurs et des vertex
    for(int i = 0; i != sizeX+1; ++i){
        for(int j = 0; j != sizeY+1; ++j){
			int picX, picY;
			picX = (int)( ( (float)i / (float)( sizeX + 1 ) ) * pic.width() );

			picY = (int)( ( (float)j / (float)( sizeY + 1 ) ) * pic.height() );

			unsigned char *rgbData = pic.scanLine( picY );
			unsigned char height = rgbData[picX*4];

            out_result[idx++] = (1.0f / (float)sizeX) * i;
			out_result[idx++] = 1.0f - (1.0f / (float)height); 
			out_result[idx++] = ( 1.0f / (float)sizeY ) * j;

			/*out_colors[idx2++] = out_result[idx - 2] * out_result[idx - 2] * out_result[idx - 2];
			out_colors[idx2++] = out_result[idx - 2] * out_result[idx - 2] * out_result[idx - 2];
			out_colors[idx2++] = out_result[idx - 2] * out_result[idx - 2] * out_result[idx - 2];*/
			out_uv[idx2++] = ((float)i / (float)( sizeX + 1 )) * 4.0f;
			out_uv[idx2++] = ((float)j / (float)( sizeY + 1 )) * 4.0f;
			//qDebug() << out_uv[idx2 - 2] << "; " << out_uv[idx2 - 1];
        }
    }

	nbVertex = idx;
	nbUV = idx2;

	// generation des (indices fil de fer & normal)
    idx = 0;
    idx2 = 0;
    for(int y = 0; y != sizeY; ++y){
        for(int x = 0; x != sizeX; ++x){
            out_indices2[idx2++] = x + y * (sizeX + 1); //v1
            out_indices2[idx2++] = x + 1 + y * (sizeX + 1); //v2

            out_indices2[idx2++] = x + 1 + y * (sizeX + 1); //v2
            out_indices2[idx2++] = x + 1 + (y + 1) * (sizeX + 1); //v4

            out_indices2[idx2++] = x + 1 + (y + 1) * (sizeX + 1); //v4
            out_indices2[idx2++] = x + y * (sizeX + 1); //v1

            out_indices2[idx2++] = x + 1 + (y + 1) * (sizeX + 1); //v4
            out_indices2[idx2++] = x + (y + 1) * (sizeX + 1); //v3

            out_indices2[idx2++] = x + (y + 1) * (sizeX + 1); //v3
            out_indices2[idx2++] = x + y * (sizeX + 1); //v1

            /////////////////////////////////////////////////////////

            out_indices[idx] = x + y * (sizeX + 1); //v1
            ++idx;
            out_indices[idx] = x + 1 + y * (sizeX + 1); //v2
            ++idx;
            out_indices[idx] = x + 1 + (y + 1) * (sizeX + 1); //v4
            ++idx;

            out_indices[idx] = x + y * (sizeX + 1); //v1
            ++idx;
            out_indices[idx] = x + 1 + (y + 1) * (sizeX + 1); //v4
            ++idx;
            out_indices[idx] = x + (y + 1) * (sizeX + 1); //v3
            ++idx;
        }
    }

    *nbIndices = idx;
    *nbIndices2 = idx2;

	// generation du VBO et de l'IBO
	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER,                   /* target */

				  ( nbVertex*sizeof( GLfloat ) ) +  /* taille des positions */

				  ( nbVertex*sizeof(GLfloat )),		/* taille des couleurs */

				  NULL,                           /* ... */

				  GL_STREAM_DRAW );                /* mode */



	/* on specifie les donnees */

	glBufferSubData( GL_ARRAY_BUFFER,

					 0,                            /* emplacement des donnees dans le VBO */

					 ( nbVertex*sizeof( GLfloat )  ), /* taille des donnees */

					 &vertices[0] );                         /* adresse des donnees */



	glBufferSubData( GL_ARRAY_BUFFER,

					 ( nbVertex*sizeof( GLfloat ) ),   /* emplacement */

					 ( nbUV*sizeof( GLfloat ) ),/* taille */

					 &uv[0] );                        /* donnees */



	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

	glBufferData( GL_ELEMENT_ARRAY_BUFFER, ( *nbIndices ) * sizeof( unsigned short ), &out_indices[0], GL_STREAM_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

//! [5] rendu de la scene
void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    m_program->bind();
	tex1->bind( 0 );
	tex2->bind( 1 );

    // la matrice finale = matWORLD * matVIEW * matPROJ
    // on place donc la caméra; la matVIEW
	// OpenGL -> matrices inversees... donc cacul inverse; matPROJ * matVIEW * matWORLD

    QMatrix4x4 matWorld, matView, matProj;
    matWorld.setToIdentity();
    matView.setToIdentity();
    matProj.setToIdentity();

    matWorld.rotate(0,45,0);
    matWorld.translate(0, 0, -2);
    matWorld.scale(2,2,2);

	// calcul du vecteur directeur de la camera
    float camRotX_rad = (camRotX / 360.0f) * 6.2831853f;
    float camRotY_rad = (camRotY / 360.0f) * 6.2831853f;
    QVector3D cameraDirection(camPosX + cosf(camRotY_rad), camPosY + sinf(camRotX_rad), camPosZ + sinf(camRotY_rad));

    matView.lookAt(QVector3D(camPosX, camPosY, camPosZ), cameraDirection, QVector3D(0, 1, 0));

    matProj.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);

	

	m_program->setUniformValue( m_matrixUniform, matProj * matView * matWorld );
	m_program->setUniformValue( m_CamPos, QVector3D( camPosX, camPosY, camPosZ ) );
	m_program->setUniformValue( "texSamp", 0 );
	m_program->setUniformValue( "nmSamp", 1 );
	m_program->setUniformValue( m_sphM, 42.0f );


    /*GLfloat vertices[] = {
        0.0f, 0.707f, 0.1f,
        -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, 0.7f
    };*/

    /*GLfloat colors[] = {
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f
    };*/

    /*glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, color);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements( GL_LINES, squareSize * squareSize * 10, GL_UNSIGNED_SHORT, &indices2[0] );

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);*/

	// dessin de la scene a l'aide du VBO et de l'IBO
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, false, 0, 0 );
	glVertexAttribPointer( m_uvAttr, 2, GL_FLOAT, false, 0, (void *)( ( squareSize + 1 ) * ( squareSize + 1 ) * 3 * sizeof( GLfloat ) ) );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );


	glDrawElements( GL_TRIANGLES, squareSize * squareSize * 6, GL_UNSIGNED_SHORT, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );

	// ------------------------------------------------------------------
	// pyramide :
	// ------------------------------------------------------------------

	pyTexture->bind( 0 );

	GLfloat vertices1[] = {
		0.2, 1.0f, -0.2f,
		-0.2, 1.0f, -0.2f,
		0, 1.5, 0,
	};

	GLfloat uv1[] = {
		0, 0,
		1, 0,
		0.5, 0.5
	};

	glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices1);
	glVertexAttribPointer(m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, uv1);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// ---------------
	pyTexture2->bind( 0 );

	GLfloat vertices2[] = {
		0.2, 1.0f, 0.2f,
		0, 1.5, 0,
		-0.2, 1.0f, 0.2f,
	};

	GLfloat uv2[] = {
		0, 0,
		1, 0,
		0.5, 0.5
	};

	glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices2 );
	glVertexAttribPointer( m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, uv2 );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 0 );

	// ---------------

	pyTexture3->bind( 0 );

	GLfloat vertices3[] = {
		0.2, 1.0f, 0.2f,
		0.2, 1.0f, -0.2f,
		0, 1.5, 0,
	};

	GLfloat uv3[] = {
		0, 0,
		1, 0,
		0.5, 0.5
	};

	glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices3 );
	glVertexAttribPointer( m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, uv3 );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 0 );

	// ---------------

	pyTexture4->bind( 0 );

	GLfloat vertices4[] = {
		-0.2, 1.0f, 0.2f,
		0, 1.5, 0,
		-0.2, 1.0f, -0.2f,
	};

	GLfloat uv4[] = {
		0, 0,
		1, 0,
		0.5, 0.5
	};

	glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices4 );
	glVertexAttribPointer( m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, uv4 );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 0 );

	// ---------------

	GLfloat vertices5[] = {
		-0.2, 1.0f, 0.2f,
		-0.2, 1.0f, -0.2f,
		0.2, 1.0, -0.2,

		-0.2, 1.0f, 0.2f,
		0.2, 1.0, -0.2,
		0.2, 1.0f, 0.2f,
	};

	GLfloat uv5[] = {
		0, 1,
		0,0,
		1, 0,

		0, 1,
		1, 0,
		1,1,
	};

	glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices5 );
	glVertexAttribPointer( m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, uv5 );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glDrawArrays( GL_TRIANGLES, 0, 6 );

	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 0 );

	// ---------------

	sphereTex->bind( 0 );
	sphereNM->bind( 1 );
	GLfloat sphereTri[9];
	GLfloat sphereNm[9];
	GLfloat sphereUv[6];

	m_program->setUniformValue( m_sphM, -42.0f );
	
	
	for( int j = 0; j != sphere.nbFaces; ++j ) {
		if( sphere.faces[j].size( ) == 3 ) {
			
			for( int k = 0; k != 3; ++k ) {
				sphereTri[k * 3] = sphere.vertex[sphere.faces[j][k] * 3] - 0.5f;
				sphereTri[k * 3 + 1] = sphere.vertex[sphere.faces[j][k] * 3 + 1]+1.0f;
				sphereTri[k * 3 + 2] = sphere.vertex[sphere.faces[j][k] * 3 + 2] - 0.5f;

				sphereNm[k * 3] = sphere.normal[sphere.faces[j][k] * 3];
				sphereNm[k * 3 + 1] = sphere.normal[sphere.faces[j][k] * 3 + 1];
				sphereNm[k * 3 + 2] = sphere.normal[sphere.faces[j][k] * 3 + 2];

				sphereUv[k * 2] = uvSphere[sphere.faces[j][k] * 2];
				sphereUv[k * 2 + 1] = uvSphere[sphere.faces[j][k] * 2 + 1];
			}

			glVertexAttribPointer( m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, sphereTri );
			glVertexAttribPointer( m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, sphereUv );
			glVertexAttribPointer( m_nmAttr, 3, GL_FLOAT, GL_FALSE, 0, sphereNm );

			glEnableVertexAttribArray( 0 );
			glEnableVertexAttribArray( 1 );
			glEnableVertexAttribArray( 2 );

			glDrawArrays( GL_TRIANGLES, 0, 3);

			glDisableVertexAttribArray( 2 );
			glDisableVertexAttribArray( 1 );
			glDisableVertexAttribArray( 0 );
		}
	}

    m_program->release();

    ++m_frame;
}
//! [5]
