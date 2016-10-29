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
    
    GLuint uModelViewMatrixLoc;
    GLuint uProjectionMatrixLoc;
    GLuint uNormalMatrixLoc;
    
    GLuint uColorLoc;
    GLuint uMinColorLoc;
    
    GLuint programId;
    
    ShaderProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
        programId = glCreateProgram();
        readAndCompileShader(programId, vertexShaderFileName, fragmentShaderFileName);
        
        aPositionLoc = glGetAttribLocation(programId, "aPosition");
        aNormalLoc = glGetAttribLocation(programId, "aNormal");
        aTexCoordLoc = glGetAttribLocation(programId, "aTexCoord");
        
        uModelViewMatrixLoc = glGetUniformLocation(programId, "uModelViewMatrix");
        uProjectionMatrixLoc = glGetUniformLocation(programId, "uProjectionMatrix");
        uNormalMatrixLoc = glGetUniformLocation(programId, "uNormalMatrix");
        uColorLoc = glGetUniformLocation(programId, "uColor");
        uMinColorLoc = glGetUniformLocation(programId, "uMinColor");
    }
    
    ~ShaderProgram() {
        glDeleteProgram(programId);
    }
    
};


#endif /* ShaderProgram_h */
