//
//  Raycaster.h
//  TemplateProject
//
//  Created by xuzebin on 10/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Raycaster_h
#define Raycaster_h

#include "cvec.h"
#include "Camera.h"
#include "matrix4.h"

/**
 * Currently only support ray sphere intersection.
 * but has some bugs when camera moves.
 * Will be fixed next time.
 */
class Raycaster {
private:
    Raycaster() {}
public:
    
    static bool isPicked(int x, int y, const int screenWidth, const int screenHeight, Matrix4 projectionMatrix, Matrix4 viewMatrix, Cvec3 cameraPosition, Cvec3 center, double radius) {
        Cvec3 rayDirection = castRay(x, y, screenWidth, screenHeight, projectionMatrix, viewMatrix);
        std::cout << "raydirection: " << rayDirection[0] << "," << rayDirection[1] << "," << rayDirection[2] << std::endl;
        return raySphereIntersect(center, radius, cameraPosition, rayDirection);
    }
    
    //get ray direction
    static Cvec3 castRay(int x, int y, const int windowWidth, const int windowHeight, Matrix4 projectionMatrix, Matrix4 viewMatrix) {
        //normalize
        float nx = (float)x / ((float)windowWidth / 2.0f) - 1.0f;
        float ny = 1.0f - (float)y / ((float)windowHeight / 2.0f);
        std::cout << "nx: " << nx << "ny: " << ny << std::endl;
        
        //convert to homogeneous clip space
        Cvec4 rayClip = Cvec4(nx, ny, -1.0, 1.0);//point toward -z axis.
        
        //convert to eye space
//        Matrix4 proj = camera.getProjectionMatrix();
        Matrix4 proj = projectionMatrix;
        proj[12] = 0.0; proj[13] = 0.0; proj[14] = 0.0; proj[15] = 1.0;
        Matrix4 projectionInverse = inv(proj);
        Cvec4 rayEye = projectionInverse * rayClip;
        rayEye[2] = -1.0;//forward
        rayEye[3] = 0.0;//vector
        
        //convert to world space
//        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 viewInverse = inv(viewMatrix);
        Cvec4 rayWorld = viewInverse * rayEye;
        
        return Cvec3(rayWorld[0], rayWorld[1], rayWorld[2]).normalize();//rayDirection
    }

    //ray sphere intersection
    static bool raySphereIntersect(Cvec3 center, double radius, Cvec3 cameraPosition, Cvec3 rayDirection) {
        Cvec3 nearPos = cameraPosition;
        nearPos += Cvec3(0, 0, Camera::Z_NEAR);
        Cvec3 toCenterDirection = center - nearPos;
        double t = dot(toCenterDirection, rayDirection);
        
        Cvec3 intersect = nearPos + rayDirection * t;
        //calculate distance
        double distance = norm(intersect - center);
//        intersect[2] = 0.0;
//        double distance = norm(intersect);
        
//        std::cout << "distance: " << distance << ", radius: " << radius;
//        printf("center: (%f, %f, %f), intersect: (%f, %f, %f)\n", center[0], center[1], center[2], intersect[0], intersect[1], intersect[2]);
        
        
        return distance <= radius;
    }

    
};



#endif /* Raycaster_h */
