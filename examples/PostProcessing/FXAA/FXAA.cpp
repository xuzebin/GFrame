/**
 * This example shows how to realize FXAA post-processing effects using GFrame.
 * By toggling key 's' you can see the difference between using and not using FXAA for the model.
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
#include "programs/ColorShader.h"
#include "programs/ScreenShader.h"
#include "glObjects/FrameBufferObject.hpp"
#include "controls/Trackball.hpp"

int screenWidth = 600;
int screenHeight = 600;

Trackball trackball;

std::shared_ptr<Entity> screen;
std::shared_ptr<ScreenShader> screenShader, simpleShader;

void display(void) {
    Scene::renderToTexture();// Render to the texture (FBO)
    Scene::renderToScreen(screenWidth, screenHeight);// Render the texture to screen
}

void init(void) {
    glClearColor(0.7, 0.7, 0.7, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    
    std::string vertexShader = "shaders/vertex_shader_simple.glsl";
    std::string fragmentShader = "shaders/fragment_shader_color.glsl";

    // Shader for model
    auto modelShader = std::make_shared<ColorShader>();
    modelShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    // FXAA shader
    screenShader = std::make_shared<ScreenShader>();
    screenShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_fxaa.glsl");

    // Simple shader (without using FXAA)
    simpleShader = std::make_shared<ScreenShader>();
    simpleShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_offscreen.glsl");

    // Frame buffer object config
    auto fbo = std::make_shared<FrameBufferObject>(1024, 1024, false);
    Scene::setFrameBufferObject(fbo);

    // Offscreen rendering to to a square plane
    auto plane = std::make_shared<Screen>();
    auto planeM = std::make_shared<Material>();
    planeM->setDiffuseTextureId(fbo->getFrameBufferTexture());
    screen = std::make_shared<Entity>(plane, planeM);
    screen->setShader(screenShader);
    Scene::setScreen(screen);

    // Camera config
    auto camera = std::make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);

    // Lights config
    auto light0 = std::make_shared<Light>();
    light0->setPosition(1, 5, -5);
    auto light1 = std::make_shared<Light>();
    light1->setPosition(-1, 0, -4);
    Scene::setLight0(light0);
    Scene::setLight1(light1);

    // Model config
    auto model0 = std::make_shared<Model>("assets/models/torus/catmark_torus_creases0.obj", "model0", "assets/models/torus/");
    model0->material->setColor(0.6, 0.6, 0.6);
    model0->setPosition(Cvec3(0, 0, -6));
    model0->setRotation(Quat::makeYRotation(20) * Quat::makeXRotation(-20));
    model0->setShader(modelShader);
    Scene::addChild(model0);

    //set trackball params
    trackball.setScreenSize(screenWidth, screenHeight);
    trackball.setRadius(screenWidth < screenHeight ? screenWidth / 2 : screenHeight / 2);
    trackball.setSpeed(3.0f);
    trackball.setTarget(model0);

    // Genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;
    trackball.setScreenSize(w, h);
    trackball.setRadius(w < h ? w / 2 : h / 2);
}

void idle(void) {
    glutPostRedisplay();
}

bool fxaaEnabled = true;
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 's':
        case 'S':
        {
            if (fxaaEnabled) {
                //Use Simple shader
                screen->setShader(simpleShader);
            } else {
                //Use FXAA shader
                screen->setShader(screenShader);
            }
            fxaaEnabled = !fxaaEnabled;
            break;
        }
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

bool mouseLeftDown = false;

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            trackball.record(x, y);
            mouseLeftDown = true;
        } else if (state == GLUT_UP) {
            mouseLeftDown = false;
        }
    }
}

void motion(int x, int y) {
    if (mouseLeftDown) {
        auto model = Scene::getEntity("model0");
        if (model != NULL) {
            Quat rotation = trackball.getRotation(x, y);
            model->setRotation(rotation);
        }
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("FXAA");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    init();
    glutMainLoop();
    return 0;
}

