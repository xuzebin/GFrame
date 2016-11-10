//
//  Sphere.h
//  TemplateProject
//
//  Created by xuzebin on 11/10/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h

#include "Geometry.h"

class Sphere : public Geometry {
    
protected:
    float radius;
    float slices;
    float stacks;
    
public:
    
    Sphere(float radius, float slices, float stacks) {
        this->radius = radius;
        this->slices = slices;
        this->stacks = stacks;
    }
    
    void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getSphereVbIbLen(slices, stacks, vertexBufferLen, indexBufferLen);
        
        std::vector<Vertex> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makeSphere(radius, slices, stacks, vtx.begin(), idx.begin());
        
        Geometry::createVBOs(vtx, idx);
    }
    
    float getRadius() {
        return radius;
    }
    
};


#endif /* Sphere_h */
