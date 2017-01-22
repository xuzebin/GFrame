#ifndef Sphere_h
#define Sphere_h

#include "Geometry.hpp"

class Sphere : public Geometry {
    
protected:
    float radius;
    float slices;
    float stacks;
    
    bool stereo;
public:
    Sphere(float radius, float slices, float stacks, bool stereo = false) {
        this->radius = radius;
        this->slices = slices;
        this->stacks = stacks;
        this->stereo = stereo;
        boundingBoxLength = 2 * radius;
    }
    
    void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getSphereVbIbLen(slices, stacks, vertexBufferLen, indexBufferLen);
        
        std::vector<Vertex> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makeSphere(radius, slices, stacks, vtx.begin(), idx.begin());
        
        //if stereo, divide v coordinate of texture by 2.
        if (stereo) {
            for(std::vector<Vertex>::iterator it = vtx.begin(); it != vtx.end(); ++it) {
                it->texCoord[1] = it->texCoord[1] / 2.0;
            }
        }
        
        Geometry::createVBOs(vtx, idx);
    }
    
    float getRadius() {
        return radius;
    }
    
};


#endif /* Sphere_h */
