#ifndef Shader_h
#define Shader_h

class Entity;
class Light;
class Camera;

class Shader {
    
protected:
    GLuint programId;

public:
    
    Shader() {}

    virtual ~Shader() {
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
