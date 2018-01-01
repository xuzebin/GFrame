/**
 * This example shows how to realize offscreen rendering using GFrame.
 * The scene is first rendered to a texture that is bound to a Framebuffer Object,
 * and then this texture is rendered to the screen.
 */

#include <GLUT/glut.h>
#include "base/glsupport.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "materials/Material.hpp"
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "geometries/Screen.h"
#include "entities/Model.hpp"
#include "core/Light.hpp"
#include "programs/Shader.h"
#include "programs/ModelShader.h"
#include "programs/ScreenShader.h"
#include "glObjects/FrameBufferObject.hpp"

int screenWidth = 600;
int screenHeight = 600;

void display(void) {
    auto model = Scene::getEntity("model0");
    model->rotate(Quat::makeYRotation(0.5));

    Scene::renderToTexture();// Render to the texture (FBO)
    Scene::renderToScreen(screenWidth, screenHeight);// Render the texture to screen
}

void init(void) {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    
    std::string vertexShader = "shaders/vertex_shader_model.glsl";
    std::string fragmentShader = "shaders/fragment_shader_model.glsl";

    // Shader for model
    auto modelShader = std::make_shared<ModelShader>();
    modelShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    // Shader for rendering the FBO to screen
    auto screenShader = std::make_shared<ScreenShader>();
    screenShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_offscreen.glsl");    

    // Frame buffer object config
    auto fbo = std::make_shared<FrameBufferObject>(1024, 1024, true);
    Scene::setFrameBufferObject(fbo);

    // Offscreen rendering to to a square plane
    auto plane = std::make_shared<Screen>();
    auto planeM = std::make_shared<Material>();
    planeM->setDiffuseTextureId(fbo->getFrameBufferTexture());
    auto screen = std::make_shared<Entity>(plane, planeM);
    screen->setShader(screenShader);
    Scene::setScreen(screen);

    // Camera config
    auto camera = std::make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);

    // Lights config
    auto light0 = std::make_shared<Light>();
    light0->setPosition(1, 5, 0);
    auto light1 = std::make_shared<Light>();
    light1->setPosition(-1, 0, 0);
    Scene::setLight0(light0);
    Scene::setLight1(light1);

    // Model config
    auto model0 = std::make_shared<Model>("assets/models/monk/Monk_Giveaway_Fixed.obj", "model0", "assets/models/monk/");
    model0->setPosition(Cvec3(0, 0, -2));
    model0->setRotation(Quat::makeYRotation(20));
    model0->setShader(modelShader);
    Scene::addChild(model0);

    // Genereate vbo/ibo for the geometry of each Entity.
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
    glutCreateWindow("OffscreenRendering");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboard);

    init();
    glutMainLoop();
    return 0;
}

