//
//  Geometry.h
//  TemplateProject
//
//  Created by xuzebin on 10/14/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#define TINYOBJLOADER_IMPLEMENTATION

#include "geometrymaker.h"
#include "cvec.h"
#include "tiny_obj_loader.h"
#include "Vertex.h"


/**
 * An abstract class that genereates and saves vbo/ibo information.
 * we use this type for polymorphism in Entity class.
 */
class Geometry {
    
private:
    GLuint vertexVBO;
    GLuint indexVBO;
    
protected:
    int indicesNum;

    
public:
    
    Geometry& operator = (const Geometry& g) {
        vertexVBO = g.vertexVBO;
        indexVBO = g.indexVBO;
        indicesNum = g.indicesNum;
        return *this;
    }
    
    virtual void createVBOs() = 0;
    
    void createVBOs(std::vector<Vertex> vtx, std::vector<unsigned short> idx) {
        indicesNum = (int)idx.size();
        glGenBuffers(1, &vertexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
    }
    
    void draw(const GLuint aPositionLocation, const GLuint aNomralLocation, const GLuint aTexCoordLocation) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(aPositionLocation);
        
        glVertexAttribPointer(aNomralLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(aNomralLocation);
        
        glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(aTexCoordLocation);
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_SHORT, 0);
    }
};



#endif /* Geometry_h */
