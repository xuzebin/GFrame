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
#include "Cubemap.hpp"

#ifndef Material_h
#define Material_h

/**
 * Currently just wrap texture and color uniform.
 * More to add in the future.
 */
class Material {
    
private:
    Cvec3f color;
    Texture texture;
    Cubemap cubemap;
    
    bool hasCubemap;
    
public:
    Material() : hasCubemap(false){}

    Material(Cvec3f color_) : color(color_), hasCubemap(false) {}
    
    Material(const char* textureFileName) : hasCubemap(false) {
        setDiffuseTexture(textureFileName);
    }
    
    void setColor(Cvec3f color) {
        this->color = color;
    }
    void setColor(float r, float g, float b) {
        this->color[0] = r;
        this->color[1] = g;
        this->color[2] = b;
    }
    
    Cvec3f getColor() {
        return color;
    }
    
    bool hasDiffuseTexture() {
        return (texture.diffuseTexture >= 0);
    }
    
    bool hasSpecularTexture() {
        return (texture.specularTexture >= 0);
    }
    
    bool hasNormalTexture() {
        return (texture.normalTexture >= 0);
    }
    
    int getDiffuseTexture() {
        return texture.diffuseTexture;
    }
    
    int getSpecularTexture() {
        return texture.specularTexture;
    }
    
    int getNormalTexture() {
        return texture.normalTexture;
    }
    
    void setDiffuseTexture(std::string diffuseTexName) {
        if (diffuseTexName != "") {
            texture.diffuseTexture = loadGLTexture(diffuseTexName.c_str());
            std::cout << "diffuse texture set: " << diffuseTexName << std::endl;
        } else {
            std::cerr << "diffuse texture name empty" << std::endl;
        }
    }
    
    void setSpecularTexture(std::string specularTexName) {
        if (specularTexName != "") {
            texture.specularTexture = loadGLTexture(specularTexName.c_str());
            std::cout << "specular texture set: " << specularTexName << std::endl;
        } else {
            std::cerr << "specular texture name empty" << std::endl;
        }
    }
    
    void setNormalTexture(std::string normalTexName) {
        if (normalTexName != "") {
            texture.normalTexture = loadGLTexture(normalTexName.c_str());
            std::cout << "normal texture set: " << normalTexName << std::endl;
        } else {
            std::cerr << "normal texture name empty" << std::endl;
        }
    }
    
    void setCubemap(const Cubemap& cubemap) {
        this->cubemap = cubemap;
        hasCubemap = true;
    }
    
    bool cubemapApplied() {
        return hasCubemap;
    }

};

#endif /* Material_h */
