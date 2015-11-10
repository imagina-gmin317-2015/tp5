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

    void scale();

    //Getteurs
    int get_etat();
    float get_rotX();
    float get_rotY();
    float get_ss();

    //Setteurs
    void set_etat(int etat);
    void set_rotX(float rotX);
    void set_rotY(float rotY);
    void set_ss(float ss);

};

#endif // CAMERA_H
