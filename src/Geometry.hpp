//
//  Geometry.hpp
//  TemplateProject
//
//  Created by xuzebin on 11/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <glut.h>
#include <stdio.h>
#include <vector>
#include "Vertex.h"
#include "cvec.h"

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
    int indicesNum;
    
    
public:
    virtual void createVBOs() = 0;
    
    Geometry& operator = (const Geometry& g);
    void createVBOs(std::vector<Vertex> vtx, std::vector<unsigned short> idx);
    void draw(const GLuint aPositionLocation, const GLuint aNomralLocation, const GLuint aTexCoordLocation, const GLuint aBinormalLocation, const GLuint aTangentLocation);
};




#endif /* Geometry_hpp */
