#include <iostream>
#include "Geometry.hpp"
#include "../base/geometrymaker.h"

Geometry& Geometry::operator = (const Geometry& g) {
    vertexVBO = g.vertexVBO;
    indexVBO = g.indexVBO;
    indicesNum = g.indicesNum;
    return *this;
}

void Geometry::createVBOs(std::vector<Vertex> vtx, std::vector<unsigned short> idx) {
    if (created) {
        return;
    } else {
        created = true;
    }
    indicesNum = idx.size();
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
}

void Geometry::draw(const GLuint aPositionLocation, const GLuint aNomralLocation, const GLuint aTexCoordLocation, const GLuint aBinormalLocation, const GLuint aTangentLocation) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    
    glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(aTexCoordLocation);
    
    glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(aPositionLocation);
    
    glVertexAttribPointer(aNomralLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(aNomralLocation);
    
    glVertexAttribPointer(aBinormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, binormal));
    glEnableVertexAttribArray(aBinormalLocation);
    
    glVertexAttribPointer(aTangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(aTangentLocation);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_SHORT, 0);
}
