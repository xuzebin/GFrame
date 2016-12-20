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
    
public:

    Material() {}

    Material(Cvec3f color_) : color(color_) {}
    
    Material(const char* textureFileName) {
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
        return texture.hasDiffuseTexture();
    }
    
    bool hasSpecularTexture() {
        return texture.hasSpecularTexture();
    }
    
    bool hasNormalTexture() {
        return texture.hasNormalTexture();
    }
    
    int getDiffuseTexture() {
        return texture.getDiffuseTexture();
    }
    
    int getSpecularTexture() {
        return texture.getSpecularTexture();
    }
    
    int getNormalTexture() {
        return texture.getNormalTexture();
    }
    
    GLint getCubemapTexture() {
        return texture.getCubemapTexture();
    }
    
    void setDiffuseTexture(std::string diffuseTexName) {
        if (diffuseTexName != "") {
            texture.setDiffuseTexture(loadGLTexture(diffuseTexName.c_str()));
            std::cout << "diffuse texture set: " << diffuseTexName << std::endl;
        } else {
            std::cerr << "diffuse texture name empty" << std::endl;
        }
    }
    
    void setDiffuseTextureId(GLuint textureId) {
        texture.setDiffuseTexture(textureId);
    }
    
    void setSpecularTexture(std::string specularTexName) {
        if (specularTexName != "") {
            texture.setSpecularTexture(loadGLTexture(specularTexName.c_str()));
            std::cout << "specular texture set: " << specularTexName << std::endl;
        } else {
            std::cerr << "specular texture name empty" << std::endl;
        }
    }
    
    void setNormalTexture(std::string normalTexName) {
        if (normalTexName != "") {
            texture.setNormalTexture(loadGLTexture(normalTexName.c_str()));
            std::cout << "normal texture set: " << normalTexName << std::endl;
        } else {
            std::cerr << "normal texture name empty" << std::endl;
        }
    }
    
    void setCubemap(GLuint cubemapTexture) {
        texture.setCubemapTexture(cubemapTexture);
    }
    
    bool hasCubemap() {
        return texture.hasCubmapTexture();
    }

};

#endif /* Material_h */
