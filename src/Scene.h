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
#include <vector>
#include <string>
#include "Entity.hpp"
#include "Camera.h"
#include "ShaderProgram.h"


/**
 * A simple scene containing entities to be rendered.
 */
class Scene {

private:
    static std::vector<Entity*> entities;
    static std::unordered_map<std::string, Entity*> table;
    
    static Camera* camera;
    
    Scene();
    
public:
    
//    void addChild(Entity* entity);
//    void createMeshes();
//    void render(Camera* camera, ShaderProgram* shaderProgram);
//    Entity* getEntity(std::string name);
//    void removeAll();
    
    static void setCamera(Camera* camera_) {
        camera = camera_;
    }
    
    static Camera* getCamera() {
        return camera;
    }
    
    static void addChild(Entity* entity) {
        entities.push_back(entity);
        
        table[entity->getName()] = entity;
    }
    
    //must be called and called once before rendering
    static void createMeshes() {
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            (*it)->createMesh();
        }
    }
    
//    static void render(Camera* camera, ShaderProgram* shaderProgram) {
//        glUseProgram(shaderProgram->programId);
//        
//        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
//            (*it)->draw(camera, shaderProgram);
//        }
//    }
    
    static void render(ShaderProgram* shaderProgram) {
        if (camera == NULL) {
            throw std::string("Camera NULL");
        }
        glUseProgram(shaderProgram->programId);
        
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            (*it)->draw(camera, shaderProgram);
        }
    }
    
    static Entity* getEntity(std::string name) {
        if (table.find(name) == table.end()) {
            return NULL;
        }
        
        return table[name];
    }
    
    static void removeAll() {
        entities.clear();
        table.clear();
    }
    
};

std::unordered_map<std::string, Entity*> Scene::table;
std::vector<Entity*> Scene::entities;
Camera* Scene::camera = NULL;

#endif /* Scene_h */
