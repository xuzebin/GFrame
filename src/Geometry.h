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


struct Vertex {
    Cvec3f position;
    Cvec3f normal;
    Cvec2f texCoord;
    
    Cvec3f binormal;
    Cvec3f tangent;
    
    
    
    Vertex() {}
    
    Vertex(Cvec3f positionCoordinate, Cvec3f normalCoordinate, Cvec2f textureCoordinate) :
    position(positionCoordinate), normal(normalCoordinate), texCoord(textureCoordinate) {}
    
    Vertex& operator = (const GenericVertex& v) {
        position = v.pos;
        normal = v.normal;
        texCoord = v.tex;
        binormal = v.binormal;
        tangent = v.tangent;
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
        
        std::vector<Vertex> vtx(vertexBufferLen);
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
        
        std::vector<Vertex> vtx(vertexBufferLen);
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
        
        std::vector<Vertex> vtx(vertexBufferLen);
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
        
        std::vector<Vertex> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makeCube(size, vtx.begin(), idx.begin());
        
        //flip normals of cube to point inside the box.
        for(std::vector<Vertex>::iterator it = vtx.begin(); it != vtx.end(); ++it) {
            it->normal *= -1;
        }
        
        Geometry::createVBOs(vtx, idx);
    }
};


class Model : public Geometry {
private:
    const std::string fileName;
public:
    Model(const std::string fileName_) : fileName(fileName_) {}
    
    void createVBOs() {
        std::vector<Vertex> vertices;
        std::vector<unsigned short> indices;
        loadFromFile(fileName, vertices, indices);
        
        Geometry::createVBOs(vertices, indices);
    }
    
private:
    
    void loadFromFile(const std::string& fileName, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
        
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
        
        if (ret) {
            for (int i = 0; i < attrib.vertices.size(); i += 3) {
                Vertex v;
                for (int j = 0; j < 3; ++j) {
                    v.position[j] = attrib.vertices[i + j];
                    v.normal[j]   = attrib.normals[i + j];
                }
                vertices.push_back(v);
            }
            for (int i = 0; i < shapes.size(); ++i) {
                for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
                    indices.push_back(shapes[i].mesh.indices[j].vertex_index);
                }
            }
        } else {
            std::cout << err << std::endl;
            assert(false);
        }
    }
};



#endif /* Geometry_h */
