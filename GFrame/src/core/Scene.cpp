#include "Scene.hpp"

std::unordered_map<std::string, Entity*> Scene::entityTable;
std::unordered_map<int, Shader*> Scene::shaderTable;
std::vector<Entity*> Scene::entities;

FrameBufferObject* Scene::frameBufferObject = NULL;
Entity* Scene::screen = NULL;

Camera* Scene::camera = NULL;

Light* Scene::light0 = NULL;
Light* Scene::light1 = NULL;

Scene::Scene()
{
}

Light* Scene::getLight(int index) {
    switch(index) {
        case 0:
            return light0;
        case 1:
            return light1;
        default:
            throw std::string("no matched light index");
    }
}

void Scene::addChild(Entity* entity) {
    entities.push_back(entity);
    entityTable[entity->getName()] = entity;
}

void Scene::createMeshes() {
    for(auto it = entities.begin(); it != entities.end(); ++it) {
        (*it)->createMesh();
    }

    if (screen != NULL) {
        screen->createMesh();
    }
}

bool Scene::testIntersect(Entity* entity, int x, int y, int screenWidth, int screenHeight) {
    if (entity == NULL) {
        return false;
    }
    return Raycaster::isPicked(x, y, screenWidth, screenHeight, camera->getProjectionMatrix(), camera->getViewMatrix(), camera->getPosition(), entity->getPosition(), entity->getBoundingBoxLength() / 2.0);
}

void Scene::renderLoop() {
    for(auto it = entities.begin(); it != entities.end(); ++it) {
        Shader* shader = shaderTable[(*it)->getProgram()];
        if (shader == NULL) {
            throw std::string("shader not exists");
        }
        (*it)->draw(camera, shader, light0, light1);
    }
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (camera == NULL) {
        throw std::string("Camera NULL");
    }
        
    renderLoop();
    glutSwapBuffers();
}

void Scene::renderToTexture() {
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

void Scene::renderToScreen(Shader* shader, GLsizei windowWidth, GLsizei windowHeight) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);
    if (screen != NULL) {
        screen->draw(Scene::camera, shader, Scene::light0, Scene::light1);
    } else {
        throw std::string("screen not set");
    }
    glutSwapBuffers();
}

Entity* Scene::getEntity(std::string name) {
    if (entityTable.find(name) == entityTable.end()) {
        return NULL;
    }
        
    return entityTable[name];
}

void Scene::removeAll() {
    entities.clear();
    entityTable.clear();
    shaderTable.clear();
    delete light0;
    delete light1;
}

//TODO change method name
void Scene::updateMouseEvent(int button , int state, int x, int y, int screenWidth, int screenHeight) {
    //test intersection
        
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //NOTE: currently only supports ray sphere intersection.
        for(auto it = entities.begin(); it != entities.end(); ++it) {
            if ((*it)->clickEventListenerRegistered()) {
                if (testIntersect(*it, x, y, screenWidth, screenHeight)) {
                    (*it)->notify(EventType::CLICK);
                }
            }
        }
    } else {
        for(auto it = entities.begin(); it != entities.end(); ++it) {
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

void Scene::updateMousePassiveMotion(int x, int y, int screenWidth, int screenHeight) {
    for(auto it = entities.begin(); it != entities.end(); ++it) {
        if ((*it)->clickEventListenerRegistered()) {
            if (testIntersect(*it, x, y, screenWidth, screenHeight)) {
                (*it)->notify(EventType::HOVER);
            } else {
                (*it)->notify(EventType::IDLE);
            }
        }
    }
}
    
void Scene::addShader(Shader* shader) {
    int programId = shader->getProgramId();
    if (shaderTable.find(programId) == shaderTable.end()) {
        shaderTable[programId] = shader;
    } else {
        std::cerr << "program: " << programId << " already exists." << std::endl;
    }
}
