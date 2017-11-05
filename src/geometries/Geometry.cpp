#include <iostream>
#include "Geometry.hpp"
#include "../base/geometrymaker.h"
#include "../base/glsupport.h"

//#define OPENGL4
#ifndef OPENGL4
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArray glDeleteVertexArrayAPPLE
#endif

GLuint vao = 0;

Geometry::Geometry() : indicesNum(0),
                       boundingBoxLength(0),
                       created(false)
{
}

Geometry::~Geometry()
{
}

Geometry& Geometry::operator = (const Geometry& g) {
    vertexVBO = g.vertexVBO;
    indexVBO = g.indexVBO;
    indicesNum = g.indicesNum;
    return *this;
}

void Geometry::createVBOs(std::vector<Vertex>& vtx, const std::vector<unsigned short>& idx, bool normalize) {
    if (created) {
        return;
    } else {
        created = true;
    }

    if (normalize) {
        this->normalizeVertices(vtx);
    }

    this->indicesNum = idx.size();
    this->verticesNum = vtx.size();

//     glGenVertexArrays(1, &vao);
//     checkGlError();
//     glBindVertexArray(vao);
//     checkGlError();

    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vtx.size(), vtx.data(), GL_STATIC_DRAW);

    checkGlError();
    
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);

    checkGlError();

    //    glBindVertexArray(0);
}

// void Geometry::createVBOs(std::vector<Vertex>& vtx, bool normalize) {
//     if (drawBbox) {
//         if (boundingBox == nullptr) {
//             boundingBox = std::make_shared<LineCube>(2);
//         }
//         boundingBox->createVBOs();
//     }
//     if (created) {
//         return;
//     } else {
//         created = true;
//     }

//     if (normalize) {
//         this->normalizeVertices(vtx);
//     }
//     this->verticesNum = vtx.size();

//     glGenBuffers(1, &vertexVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
// }

void Geometry::draw(const GLuint aPositionLocation, const GLuint aNomralLocation, const GLuint aTexCoordLocation, const GLuint aBinormalLocation, const GLuint aTangentLocation, GLenum mode) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    //    glBindVertexArray(vao);

    checkGlError();

    glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(aTexCoordLocation);

    checkGlError();
    
    glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(aPositionLocation);

    checkGlError();
    
    glVertexAttribPointer(aNomralLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(aNomralLocation);

    checkGlError();

    //#ifndef OPENGL4
    if (aBinormalLocation != -1) {
        glVertexAttribPointer(aBinormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, binormal));
        glEnableVertexAttribArray(aBinormalLocation);
        checkGlError();
    }
    //#endif

    //#ifndef OPENGL4
    if (aTangentLocation != -1) {
        glVertexAttribPointer(aTangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        glEnableVertexAttribArray(aTangentLocation);
        checkGlError();
    }
    //#endif

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glDrawElements(mode, indicesNum, GL_UNSIGNED_SHORT, 0);

    checkGlError();
    //    glBindVertexArray(0);

//     if (this->useIndex) {
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
//         glDrawElements(mode, indicesNum, GL_UNSIGNED_SHORT, 0);
// //         glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_SHORT, 0);
//     } else {
//         glDrawArrays(mode, 0, this->verticesNum);
//     }
//     if (drawBbox) {
//         boundingBox->draw(aPositionLocation, -1, -1, -1, -1, GL_LINE_LOOP);
//     }
}

BoundingBox Geometry::calcBoundingBox(const std::vector<Vertex>& vertices) {
    BoundingBox bbox;
    std::cout << "bbox min: " << bbox.min << std::endl;
    std::cout << "bbox max: " << bbox.max << std::endl;
    for (auto it = vertices.cbegin(); it != vertices.cend(); ++it) {
        for (int j = 0; j < 3; j++) {
            if (it->position[j] < bbox.min[j]) {
                bbox.min[j] = it->position[j];
            }
            if (it->position[j] > bbox.max[j]) {
                bbox.max[j] = it->position[j];
            }
        }
    }
    this->bbox = bbox;
    return bbox;
}

void Geometry::normalizeVertices(std::vector<Vertex>& vertices) {
    BoundingBox bbox = this->calcBoundingBox(vertices);
    std::cout << "bbox min: " << bbox.min << std::endl;
    std::cout << "bbox max: " << bbox.max << std::endl;

    /** Calculate the scale and translation to recenter and normalize the geometry **/
    float abs_max = 0;
    for (int i = 0; i < 3; i++) {
        if (abs_max < std::abs(bbox.max[i])) {
            abs_max = std::abs(bbox.max[i]);
        }
        if (abs_max < std::abs(bbox.min[i])) {
            abs_max = std::abs(bbox.min[i]);
        }
    }
    std::cout << "max_abso: " << abs_max << std::endl;

    //Scale and recenter the vertices
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].position -= (bbox.max + bbox.min) * 0.5;
        vertices[i].position /= abs_max;
    }
}
