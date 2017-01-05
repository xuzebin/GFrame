#ifndef Cubemap_hpp
#define Cubemap_hpp

#include <stdio.h>
#include <GLUT/glut.h>
#include <vector>

class Cubemap {
private:
    GLuint texture;
public:

    Cubemap() : texture(0) {}
    
    void loadTextures(std::string posX, std::string negX,
               std::string posY, std::string negY,
               std::string posZ, std::string negZ);
    
    int getTexture() {
        return texture;
    }
};

#endif /* Cubemap_hpp */
