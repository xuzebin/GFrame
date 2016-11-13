//
//  ShaderProgram.h
//  TemplateProject
//
//  Created by xuzebin on 10/20/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef ShaderProgram_h
#define ShaderProgram_h

#include "glsupport.h"

/**
 * Includes locations in shaders.
 */
struct ShaderProgram {

    GLuint aPositionLoc;
    GLuint aNormalLoc;
    GLuint aTexCoordLoc;
    GLuint aBinormalLoc;
    GLuint aTangentLoc;
    
    GLuint uModelViewMatrixLoc;
    GLuint uProjectionMatrixLoc;
    GLuint uNormalMatrixLoc;
    
    GLuint uColorLoc;
    GLuint uMinColorLoc;
    
    //light
    GLuint uLightPositionLoc0;
    GLuint uLightColorLoc0;
    GLuint uSpecularLightColorLoc0;
    
    GLuint uLightPositionLoc1;
    GLuint uLightColorLoc1;
    GLuint uSpecularLightColorLoc1;
    
    
    GLuint uDiffuseTextureLoc;
    GLuint uSpecularTextureLoc;
    GLuint uNormalTextureLoc;
    
    GLuint programId;
    
    ShaderProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
        programId = glCreateProgram();
        readAndCompileShader(programId, vertexShaderFileName, fragmentShaderFileName);
        
        aPositionLoc = glGetAttribLocation(programId, "aPosition");
        aNormalLoc = glGetAttribLocation(programId, "aNormal");
        aTexCoordLoc = glGetAttribLocation(programId, "aTexCoord");
        aBinormalLoc = glGetAttribLocation(programId, "aBinormal");
        aTangentLoc = glGetAttribLocation(programId, "aTangent");
        
        uModelViewMatrixLoc = glGetUniformLocation(programId, "uModelViewMatrix");
        uProjectionMatrixLoc = glGetUniformLocation(programId, "uProjectionMatrix");
        uNormalMatrixLoc = glGetUniformLocation(programId, "uNormalMatrix");
        uColorLoc = glGetUniformLocation(programId, "uColor");
        uMinColorLoc = glGetUniformLocation(programId, "uMinColor");
        
        uLightPositionLoc0 = glGetUniformLocation(programId, "uLight[0].lightPosition");
        uLightColorLoc0 = glGetUniformLocation(programId, "uLight[0].lightColor");
        uSpecularLightColorLoc0 = glGetUniformLocation(programId, "uLight[0].specularLightColor");
        
        uLightPositionLoc1 = glGetUniformLocation(programId, "uLight[1].lightPosition");
        uLightColorLoc1 = glGetUniformLocation(programId, "uLight[1].lightColor");
        uSpecularLightColorLoc1 = glGetUniformLocation(programId, "uLight[1].specularLightColor");
        
        
        uDiffuseTextureLoc = glGetUniformLocation(programId, "uDiffuseTexture");
        uSpecularTextureLoc = glGetUniformLocation(programId, "uSpecularTexture");
        uNormalTextureLoc = glGetUniformLocation(programId, "uNormalTexture");
    }
    
    ~ShaderProgram() {
        glDeleteProgram(programId);
    }
    
};


#endif /* ShaderProgram_h */
