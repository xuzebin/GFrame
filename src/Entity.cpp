//
//  Entity.cpp
//  TemplateProject
//
//  Created by xuzebin on 11/17/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"


int Entity::id_counter = 0;

void Entity::createMesh()  {
    if (geometry == NULL) {
        throw string("Geometry NULL");
    }
    geometry->createVBOs();
    
    
    initState.transform = transform;
    initState.color = material->color;
}

void Entity::draw(Camera* camera, ShaderProgram* shaderProgram, Light* light0, Light* light1) {
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
    
    const Matrix4 viewMatrix = camera->getViewMatrix();
    Matrix4 modelViewMatrix = inv(viewMatrix) * modelMatrix;
    Matrix4 normal = normalMatrix(modelViewMatrix);
    
    modelMatrix.writeToColumnMajorMatrix(modelMat);
    modelViewMatrix.writeToColumnMajorMatrix(modelViewMat);
    projectionMatrix.writeToColumnMajorMatrix(projectionMat);
    normal.writeToColumnMajorMatrix(normalMat);
    
    glUniformMatrix4fv(shaderProgram->uModelMatrixLoc, 1, false, modelMat);
    glUniformMatrix4fv(shaderProgram->uModelViewMatrixLoc, 1, false, modelViewMat);
    glUniformMatrix4fv(shaderProgram->uProjectionMatrixLoc, 1, false, projectionMat);
    glUniformMatrix4fv(shaderProgram->uNormalMatrixLoc, 1, false, normalMat);
    
    glUniform3f(shaderProgram->uColorLoc, material->color[0], material->color[1], material->color[2]);
//    glUniform1f(shaderProgram->uMinColorLoc, 0.2);

    if (light0 != NULL) {
        Cvec3 lightPosEye0 = light0->getPositionInEyeSpace(viewMatrix);
        glUniform3f(shaderProgram->uLightPositionLoc0, lightPosEye0[0], lightPosEye0[1], lightPosEye0[2]);
        Cvec3f lightColor = light0->lightColor;
        glUniform3f(shaderProgram->uLightColorLoc0, lightColor[0], lightColor[1], lightColor[2]);
        Cvec3f specularLightColor = light0->specularLightColor;
        glUniform3f(shaderProgram->uSpecularLightColorLoc0, specularLightColor[0], specularLightColor[1], specularLightColor[2]);
    }
    if (light1 != NULL) {
        Cvec3 lightPosEye1 = light1->getPositionInEyeSpace(viewMatrix);
        glUniform3f(shaderProgram->uLightPositionLoc1, lightPosEye1[0], lightPosEye1[1], lightPosEye1[2]);
        Cvec3f lightColor = light1->lightColor;
        glUniform3f(shaderProgram->uLightColorLoc1, lightColor[0], lightColor[1], lightColor[2]);
        Cvec3f specularLightColor = light1->specularLightColor;
        glUniform3f(shaderProgram->uSpecularLightColorLoc1, specularLightColor[0], specularLightColor[1], specularLightColor[2]);
    }
    

    //cubemap
//    if (material->cubemapApplied()) {
//        std::cout << "cubemap applied" << std::endl;
//        glUniform1f(shaderProgram->uEnvironmentMapLoc, 0);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, material->cubemap.texture);
//    }


    glUniform1f(shaderProgram->uDiffuseTextureLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->texture.diffuseTexture);
    
    glUniform1f(shaderProgram->uSpecularTextureLoc, 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, material->texture.specularTexture);

    glUniform1f(shaderProgram->uNormalTextureLoc, 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, material->texture.normalTexture);

//    checkGlErrors(__FILE__, __LINE__);
    
    std::cout << "dif: " << material->texture.diffuseTexture << std::endl;
    std::cout << "spe: " << material->texture.specularTexture << std::endl;
    std::cout << "nor: " << material->texture.normalTexture << std::endl;
    
    std::cout << "difLoc: " << shaderProgram->uDiffuseTextureLoc << ": " << glGetUniformLocation(shaderProgram->programId, "uDiffuseTexture") << std::endl;
    std::cout << "speLoc: " << shaderProgram->uSpecularTextureLoc << ": " << glGetUniformLocation(shaderProgram->programId, "uSpecularTexture") <<  std::endl;
    std::cout << "norLoc: " << shaderProgram->uNormalTextureLoc << ": " << glGetUniformLocation(shaderProgram->programId, "uNormalTexture") << std::endl;
    
    if (depthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    geometry->draw(shaderProgram->aPositionLoc, shaderProgram->aNormalLoc, shaderProgram->aTexCoordLoc, shaderProgram->aBinormalLoc, shaderProgram->aTangentLoc);
}

std::string Entity::getName() {
    return name;
}

void Entity::setVisible(bool visible) {
    this->visible = visible;
}

bool Entity::isVisible() const {
    return this->visible;
}

void Entity::setDepthTest(bool enable) {
    depthTest = enable;
}

bool Entity::depthTestEnabled() const {
    return depthTest;
}

void Entity::setPosition(Cvec3 position) {
    transform.setPosition(position);
}

void Entity::translate(Cvec3 translation) {
    transform.translate(translation);
}

const Cvec3& Entity::getPosition() {
    return transform.getPosition();
}

void Entity::setRotation(Quat rotation) {
    transform.setRotation(rotation);
}

void Entity::rotate(Quat rotation) {
    transform.rotate(rotation);
}

const Quat& Entity::getRotation() {
    return transform.getRotation();
}

void Entity::setScale(Cvec3 scale) {
    transform.setScale(scale);
}

const Cvec3& Entity::getScale() {
    return transform.getScale();
}

const Matrix4& Entity::getModelMatrix() {
    return transform.getModelMatrix();
}