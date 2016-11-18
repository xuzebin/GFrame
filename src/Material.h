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
#include "Texture.hpp"

#ifndef Material_h
#define Material_h

/**
 * Currently just wrap texture and color uniform.
 * More to add in the future.
 */
struct Material {
    Cvec3f color;//we here define a default (-1, -1, -1) which means do not apply color uniform
    std::vector<Texture> textures;
    
    Material() : Material(Cvec3f(-1, -1, -1)) {}

    Material(Cvec3f color_) : color(color_) {}
    
    Material(const char* textureFileName) : color(-1, -1, -1) {
//        diffuseTexture.push_back(loadGLTexture(textureFileName));
        
        Texture tex;
        setDiffuseTexture(tex, textureFileName);
        textures.push_back(tex);
    }
    
private:
    void setDiffuseTexture(Texture& tex, const char* textureFileName) {
        tex.diffuseTexture = loadGLTexture(textureFileName);
    }
    
//    
//    Material& operator = (const Material& g) {
//        color = g.color;
//        diffuseTexture = g.diffuseTexture;
//        specularTexture = g.specularTexture;
//        normalTexture = g.normalTexture;
//        return *this;
//    }
};

#endif /* Material_h */
