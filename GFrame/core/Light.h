//
//  Light.h
//  TemplateProject
//
//  Created by xuzebin on 11/19/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include "../base/cvec.h"


class Light {
private:
    bool isPositionDirty;
    Cvec3 position;
    Cvec3 positionInEyeSpace;
    //TODO add attenuation coefficient in c code.


public:
    
    Cvec3f lightColor;
    Cvec3f specularLightColor;
    
    
    Light() : isPositionDirty(true), lightColor(Cvec3f(1, 1, 1)), specularLightColor(Cvec3f(1, 1, 1)) {}
    
    inline void setPosition(double x, double y, double z) {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        isPositionDirty = true;
    }
    inline void setPositionX(double x) {
        position[0] = x;
        isPositionDirty = true;
    }
    inline void setPositionY(double y) {
        position[1] = y;
        isPositionDirty = true;
    }
    inline void setPositionZ(double z) {
        position[2] = z;
        isPositionDirty = true;
    }
    inline void moveX(double x) {
        setPositionX(position[0] + x);
    }
    inline void moveY(double y) {
        setPositionY(position[1] + y);
    }
    inline void moveZ(double z) {
        setPositionZ(position[2] + z);
    }
    inline const Cvec3& getPosition() {
        return position;
    }
    
    inline void updatePositionInEyeSpace(const Matrix4& viewMatrix) {
        Cvec4 p = normalMatrix(viewMatrix) * Cvec4(position[0], position[1], position[2], 1);
        positionInEyeSpace[0] = p[0];
        positionInEyeSpace[1] = p[1];
        positionInEyeSpace[2] = p[2];
        isPositionDirty = false;
    }
    
    inline const Cvec3& getPositionInEyeSpace(const Matrix4& viewMatrix) {
        if (isPositionDirty) {
            updatePositionInEyeSpace(viewMatrix);
        }
        return positionInEyeSpace;
    }
    


};


#endif /* Light_h */
