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

    GLint aPositionLoc;
    GLint aNormalLoc;
    GLint aTexCoordLoc;
    GLint aBinormalLoc;
    GLint aTangentLoc;
  
    GLint uModelMatrixLoc;
    GLint uModelViewMatrixLoc;
    GLint uProjectionMatrixLoc;
    GLint uNormalMatrixLoc;
    
    GLint uColorLoc;
    GLint uMinColorLoc;
    
    //light
    GLint uLightPositionLoc0;
    GLint uLightColorLoc0;
    GLint uSpecularLightColorLoc0;
    
    GLint uLightPositionLoc1;
    GLint uLightColorLoc1;
    GLint uSpecularLightColorLoc1;
    
    
    GLint uDiffuseTextureLoc;
    GLint uSpecularTextureLoc;
    GLint uNormalTextureLoc;
    
    GLint uEnvironmentMapLoc;
    
    
    GLuint programId;
    
    
    ShaderProgram() {
    }
    
    void createProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
        programId = glCreateProgram();
        readAndCompileShader(programId, vertexShaderFileName, fragmentShaderFileName);
        glUseProgram(programId);
        
        aPositionLoc = glGetAttribLocation(programId, "aPosition");
        aNormalLoc = glGetAttribLocation(programId, "aNormal");
        aTexCoordLoc = glGetAttribLocation(programId, "aTexCoord");
        aBinormalLoc = glGetAttribLocation(programId, "aBinormal");
        aTangentLoc = glGetAttribLocation(programId, "aTangent");
        
        uModelMatrixLoc = glGetUniformLocation(programId, "uModelMatrix");
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
        
        
        printLocation("aPositionLoc", aPositionLoc);
        printLocation("aNormalLoc", aNormalLoc);
        printLocation("aTexCoordLoc", aTexCoordLoc);
        printLocation("aBinormalLoc", aBinormalLoc);
        printLocation("aTangentLoc", aTangentLoc);
        printLocation("uModelMatrixLoc", uModelMatrixLoc);
        printLocation("uLightPositionLoc0", uLightPositionLoc0);
        printLocation("uDiffuseTextureLoc", uDiffuseTextureLoc);
        printLocation("uSpecularTextureLoc", uSpecularTextureLoc);
        printLocation("uNormalTextureLoc", uNormalTextureLoc);
        

//                uEnvironmentMapLoc = glGetUniformLocation(programId, "uEnviromentMap");
    }
    
    void setAttributes() {
        
    }
    
    void setUniforms() {
        
    }
    
    
    
    void printLocation(std::string name, int location) {
        std::cout << name << ": " << location << std::endl;
    }
    
    ~ShaderProgram() {
        glDeleteProgram(programId);
    }
    
};

struct Shader {
    
    const std::string A_POSITION = "aPosition";
    const std::string A_TEXCOORD = "aTexCoord";
    const std::string U_MODEL_MATRIX = "uModelMatrix";
    const std::string U_MODELVIEW_MATRIX = "uModelViewMatrix";
    const std::string U_PROJECTION_MATRIX = "uProjectionMatrix";
    const std::string U_COLOR = "uColor";
    const std::string U_DIFFUSE_TEXTURE = "uDiffuseTexture";
    const std::string U_SPECULAR_TEXTURE = "uSpecularTexture";
    const std::string U_NORMAL_TEXTURE = "uNormalTexture";

//    
//    
    GLuint programId;
    //    std::unordered_map<std::string, GLuint> uniforms;
    //    std::unordered_map<std::string, GLuint> attributes;
//
    
    virtual void createProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) = 0;
    virtual void setAttributes() = 0;
    virtual void setUniforms() = 0;
    virtual ~Shader() {}
};
//





#endif /* ShaderProgram_h */
