#ifndef Scene_h
#define Scene_h

#include <unordered_map>
#include <vector>
#include <string>
#include "../entities/Entity.hpp"
#include "../entities/Entity.cpp"
#include "Camera.hpp"
#include "../programs/Shader.h"
#include "Light.h"
#include "../physics/Raycaster.h"
#include "../geometries/Sphere.h"
#include "../glObjects/FrameBufferObject.h"
class ScreenShader;
/**
 * A simple scene containing entities to be rendered.
 */
class Scene {

private:
    static std::vector<Entity*> entities;
    static std::unordered_map<std::string, Entity*> entityTable;
    static std::unordered_map<int, Shader*> shaderTable;
    
    static FrameBufferObject* frameBufferObject;
    static Entity* screen;

    Scene() {
    }

public:
    //currently only support 2 lights.
    static Light* light0;
    static Light* light1;
    
    static Camera* camera;

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
        entityTable[entity->getName()] = entity;
    }
    
    //must be called and called once before rendering
    static void createMeshes() {
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            (*it)->createMesh();
        }

        if (screen != NULL) {
            screen->createMesh();
        }
    }
    
    static bool testIntersect(Entity* entity, int x, int y, int screenWidth, int screenHeight) {
        if (entity == NULL) {
            return false;
        }
        Geometry* geometry = entity->geometry;
        return Raycaster::isPicked(x, y, screenWidth, screenHeight, camera->getProjectionMatrix(), camera->getViewMatrix(), camera->getPosition(), entity->getPosition(), geometry->getDiameter() / 2.0 * entity->getScale()[0]);
    }

    static void renderLoop() {
        for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
            Shader* shader = shaderTable[(*it)->getProgram()];
            if (shader == NULL) {
                throw std::string("shader not exists");
            }
            (*it)->draw(camera, shader, light0, light1);
        }
    }

    static void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (camera == NULL) {
            throw std::string("Camera NULL");
        }
        
        renderLoop();
        glutSwapBuffers();
    }


    static void renderToTexture() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (camera == NULL) {
            throw std::string("Camera not set");
        }
        if (frameBufferObject == NULL) {
            throw std::string("FrameBufferObject not set");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject->getFrameBuffer());
        glViewport(0, 0, 1024, 1024);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderLoop();
    }
    static void renderToScreen(Shader* shader, GLsizei windowWidth, GLsizei windowHeight) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);
        if (screen != NULL) {
            screen->draw(Scene::camera, shader, Scene::light0, Scene::light1);
        } else {
            throw std::string("screen not set");
        }
        glutSwapBuffers();
    }


    
    static Entity* getEntity(std::string name) {
        if (entityTable.find(name) == entityTable.end()) {
            return NULL;
        }
        
        return entityTable[name];
    }

    static void removeAll() {
        entities.clear();
        entityTable.clear();
        shaderTable.clear();
        delete light0;
        delete light1;
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
    
    static void addShader(Shader* shader) {
        int programId = shader->getProgramId();
        if (shaderTable.find(programId) == shaderTable.end()) {
            shaderTable[programId] = shader;
        } else {
            std::cerr << "program: " << programId << " already exists." << std::endl;
        }
    }

    //currently only support one fbo
    static void setFrameBufferObject(FrameBufferObject* fbo) {
        frameBufferObject = fbo;
    }
    static void removeFrameBufferObject() {
        frameBufferObject = NULL;
    }

    //set the target that the texture in FBO is rendered to, which will be rendered to the window.
    static void setScreen(Entity* screen) {
        Scene::screen = screen;
    }
};

std::unordered_map<std::string, Entity*> Scene::entityTable;
std::unordered_map<int, Shader*> Scene::shaderTable;
std::vector<Entity*> Scene::entities;

FrameBufferObject* Scene::frameBufferObject = NULL;
Entity* Scene::screen = NULL;

Camera* Scene::camera = NULL;

Light* Scene::light0 = NULL;
Light* Scene::light1 = NULL;
#endif /* Scene_h */
