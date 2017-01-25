#ifndef ShadowShader_h
#define ShadowShader_h
class Shader;

class ShadowShader : public Shader {
public:

    void createProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
        Shader::createProgram(vertexShaderFileName, fragmentShaderFileName);
        getLocations(programId);
    }

    void setLocationsAndDraw(Entity* entity, std::shared_ptr<Camera> camera, std::shared_ptr<Light> light0, std::shared_ptr<Light> light1) {
        glUseProgram(programId);

        Matrix4 projectionMatrix = camera->getProjectionMatrix();
        
        //Transform hierachy, iteratively multiply parent rigidbody matrices
        //to get the ultimate modelmatrix that transform from object frame to world frame.
        Matrix4 modelMatrix;
        modelMatrix = entity->transform.getModelMatrix();

        Entity* current = entity->parent;
        while (current != NULL) {
            modelMatrix = current->transform.getRigidBodyMatrix() * modelMatrix;
            current = current->parent;
        }

        const Matrix4 viewMatrix = camera->getViewMatrix();
        Matrix4 modelViewMatrix = inv(viewMatrix) * modelMatrix;
        Matrix4 normal = normalMatrix(modelViewMatrix);

        modelMatrix.writeToColumnMajorMatrix(modelMat);//TODO add switch
        modelViewMatrix.writeToColumnMajorMatrix(modelViewMat);
        projectionMatrix.writeToColumnMajorMatrix(projectionMat);
        normal.writeToColumnMajorMatrix(normalMat);

        glUniformMatrix4fv(uModelMatrixLoc, 1, false, modelMat);
        glUniformMatrix4fv(uModelViewMatrixLoc, 1, false, modelViewMat);
        glUniformMatrix4fv(uProjectionMatrixLoc, 1, false, projectionMat);
        glUniformMatrix4fv(uNormalMatrixLoc, 1, false, normalMat);

        Cvec3f color = entity->material->getColor();
        glUniform3f(uColorLoc, color[0], color[1], color[2]);

        if (light0 != nullptr) {
            Cvec3 lightPosEye0 = light0->getPositionInEyeSpace(viewMatrix);
            glUniform3f(uLightPositionLoc, lightPosEye0[0], lightPosEye0[1], lightPosEye0[2]);
            Cvec3f lightColor = light0->lightColor;
            glUniform3f(uLightColorLoc, lightColor[0], lightColor[1], lightColor[2]);

            Cvec3f specularLightColor;
            if (entity->isLightOn(0)) {
                specularLightColor = light0->specularLightColor;
            } else {
                specularLightColor = Cvec3f(0, 0, 0);
            }

            glUniform3f(uSpecularLightColorLoc, specularLightColor[0], specularLightColor[1], specularLightColor[2]);
        } else {
            glUniform3f(uLightPositionLoc, 0, 0, 0);
            glUniform3f(uLightColorLoc, 1.0f, 1.0f, 1.0f);
            glUniform3f(uSpecularLightColorLoc, 0, 0, 0);
        }

        if (entity->material->hasDiffuseTexture()) {
            glUniform1i(uShadowTextureLoc, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, entity->material->getDiffuseTexture());
        }

        glUniform3f(uSpotDirectionLoc, 1.0f, -1.0f, -1.0f);
        glUniform1f(uSpotExponentLoc, 2.0);
        glUniform1f(uSpotCosCutoffLoc, 0.5);

        entity->geometry->draw(aPositionLoc, aNormalLoc, aTexCoordLoc, -1, -1);
    }
protected:
    void getLocations(int programId) {
        aPositionLoc = glGetAttribLocation(programId, "aPosition");
        aNormalLoc = glGetAttribLocation(programId, "aNormal");
        aTexCoordLoc = glGetAttribLocation(programId, "aTexCoord");

        uModelMatrixLoc = glGetUniformLocation(programId, "uModelMatrix");
        uModelViewMatrixLoc = glGetUniformLocation(programId, "uModelViewMatrix");
        uProjectionMatrixLoc = glGetUniformLocation(programId, "uProjectionMatrix");
        uNormalMatrixLoc = glGetUniformLocation(programId, "uNormalMatrix");
        uColorLoc = glGetUniformLocation(programId, "uColor");

        uLightPositionLoc = glGetUniformLocation(programId, "uLightPosition");
        uLightColorLoc = glGetUniformLocation(programId, "uLightColor");
        uSpecularLightColorLoc = glGetUniformLocation(programId, "uSpecularLightColor");

        uShadowTextureLoc = glGetUniformLocation(programId, "uShadowTexture");

        uSpotDirectionLoc = glGetUniformLocation(programId, "uSpotDirection");
        uSpotExponentLoc = glGetUniformLocation(programId, "uSpotExponent");
        uSpotCosCutoffLoc = glGetUniformLocation(programId, "uSpotCosCutoff");
    }

    //attributes
    GLint aPositionLoc;
    GLint aNormalLoc;
    GLint aTexCoordLoc;

    //matrix uniforms
    GLint uModelMatrixLoc;
    GLint uModelViewMatrixLoc;
    GLint uProjectionMatrixLoc;
    GLint uNormalMatrixLoc;

    //light uniforms
    GLint uLightPositionLoc;
    GLint uLightColorLoc;
    GLint uSpecularLightColorLoc;

    //texture uniforms
    GLint uShadowTextureLoc;

    //color uniform
    GLint uColorLoc;


    //spot light uniforms
    GLint uSpotDirectionLoc;
    GLint uSpotExponentLoc;
    GLint uSpotCosCutoffLoc;

    GLfloat modelMat[16];
    GLfloat modelViewMat[16];
    GLfloat projectionMat[16];
    GLfloat normalMat[16];

};

#endif /* ShadowShader_h */
