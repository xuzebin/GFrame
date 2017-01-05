#ifndef FpsCamera_h
#define FpsCamera_h

#include "matrix4.h"
#include "cvec.h"

//static const double Z_NEAR = -0.1;
//static const double Z_FAR = -800;
//static const double FOV = 45;

/** 
 * a first person camera
 * currently we here restrict rotation direction to only yaw and pitch
 */
struct FpsCamera {
    
private:

    Quat rotation;
    
    double yaw;
    double pitch;
    
    Cvec3 position;
    
    const double MovingSpeed = 8.0;
    
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
    
    double fov = 45.0;
    
public:
    FpsCamera() : yaw(0.0), pitch(0.0), position(30.0, 10.0, 30.0) {
        updateView();
        projectionMatrix = Matrix4::makeProjection(fov, 1.0, Z_NEAR, Z_FAR);
    }
    
    Matrix4 getViewMatrix() {
        return viewMatrix;
    }
    
    Matrix4 getProjectionMatrix() {
        return projectionMatrix;
    }
    Cvec3 getPosition() {
        return position;
    }
    void translate(Cvec3 translation) {
        position += translation;
    }
    void updateView() {
        viewMatrix = Matrix4::makeTranslation(position) * Matrix4::makeXRotation(pitch) * Matrix4::makeYRotation(yaw);
    }
    
    void control(const unsigned char key) {
        float dx = 0;
        float dz = 0;
        
        switch (key) {
            case 'd':
            case 'D':
                dx = 2;
                break;
            case 's':
            case 'S':
                dz = -2;
                break;
            case 'a':
            case 'A':
                dx = -2;
                break;
            case 'w':
            case 'W':
                dz = 2;
                break;
            case 'f':
            case 'F':
                dz = 20;
            default:
                break;
        }
        
        
        Cvec3 forward = Cvec3(viewMatrix(0, 2), viewMatrix(1, 2), viewMatrix(2, 2));
        Cvec3 strafe = Cvec3(viewMatrix(0, 0), viewMatrix(1, 0), viewMatrix(2, 0));
//
        position += (forward * (-dz) + strafe * dx) * MovingSpeed;
//        position += (forward * (-dz)) * MovingSpeed;
        this->pitch += dz;
        this->yaw -= dx;

        updateView();
    }
    
private:
    int lastX;
    int lastY;
    bool pressed = false;
    const float ROTATE_SPEED = 0.2f;
public:
    void mouseMotion(int x, int y) {
//        if (!pressed) return;
        int dx = x - lastX;
        int dy = y - lastY;
        
        this->yaw += dx * ROTATE_SPEED;
        this->pitch += dy * ROTATE_SPEED;
        
        lastX = x;
        lastY = y;
        updateView();
    }
    void mousePressed(int state, int x, int y) {
        switch(state) {
            case GLUT_DOWN:
                lastX = x;
                lastY = y;
                pressed = true;
                break;
            case GLUT_UP:
                pressed = false;
                break;
            default:
                break;
        }
    }
    
};

#endif /* FpsCamera_h */
