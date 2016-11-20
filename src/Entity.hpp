//
//  Entity.hpp
//  TemplateProject
//
//  Created by xuzebin on 11/17/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "Transform.h"
#include "Geometry.hpp"
#include "Material.h"
#include "Camera.h"
#include "ShaderProgram.h"

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
    
    GLfloat modelMat[16];
    GLfloat modelViewMat[16];
    GLfloat projectionMat[16];
    GLfloat normalMat[16];
    
    void setName(int counter) {
        if (name.empty()) {
            name = "Entity" + std::to_string(counter);
        }
    }
    
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
        
        setName(id_counter);
    }
    
    ~Entity() {
        delete material;
    }
    
    void createMesh();
    void draw(Camera* camera, ShaderProgram* shaderProgram);
    std::string getName();
    void setVisible(bool visible);
    bool isVisible() const;
    void setDepthTest(bool enable);
    bool depthTestEnabled() const;
    void setPosition(Cvec3 position);
    void translate(Cvec3 translation);
    const Cvec3& getPosition();
    void setRotation(Quat rotation);
    void rotate(Quat rotation);
    const Quat& getRotation();
    void setScale(Cvec3 scale);
    const Cvec3& getScale();
    const Matrix4& getModelMatrix();
};


#endif /* Entity_hpp */
