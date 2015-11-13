#ifndef CAMERA_H
#define CAMERA_H


class Camera
{
private:
    int m_etat;
    float m_rotX;
    float m_rotY;
    float m_ss;

public:
    Camera();

    float getSS();
    float getRotY();
    float getRotX();
    int getEtat();

    void setSS(float ss);
    void setRotX(float rotX);
    void setRotY(float rotY);
    void setEtat(int etat);

    void scale();

};

#endif // CAMERA_H
