//
//  Vertex.h
//  TemplateProject
//
//  Created by xuzebin on 11/10/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include "cvec.h"
#include "geometrymaker.h"

struct Vertex {
    Cvec3f position;
    Cvec3f normal;
    Cvec2f texCoord;
    
    Cvec3f binormal;
    Cvec3f tangent;
    
    Vertex() {}
    
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
