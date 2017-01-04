//
//  Geometry.hpp
//  TemplateProject
//
//  Created by xuzebin on 11/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <GLUT/glut.h>
#include <stdio.h>
#include <vector>
#include "../base/Vertex.h"
#include "../base/cvec.h"

/**
 * An abstract class that genereates and saves vbo/ibo information.
 * we use this type for polymorphism in Entity class.
 */
class Geometry {
    
private:
    GLuint vertexVBO;
    GLuint indexVBO;
    
    bool created = false;
    
protected:
    unsigned short indicesNum;
    
    float minSphereDiameter;//currently all objects are considered sphere when testing intersection
    
public:
    virtual void createVBOs() = 0;
    virtual ~Geometry() {};
    
    Geometry& operator = (const Geometry& g);
    void createVBOs(std::vector<Vertex> vtx, std::vector<unsigned short> idx);
    virtual void draw(const GLuint aPositionLocation, const GLuint aNomralLocation, const GLuint aTexCoordLocation, const GLuint aBinormalLocation, const GLuint aTangentLocation);
    
    inline float getDiameter() {
        return minSphereDiameter;
    }
};




#endif /* Geometry_hpp */
