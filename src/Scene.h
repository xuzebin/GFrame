//
//  Scene.h
//  TemplateProject
//
//  Created by xuzebin on 10/20/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include <unordered_map>

#include "Entity.h"
#include "Camera.h"
#include "ShaderProgram.h"

/**
 * A simple scene containing entities to be rendered.
 */
class Scene {

private:
    std::vector<Entity*> entities;
    static std::unordered_map<std::string, Entity*> table;
    
    
public:
    
//    void addChild(Entity* entity);
//    void createMeshes();
//    void render(Camera* camera, ShaderProgram* shaderProgram);
//    Entity* getEntity(std::string name);
//    void removeAll();
    
    void addChild(Entity* entity) {
        entities.push_back(entity);
        
        table[entity->getName()] = entity;
    }
    
    //must be called and called once before rendering
    void createMeshes() {
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            (*it)->createMesh();
        }
    }
    
    void render(Camera* camera, ShaderProgram* shaderProgram) {
        glUseProgram(shaderProgram->programId);
        
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            (*it)->draw(camera, shaderProgram);
        }
    }
    
    Entity* getEntity(std::string name) {
        if (table.find(name) == table.end()) {
            return NULL;
        }
        
        return table[name];
    }
    
    void removeAll() {
        entities.clear();
        table.clear();
    }
    
};

std::unordered_map<std::string, Entity*> Scene::table;

#endif /* Scene_h */
