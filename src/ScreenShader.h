//
//  ScreenShader.h
//  TemplateProject
//
//  Created by xuzebin on 12/11/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef ScreenShader_h
#define ScreenShader_h

class Shader;

class ScreenShader : public Shader {

protected:
    //attributes
    GLint aPositionLoc;
    GLint aTexCoordLoc;

    //texture uniforms
    GLint uScreenFrameBufferLoc;

    //screen resolution for fxaa shader
    GLint uResolutionLoc;

    //exposure for hdr tone shader
    GLint uExposureLoc;

    void getLocations(int programId) {
        aPositionLoc = glGetAttribLocation(programId, "aPosition");
        aTexCoordLoc = glGetAttribLocation(programId, "aTexCoord");

        uScreenFrameBufferLoc = glGetUniformLocation(programId, "uScreenFrameBuffer");
        uResolutionLoc = glGetUniformLocation(programId, "uResolution");//used for fxaa shader
        uExposureLoc = glGetUniformLocation(programId, "uExposure");//for hdr tone shader
    }

public:

    void createProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
        Shader::createProgram(vertexShaderFileName, fragmentShaderFileName);
        getLocations(programId);
    }

    void setLocationsAndDraw(Entity* entity, Camera* camera, Light* light0, Light* light1) {
        glUseProgram(programId);

        if (entity->material->hasDiffuseTexture()) {//we store the screen frame buffer texture in diffuse texture.
            glUniform1i(uScreenFrameBufferLoc, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, entity->material->getDiffuseTexture());
        }

        //fxaa shader
        glUniform2f(uResolutionLoc, 1024.0f, 1024.0f);

        //hdr tone shader
        glUniform1f(uExposureLoc, 3.0);

        entity->geometry->draw(aPositionLoc, -1, aTexCoordLoc, -1, -1);
    }

};

#endif /* ScreenShader_h */
