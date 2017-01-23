#ifndef Scene_hpp
#define Scene_hpp

#include <unordered_map>
#include <vector>
#include <string>
#include "../entities/Entity.hpp"
#include "Camera.hpp"
#include "../physics/Raycaster.h"
#include "../geometries/Sphere.h"
#include "../glObjects/FrameBufferObject.hpp"

class Light;
class Shader;
class ScreenShader;
/**
 * A simple scene containing entities to be rendered.
 */
class Scene {

public:
    //currently only support 2 lights.
    static Light* light0;
    static Light* light1;
    
    static Camera* camera;

    static void setCamera(Camera* camera_) { camera = camera_; }
    static void setLight0(Light* light)    { light0 = light; }
    static void setLight1(Light* light)    { light1 = light; }

    static Camera* getCamera() { return camera; }
    static Light* getLight(int index);

    static void addChild(Entity* entity);

    //must be called and called once before rendering
    static void createMeshes();
    
    static bool testIntersect(Entity* entity, int x, int y, int screenWidth, int screenHeight);

    static void renderLoop();
    static void render();
    static void renderToTexture();
    static void renderToScreen(Shader* shader, GLsizei windowWidth, GLsizei windowHeight);
    
    static Entity* getEntity(std::string name);

    static void removeAll();

    static void updateMouseEvent(int button , int state, int x, int y, int screenWidth, int screenHeight);
    static void updateMousePassiveMotion(int x, int y, int screenWidth, int screenHeight);
    static void addShader(Shader* shader);


    //currently only support one fbo
    static void setFrameBufferObject(FrameBufferObject* fbo) { frameBufferObject = fbo; }
    static void removeFrameBufferObject()                    { frameBufferObject = NULL; }
    //set the target that the texture in FBO is rendered to, which will be rendered to the window.
    static void setScreen(Entity* screen)                    { Scene::screen = screen; }

private:
    Scene();

    static std::vector<Entity*> entities;
    static std::unordered_map<std::string, Entity*> entityTable;
    static std::unordered_map<int, Shader*> shaderTable;
    
    static FrameBufferObject* frameBufferObject;
    static Entity* screen;
};


#endif /* Scene_hpp */
