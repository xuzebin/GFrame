//
//  Geometry.h
//  TemplateProject
//
//  Created by xuzebin on 10/14/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#include "geometrymaker.h"
#include "cvec.h"


struct VertexPN {
    Cvec3f position;
    Cvec3f normal;
    Cvec2f texCoord;
    
    VertexPN() {}
    
    VertexPN(Cvec3f positionCoordinate, Cvec3f normalCoordinate, Cvec2f textureCoordinate) :
    position(positionCoordinate), normal(normalCoordinate), texCoord(textureCoordinate) {}
    
    VertexPN& operator = (const GenericVertex& v) {
        position = v.pos;
        normal = v.normal;
        texCoord = v.tex;
        return *this;
    }
};


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
    
    void createVBOs(std::vector<VertexPN> vtx, std::vector<unsigned short> idx) {
        
        glGenBuffers(1, &vertexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
    }
    
    void draw(const GLuint aPositionLocation, const GLuint aNomralLocation, const GLuint aTexCoordLocation) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, position));
        glEnableVertexAttribArray(aPositionLocation);
        
        glVertexAttribPointer(aNomralLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, normal));
        glEnableVertexAttribArray(aNomralLocation);
        
        glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, texCoord));
        glEnableVertexAttribArray(aTexCoordLocation);
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_SHORT, 0);
    }
};

/**
 * Geometry subclass that is a cube.
 */
class Cube : public Geometry {
    
protected:
    float size;
    
public:
    
    Cube(float size) {
        this->size = size;
    }
    
    void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getCubeVbIbLen(vertexBufferLen, indexBufferLen);
        
        indicesNum = indexBufferLen;
        
        std::vector<VertexPN> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makeCube(size, vtx.begin(), idx.begin());
        
        Geometry::createVBOs(vtx, idx);
    }
};

/**
 * Geometry subclass that is a plane.
 */
class Plane : public Geometry {
    
protected:
    float size;
    
public:
    Plane(float size) {
        this->size = size;
    }

    void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getPlaneVbIbLen(vertexBufferLen, indexBufferLen);
        
        indicesNum = indexBufferLen;
        
        std::vector<VertexPN> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makePlane(size, vtx.begin(), idx.begin());
        
        Geometry::createVBOs(vtx, idx);
    }
};

/**
 * Geometry subclass that is a sphere.
 */
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
        
        
        indicesNum = indexBufferLen;
        
        std::vector<VertexPN> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        
        makeSphere(radius, slices, stacks, vtx.begin(), idx.begin());
        
        Geometry::createVBOs(vtx, idx);
    }
    
    float getRadius() {
        return radius;
    }
    
};

//The difference between SkyBox and Cube here is that SkyBox flips normals to point inside the box.
class SkyBox : public Cube {
    
public:
    SkyBox(int size) : Cube(size) {}
    
    void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getCubeVbIbLen(vertexBufferLen, indexBufferLen);
        
        indicesNum = indexBufferLen;
        
        std::vector<VertexPN> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makeCube(size, vtx.begin(), idx.begin());
        
        //flip normals of cube to point inside the box.
        for(std::vector<VertexPN>::iterator it = vtx.begin(); it != vtx.end(); ++it) {
            it->normal *= -1;
        }
        
        Geometry::createVBOs(vtx, idx);
    }
};



#endif /* Geometry_h */
