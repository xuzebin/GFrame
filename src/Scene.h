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
#include "Light.h"
#include "Raycaster.h"
#include "Sphere.h"

/**
 * A simple scene containing entities to be rendered.
 */
class Scene {

private:
    static std::vector<Entity*> entities;
    static std::unordered_map<std::string, Entity*> table;
    
    static Camera* camera;
    
    Scene() {}
    

public:
    //currently only support 2 lights.
    static Light* light0;
    static Light* light1;

    
    static void setCamera(Camera* camera_) {
        camera = camera_;
    }
    
    static Camera* getCamera() {
        return camera;
    }
    
    static void setLight0(Light* light) {
        light0 = light;
    }
    
    static void setLight1(Light* light) {
        light1 = light;
    }
    
    static Light* getLight(int index) {
        switch(index) {
            case 0:
                return light0;
            case 1:
                return light1;
            default:
                throw std::string("no matched light index");
        }
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
    
    static bool testIntersect(Entity* entity, int x, int y, int screenWidth, int screenHeight) {
        if (entity == NULL) {
            return false;
        }
        Geometry* geometry = entity->geometry;
        return Raycaster::isPicked(x, y, screenWidth, screenHeight, camera->getProjectionMatrix(), camera->getViewMatrix(), camera->getPosition(), entity->getPosition(), geometry->getDiameter() / 2.0 * entity->getScale()[0]);
    }
    
    static void render(ShaderProgram* shaderProgram) {
        if (camera == NULL) {
            throw std::string("Camera NULL");
        }
        
        glUseProgram(shaderProgram->programId);
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            (*it)->draw(camera, shaderProgram, light0, light1);
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

    
    static void updateMouseEvent(int button , int state, int x, int y, int screenWidth, int screenHeight) {
        //test intersection
        
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            //NOTE: currently only supports ray sphere intersection.
            for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
                if ((*it)->clickEventListenerRegistered()) {
                    if (testIntersect(*it, x, y, screenWidth, screenHeight)) {
                        (*it)->notify(EventType::CLICK);
                    }
                }
            }
        } else {
            for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
                if ((*it)->clickEventListenerRegistered()) {
                    if (testIntersect(*it, x, y, screenWidth, screenHeight)) {
                        (*it)->notify(EventType::HOVER);
                    } else {
                        (*it)->notify(EventType::IDLE);
                    }
                }
            }
        }
    }
    static void updateMousePassiveMotion(int x, int y, int screenWidth, int screenHeight) {
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            if ((*it)->clickEventListenerRegistered()) {
                if (testIntersect(*it, x, y, screenWidth, screenHeight)) {
                    (*it)->notify(EventType::HOVER);
                } else {
                    (*it)->notify(EventType::IDLE);
                }
            }
        }
    }
};

std::unordered_map<std::string, Entity*> Scene::table;
std::vector<Entity*> Scene::entities;
Camera* Scene::camera = NULL;

Light* Scene::light0 = NULL;
Light* Scene::light1 = NULL;
#endif /* Scene_h */
