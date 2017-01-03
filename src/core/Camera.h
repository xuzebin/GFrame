//
//  Camera.h
//  TemplateProject
//
//  Created by xuzebin on 10/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include "quat.h"



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
    
    void setViewMatrix(Cvec3 position, Quat rotation);
    void updateViewMatrix();
    void setProjectionMatrix(const double fov, const double aspectRatio, const double zNear, const double zFar);


public:

    Camera() : position(0, 0, 0), offsetToObject(0, 10, 80) {
        updateView(Cvec3(0, 0, 0));
        setProjectionMatrix(fov, 1.0, Z_NEAR, Z_FAR);
    }
    
    Camera(Cvec3 position_, Quat rotation_) : position(position_), rotation(rotation_) {
        setViewMatrix(position, rotation);
        setProjectionMatrix(fov, 1.0, Z_NEAR, Z_FAR);
    }
    
    Matrix4 getViewMatrix() const;
    Matrix4 getProjectionMatrix() const;
    Cvec3 getPosition() const;
    void updateView(Cvec3 target);
    
    void switchMode(char mode);
    
    
    void rotate(Quat rotation) {
        this->rotation = this->rotation * rotation;
        updateViewMatrix();
    }
    void setPosition(Cvec3 position) {
        this->position = position;
        updateViewMatrix();
    }
    
    double getFov();
};


#endif /* Camera_h */
