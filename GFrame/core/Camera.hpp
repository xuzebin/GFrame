#ifndef Camera_hpp
#define Camera_hpp

#include "../base/quat.h"
#include "../base/cvec.h"

/**
 * The camera object holds the view matrix as well as projection matrix.
 */
class Camera {

public:
  static const Cvec3 UP_VEC;
  static const double Z_NEAR;
  static const double Z_FAR;

private:
    /** could have used RigidBodyTransform, 
     * but i don't need automatic * viewmatrix calculation because i will use setlookat matrix here.
     */
//    RigidBodyTransform transform;
    
    Cvec3 position;
    Quat rotation;
    
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
    
    Cvec3 offsetToObject;
    
    double fov = 45.0;
    
    void setViewMatrix(Cvec3 position, Quat rotation) {
        viewMatrix = Matrix4::makeTranslation(position) * quatToMatrix(rotation);
    }
    void updateViewMatrix() {
        setViewMatrix(position, rotation);
    }
    void setProjectionMatrix(const double fov, const double aspectRatio, const double zNear, const double zFar) {
        projectionMatrix = Matrix4::makeProjection(fov, aspectRatio, zNear, zFar);
    }


public:

    Camera() : position(0, 0, 0), offsetToObject(0, 10, 80) {
        updateView(Cvec3(0, 0, 0));
        setProjectionMatrix(fov, 1.0, Z_NEAR, Z_FAR);
    }
    
    Camera(Cvec3 position_, Quat rotation_) : position(position_), rotation(rotation_) {
        setViewMatrix(position, rotation);
        setProjectionMatrix(fov, 1.0, Z_NEAR, Z_FAR);
    }
    
    Matrix4 getViewMatrix() const {
        return viewMatrix;
    }
    Matrix4 getProjectionMatrix() const {
        return projectionMatrix;
    }
    Cvec3 getPosition() const {
        return position;
    }
    void updateView(Cvec3 target) {
        viewMatrix = setLookat(position, target, UP_VEC);
    }
    
    void switchMode(char mode);
    
    
    void rotate(Quat rotation) {
        this->rotation = this->rotation * rotation;
        updateViewMatrix();
    }
    void setPosition(Cvec3 position) {
        this->position = position;
        updateViewMatrix();
    }
    
    double getFov() const {
        return fov;
    }
};

const Cvec3 Camera::UP_VEC = Cvec3(0, 1, 0);
const double Camera::Z_NEAR = -0.1;
const double Camera::Z_FAR = -800;

#endif /* Camera_hpp */
