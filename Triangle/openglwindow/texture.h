///////////////////////////////////////////////////////////////////////////////
// texture.h
// ----------------------------------------------------------------------------
// Classe permettant de gérer une texture OpenGL.
///////////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_H
#define TEXTURE_H


#include <string>
#include <QtGui/QOpenGLShaderProgram>


class Texture
{
public :
    // Constructeur
    Texture();

    // Destructeur
    ~Texture();

    // Charge une image de type TGA, et en fait une texture OpenGL
    bool charger( const char *nom_fichier);

    // Définit la texture comme étant la texture courante
    void utilise();

    // Définit le mode de filtrage de la texture
    void definit_filtrage( GLint mode_min, GLint mode_mag );

    // Définit le mode de bouclage de la texture
    void definit_bouclage( GLint mode_s, GLint mode_t );

    // Contrôle la manière selon laquelle la texture est mélangée à la couleur de l'objet
    void definit_melange( GLint mode );

    // Supprime la texture de la mémoire
    void supprime();

private:
    // Identificateur de texture donné par la fonction glGenTextures()
    GLuint	ident;

    // Largeur et hauteur de la texture, données par les dimensions de l'image TGA
    int		largeur, hauteur;
};



#endif
