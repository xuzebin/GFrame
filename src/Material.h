//
//  Material.h
//  TemplateProject
//
//  Created by xuzebin on 10/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "cvec.h"
#include "stb_image.h"
#include "glsupport.h"

#ifndef Material_h
#define Material_h

/**
 * Currently just wrap texture and color uniform.
 * More to add in the future.
 */
struct Material {
    Cvec3f color;//we here define a default (-1, -1, -1) which means do not apply color uniform
    GLuint diffuseTexture;
    GLuint specularTexture;
    GLuint normalTexture;
    
    Material() : color (-1, -1, -1), diffuseTexture(0), specularTexture(0), normalTexture(0){}
    
    Material(const char* textureFileName) : color(-1, -1, -1) {
        diffuseTexture = loadGLTexture(textureFileName);
    }
    
    Material(Cvec3f color_) : color(color_), diffuseTexture(0), specularTexture(0), normalTexture(0) {}
    
    Material& operator = (const Material& g) {
        color = g.color;
        diffuseTexture = g.diffuseTexture;
        specularTexture = g.specularTexture;
        normalTexture = g.normalTexture;
        return *this;
    }
};

#endif /* Material_h */
