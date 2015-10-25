///////////////////////////////////////////////////////////////////////////////
// texture.cpp
// ----------------------------------------------------------------------------
// Classe permettant de gérer une texture OpenGL.
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <stdio.h>
#include "texture.h"
#include "tga.h"



///////////////////////////////////////////////////////////////////////////////
// Constructeur de la classe Texture.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
Texture::Texture()
{
    ident = 0;
}



///////////////////////////////////////////////////////////////////////////////
// Destructeur de la classe Texture.
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
Texture::~Texture()
{
    supprime();
}



///////////////////////////////////////////////////////////////////////////////
// Définit la texture comme étant la texture courante
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::utilise()
{
    glBindTexture(GL_TEXTURE_2D, ident);
}



///////////////////////////////////////////////////////////////////////////////
// Définit le mode de filtrage de la texture
//-----------------------------------------------------------------------------
// Paramètres :
//    mode_min  (in) : mode de filtrage pour la réduction de texture
//                    (GL_NEAREST ou GL_LINEAR ou
//                     GL_NEAREST_MIPMAP_NEAREST ou GL_LINEAR_MIPMAP_LINEAR).
//    mode_mag  (in) : mode de filtrage pour l'agrandissement de texture
//                    (GL_NEAREST ou GL_LINEAR ou
//                     GL_NEAREST_MIPMAP_NEAREST ou GL_LINEAR_MIPMAP_LINEAR).
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::definit_filtrage( GLint mode_min, GLint mode_mag )
{
    glBindTexture(GL_TEXTURE_2D, ident);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode_mag);
}



///////////////////////////////////////////////////////////////////////////////
// Définit le mode de bouclage de la texture
//-----------------------------------------------------------------------------
// Paramètres :
//    mode_s (in) : mode de bouclage selon l'axe s (GL_REPEAT ou GL_CLAMP).
//    mode_t (in) : mode de bouclage selon l'axe t (GL_REPEAT ou GL_CLAMP).
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::definit_bouclage( GLint mode_s, GLint mode_t )
{
    glBindTexture(GL_TEXTURE_2D, ident);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode_s );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode_t );
}



///////////////////////////////////////////////////////////////////////////////
// Contrôle la manière selon laquelle la texture est mélangée à la couleur de
// l'objet
//-----------------------------------------------------------------------------
// Paramètres :
//    mode  (in) : mode de mélange (GL_DECAL ou GL_MODULATE)
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::definit_melange( GLint mode )
{
    glBindTexture(GL_TEXTURE_2D, ident);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode );
}



///////////////////////////////////////////////////////////////////////////////
// Supprime la texture de la mémoire
//-----------------------------------------------------------------------------
// Paramètres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Texture::supprime()
{
    glDeleteTextures(1, &ident);
}



///////////////////////////////////////////////////////////////////////////////
// Chargement d'une image TGA, utilisée pour créer une texture OpenGL.
//-----------------------------------------------------------------------------
// Paramètres :
//    nom_fichier (in): nom du fichier TGA à charger.
//    mipmap      (in): indique si on veut une texture en mipmaping ou non.
//
// Retour :
//    true si la texture a été chargée, false sinon.
///////////////////////////////////////////////////////////////////////////////
bool Texture::charger( const char *nom_fichier)
{
    byte	*img;
    int		bpp;

    // Supprime la texture précédente
    if( glIsTexture(ident) )
        supprime();

    // Génère un identificateur de texture
    glGenTextures(1, &ident);

    // Chargement de l'image au format TGA
    img = load_tga( nom_fichier, &largeur, &hauteur, &bpp );

    if( img == NULL )
        return false;

    // On déclare cette texture comme la texture courante
    glBindTexture(GL_TEXTURE_2D, ident);

    // Chargement de l'image en mémoire vidéo
    if( bpp == 24 )
        glTexImage2D(GL_TEXTURE_2D, 0, 3, largeur, hauteur, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, 4, largeur, hauteur, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // On supprime le tableau dynamique alloué par load_tga(),
    // l'image ayant été recopiée en mémoire vidéo
    delete[] img;

    return true;
}
