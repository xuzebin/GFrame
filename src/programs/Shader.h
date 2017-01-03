//
//  Shader.h
//  TemplateProject
//
//  Created by xuzebin on 12/9/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

class Entity;
class Light;

class Shader {
    
protected:
    GLuint programId;

public:
    
    Shader() {}

    ~Shader() {
        glDeleteProgram(programId);
    }
    void use() {
        glUseProgram(programId);
    }
    int getProgramId() {
        return programId;
    }
    virtual void createProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
        programId = glCreateProgram();
        readAndCompileShader(programId, vertexShaderFileName, fragmentShaderFileName);
    }
    virtual void setLocationsAndDraw(Entity* entity, Camera* camera, Light* light0, Light* light1) = 0;
};

#endif /* Shader_h */
