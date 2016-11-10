//
//  Vertex.h
//  TemplateProject
//
//  Created by xuzebin on 11/10/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

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

#endif /* Vertex_h */
