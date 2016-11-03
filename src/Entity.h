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

/**
 * The object/Entity to be rendered.
 */
struct Entity {
    Transform transform;
    Geometry* geometry;
    Material* material;
    
    Entity* parent;
    
private:
    static int id_counter;

    std::string name;
    
    bool visible;
    bool depthTest;
    
    GLfloat modelViewMat[16];
    GLfloat projectionMat[16];
    GLfloat normalMat[16];
    
public:
    
    Entity(Geometry* geometry_, Material* material_) : Entity("", geometry_, material_, NULL) {}
    
    Entity(std::string name_, Geometry* geometry_, Material* material_) : Entity(name_, geometry_, material_, NULL) {}
 
    Entity(Geometry* geometry_, Material* material_, Entity* parent_)
    : Entity("", geometry_, material_, parent_) {}

    Entity(std::string name_, Geometry* geometry_, Material* material_, Entity* parent_)
    : name(name_), geometry(geometry_), material(material_), parent(parent_) {
        
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
    
    void createMesh()  {
        if (geometry == NULL) {
            throw string("Geometry NULL");
        }
        geometry->createVBOs();
    }
    
    void draw(const Matrix4& eyeInverseMatrix, const Matrix4& projectionMatrix, ShaderProgram* shaderProgram) {
        if (!isVisible()) {
            return;
        }
        if (material == NULL) {
            throw string("Material NULL");
        }
        if (geometry == NULL) {
            throw string("Geometry NULL");
        }
        
        
        //Transform hierachy, iteratively multiply parent rigidbody matrices
        //to get the ultimate modelmatrix that transform from object frame to world frame.
        Matrix4 modelMatrix;
        modelMatrix = transform.getModelMatrix();
       
        Entity* current = parent;
        while (current != NULL) {
            modelMatrix = current->transform.getRigidBodyMatrix() * modelMatrix;
            current = current->parent;
        }
        
        
        Matrix4 modelViewMatrix = eyeInverseMatrix * modelMatrix;
        Matrix4 normal = normalMatrix(modelViewMatrix);
        
        modelViewMatrix.writeToColumnMajorMatrix(modelViewMat);
        projectionMatrix.writeToColumnMajorMatrix(projectionMat);
        normal.writeToColumnMajorMatrix(normalMat);
        
        glUniformMatrix4fv(shaderProgram->uModelViewMatrixLoc, 1, false, modelViewMat);
        glUniformMatrix4fv(shaderProgram->uProjectionMatrixLoc, 1, false, projectionMat);
        glUniformMatrix4fv(shaderProgram->uNormalMatrixLoc, 1, false, normalMat);

        glUniform3f(shaderProgram->uColorLoc, material->color[0], material->color[1], material->color[2]);
        glUniform1f(shaderProgram->uMinColorLoc, 0.2);
 
        glBindTexture(GL_TEXTURE_2D, material->texture);
        glUniform1i(material->texture, 0);

        if (depthTest) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }

        geometry->draw(shaderProgram->aPositionLoc, shaderProgram->aNormalLoc, shaderProgram->aTexCoordLoc);
    }
    
    string getName() {
        return name;
    }
    
    void setVisible(bool visible) {
        this->visible = visible;
    }
    bool isVisible() {
        return this->visible;
    }
    
    void setDepthTest(bool enable) {
        depthTest = enable;
    }
    bool depthTestEnabled() {
        return depthTest;
    }
    

};

int Entity::id_counter = 0;



#endif /* Entity_h */
