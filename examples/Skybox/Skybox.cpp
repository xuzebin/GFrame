#include <GLUT/glut.h>
#include "base/glsupport.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "core/Scene.hpp"
#include "core/Camera.hpp"
#include "core/Light.hpp"
#include "entities/Entity.hpp"
#include "materials/Cubemap.hpp"
#include "materials/Material.hpp"
#include "geometries/Cube.h"
#include "programs/TextureShader.h"

int screenWidth = 600;
int screenHeight = 600;

void display(void) {
    auto camera = Scene::getCamera();
    camera->rotate(Quat::makeYRotation(0.2));

    Scene::render();
}

void init(void) {
    /** GL config **/
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);

    /** Camera config **/
    auto camera = make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);

    /** Shader config **/
    auto cubemapShader = std::make_shared<TextureShader>();
    cubemapShader->createProgram("shaders/vertex_shader_cubemap.glsl", "shaders/fragment_shader_cubemap.glsl");

    /** Material config **/
    Cubemap cubemap;
    cubemap.loadTextures("assets/cubemap/posx.jpg", "assets/cubemap/negx.jpg", "assets/cubemap/posy.jpg", "assets/cubemap/negy.jpg", "assets/cubemap/posz.jpg", "assets/cubemap/negz.jpg");

    auto cubemapM = std::make_shared<Material>();
    cubemapM->setCubemap(cubemap.getTexture());

    /** Geometry config **/
    auto skyGeo = std::make_shared<Cube>(100);

    /** Entity config **/
    auto skybox = std::make_shared<Entity>(skyGeo, cubemapM, "skybox");
    skybox->setShader(cubemapShader);
    skybox->setRotation(Quat::makeYRotation(180));

    /** Add to Scene **/
    Scene::addChild(skybox);

    // genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;
}

void idle(void) {
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 'Q':
        {
            exit(0);
            break;
        }
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Skybox");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboard);

    init();
    glutMainLoop();
    return 0;
}


