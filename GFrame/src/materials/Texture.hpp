#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>


#include <GLUT/glut.h>

class Texture {
    
private:
    bool hasDiffuseTex;
    bool hasSpecularTex;
    bool hasNormalTex;
    
    GLuint diffuseTexture;
    GLuint specularTexture;
    GLuint normalTexture;
    
    bool hasCubemapTex;
    GLuint cubemapTexture;
    
    
public:
    
    Texture() : diffuseTexture(0), specularTexture(0), normalTexture(0), cubemapTexture(0),
                hasDiffuseTex(false), hasSpecularTex(false), hasNormalTex(false), hasCubemapTex(false)
    {}
    
    Texture& operator = (const Texture& t) {
        diffuseTexture = t.diffuseTexture;
        specularTexture = t.specularTexture;
        normalTexture = t.normalTexture;
        return *this;
    }

    bool hasDiffuseTexture() {
        return hasDiffuseTex;
    }
    bool hasSpecularTexture() {
        return hasSpecularTex;
    }
    bool hasNormalTexture() {
        return hasNormalTex;
    }
    bool hasCubmapTexture() {
        return hasCubemapTex;
    }
    void setDiffuseTexture(GLuint diffuse) {
        diffuseTexture = diffuse;
        hasDiffuseTex = true;
    }
    void setSpecularTexture(GLuint specular) {
        specularTexture = specular;
        hasSpecularTex = true;
    }
    void setNormalTexture(GLuint normal) {
        normalTexture = normal;
        hasNormalTex = true;
    }
    void setCubemapTexture(GLuint cubemap) {
        cubemapTexture = cubemap;
        hasCubemapTex = true;
    }
    GLuint getDiffuseTexture() {
        return diffuseTexture;
    }
    GLuint getSpecularTexture() {
        return specularTexture;
    }
    GLuint getNormalTexture() {
        return normalTexture;
    }
    GLuint getCubemapTexture() {
        return cubemapTexture;
    }
};

#endif /* Texture_hpp */
