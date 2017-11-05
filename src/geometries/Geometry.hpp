#ifndef Geometry_hpp
#define Geometry_hpp

#ifdef __APPLE__
#include <GLUT/glut.h>
//#   include <OpenGL/gl3.h>
//#   define __gl_h_ /* Prevent inclusion of the old gl.h */
#endif 
#include <stdio.h>
#include <vector>
#include "../base/Vertex.h"
#include "../base/cvec.h"

struct BoundingBox {
    Cvec3f min;
    Cvec3f max;
};

/**
 * An abstract class that genereates and saves vbo/ibo information.
 * we use this type for polymorphism in Entity class.
 */
class Geometry {
public:
    virtual void createVBOs() = 0;
    virtual ~Geometry();
    
    Geometry& operator = (const Geometry& g);
    void createVBOs(std::vector<Vertex>& vtx, const std::vector<unsigned short>& idx, bool normalize = true);
//     void createVBOs(std::vector<Vertex>& vtx, bool normalize = true);
    virtual void draw(const GLuint aPositionLocation, const GLuint aNomralLocation, const GLuint aTexCoordLocation, const GLuint aBinormalLocation, const GLuint aTangentLocation, GLenum mode = GL_TRIANGLES);
    
    float getBoundingBoxLength() { return boundingBoxLength; }

    const BoundingBox& getBoundingBox() const { return bbox; }

    BoundingBox calcBoundingBox(const std::vector<Vertex>& vertices);

//     void drawBoundingBox(bool drawBbox) { this->drawBbox = drawBbox; }

protected:
    Geometry();
    void normalizeVertices(std::vector<Vertex>& vertices);

    unsigned short indicesNum;
    int verticesNum;

    float boundingBoxLength;//currently all objects are considered sphere when testing intersection

//     bool useIndex;

private:
    GLuint vertexVBO;
    GLuint indexVBO;
    
    bool created;

    BoundingBox bbox;

//     std::shared_ptr<Geometry> boundingBox;
//     bool drawBbox;
};


#endif /* Geometry_hpp */
