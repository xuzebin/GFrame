//
//  Texture.hpp
//  TemplateProject
//
//  Created by xuzebin on 11/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>


#include <glut.h>

class Texture {
public:
    GLuint diffuseTexture;
    GLuint specularTexture;
    GLuint normalTexture;
    
    Texture() {
        this->diffuseTexture = 0;
        this->specularTexture = 0;
        this->normalTexture = 0;
    }
    
    Texture& operator = (const Texture& t) {
        diffuseTexture = t.diffuseTexture;
        specularTexture = t.specularTexture;
        normalTexture = t.normalTexture;
        return *this;
    }


    
};

#endif /* Texture_hpp */
