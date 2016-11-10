//
//  Camera.cpp
//  TemplateProject
//
//  Created by xuzebin on 11/10/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include <stdio.h>
#include "Camera.h"

const Cvec3 Camera::UP_VEC = Cvec3(0, 1, 0);
const double Camera::Z_NEAR = -0.1;
const double Camera::Z_FAR = -800;


void Camera::setViewMatrix(const Cvec3 position, const Quat rotation) {
    viewMatrix = Matrix4::makeTranslation(position) * quatToMatrix(rotation);
}

void Camera::setProjectionMatrix(const double fov, const double aspectRatio, const double zNear, const double zFar) {
    projectionMatrix = Matrix4::makeProjection(fov, aspectRatio, zNear, zFar);
}

Matrix4 Camera::getViewMatrix() const {
    return viewMatrix;
}

Matrix4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

Cvec3 Camera::getPosition() const {
    return position;
}

void Camera::updateView(Cvec3 target) {
    position = target + offsetToObject;
    viewMatrix = setLookat(position, target, UP_VEC);
}



int viewSwitch = 0;
void Camera::switchMode(char mode) {
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
