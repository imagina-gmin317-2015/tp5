#include "texture.h"

Texture::Texture(){

}

Texture::Texture(QString n){
    nom = n;
    QImage buffTexture;
    if(!buffTexture.load(n)){
        qDebug()<<"Echec chargement texture :"<<n<<endl;
    }else{
        //qDebug()<<"Youhou, texture chargée"<<endl;
    }
    glEnable(GL_TEXTURE_2D);
    texture = QGLWidget::convertToGLFormat(buffTexture);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());


    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}


void Texture::initTexture(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);

}

void Texture::delTexture(){
    glDisable(GL_TEXTURE_2D);
}
