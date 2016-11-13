//
//  Entity.h
//  TemplateProject
//
//  Created by xuzebin on 10/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include "Transform.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Geometry.h"


/**
 * The object/Entity to be rendered.
 */
class Entity {
    
public:
    Transform transform;
    Geometry* geometry;
    Material* material;
protected:
    
    static int id_counter;

    std::string name;
    
    bool visible;
    bool depthTest;
    
    GLfloat modelViewMat[16];
    GLfloat projectionMat[16];
    GLfloat normalMat[16];
    
public:
    Entity* parent;
    
    Entity() : Entity(NULL, NULL) {}
    
    Entity(Geometry* geometry_, Material* material_)
        : Entity("", geometry_, material_, NULL) {}
    
    Entity(std::string name_, Geometry* geometry_, Material* material_)
        : Entity(name_, geometry_, material_, NULL) {}
 
    Entity(Geometry* geometry_, Material* material_, Entity* parent_)
        : Entity("", geometry_, material_, parent_) {}

    Entity(std::string name_, Geometry* geometry_, Material* material_, Entity* parent_)
        :name(name_), geometry(geometry_), material(material_), parent(parent_) {
        
        ++id_counter;
        visible = true;
        depthTest = true;
        
        if (name.empty()) {
            name = "Entity" + std::to_string(id_counter);
        }
    }
    
    ~Entity() {
        delete material;
    }
    
//    void createMesh();
//    void draw(Camera* camera, ShaderProgram* shaderProgram);
//    string getName();
//    void setVisible(bool visible);
//    bool isVisible();
//    void setDepthTest(bool enable);
//    bool depthTestEnabled();
//    void setPosition(Cvec3 position);
//    void translate(Cvec3 translation);
//    const Cvec3& getPosition();
//    void setRotation(Quat rotation);
//    void rotate(Quat rotation);
//    const Quat& getRotation();
//    void setScale(Cvec3 scale);
//    const Cvec3& getScale();
    
    void createMesh()  {
        if (geometry == NULL) {
            throw string("Geometry NULL");
        }
        geometry->createVBOs();
    }
    
    void draw(Camera* camera, ShaderProgram* shaderProgram) {
        if (!isVisible()) {
            return;
        }
        if (material == NULL) {
            throw string("Material NULL");
        }
        if (geometry == NULL) {
            throw std::string("Geometry NULL");
        }
        
        Matrix4 projectionMatrix = camera->getProjectionMatrix();
        
        //Transform hierachy, iteratively multiply parent rigidbody matrices
        //to get the ultimate modelmatrix that transform from object frame to world frame.
        Matrix4 modelMatrix;
        modelMatrix = transform.getModelMatrix();
        
        Entity* current = parent;
        while (current != NULL) {
            modelMatrix = current->transform.getRigidBodyMatrix() * modelMatrix;
            current = current->parent;
        }
        
        
        Matrix4 modelViewMatrix = inv(camera->getViewMatrix()) * modelMatrix;
        Matrix4 normal = normalMatrix(modelViewMatrix);
        
        modelViewMatrix.writeToColumnMajorMatrix(modelViewMat);
        projectionMatrix.writeToColumnMajorMatrix(projectionMat);
        normal.writeToColumnMajorMatrix(normalMat);
        
        glUniformMatrix4fv(shaderProgram->uModelViewMatrixLoc, 1, false, modelViewMat);
        glUniformMatrix4fv(shaderProgram->uProjectionMatrixLoc, 1, false, projectionMat);
        glUniformMatrix4fv(shaderProgram->uNormalMatrixLoc, 1, false, normalMat);
        
        glUniform3f(shaderProgram->uColorLoc, material->color[0], material->color[1], material->color[2]);
        glUniform1f(shaderProgram->uMinColorLoc, 0.2);
        
        Cvec3 lightPosWorld0 = Cvec3(0.0, 10.0, 0.0);
        Cvec4 lightPosEye0 = normalMatrix(camera->getViewMatrix()) * Cvec4(lightPosWorld0, 1);
        glUniform3f(shaderProgram->uLightPositionLoc0, lightPosEye0[0], lightPosEye0[1], lightPosEye0[2]);
        glUniform3f(shaderProgram->uLightColorLoc0, 1, 1, 1);
        glUniform3f(shaderProgram->uSpecularLightColorLoc0, 1, 1, 1);
        
        Cvec3 lightPosWorld1 = Cvec3(-10, 5, 0);
        Cvec4 lightPosEye1 = normalMatrix(camera->getViewMatrix()) * Cvec4(lightPosWorld1, 1);
        glUniform3f(shaderProgram->uLightPositionLoc1, lightPosEye1[0], lightPosEye1[1], lightPosEye1[2]);
        glUniform3f(shaderProgram->uLightColorLoc1, 1, 1, 1);
        glUniform3f(shaderProgram->uSpecularLightColorLoc1, 1, 1, 1);
        
        glUniform1f(shaderProgram->uDiffuseTextureLoc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->diffuseTexture);
        
        glUniform1f(shaderProgram->uSpecularTextureLoc, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindBuffer(GL_TEXTURE_2D, material->specularTexture);
        
        glUniform1f(shaderProgram->uNormalTextureLoc, 2);
        glActiveTexture(GL_TEXTURE2);
        glBindBuffer(GL_TEXTURE_2D, material->normalTexture);
        
        if (depthTest) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        
        geometry->draw(shaderProgram->aPositionLoc, shaderProgram->aNormalLoc, shaderProgram->aTexCoordLoc, shaderProgram->aBinormalLoc, shaderProgram->aTangentLoc);
    }
    
    std::string getName() const {
        return name;
    }
    
    void setVisible(bool visible) {
        this->visible = visible;
    }
    
    bool isVisible() const {
        return this->visible;
    }
    
    void setDepthTest(bool enable) {
        depthTest = enable;
    }
    
    bool depthTestEnabled() const {
        return depthTest;
    }
    
    
    void setPosition(Cvec3 position) {
        transform.setPosition(position);
    }
    void translate(Cvec3 translation) {
        transform.translate(translation);
    }
    const Cvec3& getPosition() {
        return transform.getPosition();
    }
    
    void setRotation(Quat rotation) {
        transform.setRotation(rotation);
    }
    void rotate(Quat rotation) {
        transform.rotate(rotation);
    }
    const Quat& getRotation() {
        return transform.getRotation();
    }
    void setScale(Cvec3 scale) {
        transform.setScale(scale);
    }
    const Cvec3& getScale() {
        return transform.getScale();
    }
    
    const Matrix4& getModelMatrix() {
        return transform.getModelMatrix();
    }

};


int Entity::id_counter = 0;


#endif /* Entity_h */
