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

static const Cvec3 UP_VEC = Cvec3(0, 1, 0);
static const double Z_NEAR = -0.1;
static const double Z_FAR = -1500;

/**
 * The camera object holds the view matrix as well as projection matrix.
 */
class Camera {
    
private:
    //could have used RigidBodyTransform,
    //but i don't need automatic view matrix calculation because i will use setlookat matrix here.
//    RigidBodyTransform transform;
    Cvec3 position;
    Quat rotation;
    
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
    
    Cvec3 offsetToObject;
    
    double fov = 45.0;
    
public:

    Camera() : position(0, 0, 0), offsetToObject(0, 10, 80) {
        updateView(Cvec3(0, 0, 0));
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

    void updateView(Cvec3 target) {
        position = target + offsetToObject;
        viewMatrix = setLookat(position, target, UP_VEC);
    }
    
//    void updateFov(double fov) {
//        this->fov = fov;
//        projectionMatrix = Matrix4::makeProjection(fov, 1.0, Z_NEAR, Z_FAR);
//    }
    
    
    int viewSwitch = 0;
    void switchMode(char mode) {
        viewSwitch = viewSwitch % 4;
        switch(mode) {
            case 'q':
            case 'Q':
                switch(viewSwitch) {
                    case 0:
                        offsetToObject = Cvec3(30, 40, 80);
                        break;
                    case 1:
                        offsetToObject = Cvec3(80, 80, 0);
                        break;
                    case 2:
                        offsetToObject = Cvec3(30, 40, -80);
                        break;
                    case 3:
                        offsetToObject = Cvec3(0, 10, 80);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        viewSwitch += 1;
    }
};


#endif /* Camera_h */
