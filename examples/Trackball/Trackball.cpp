#include <GLUT/glut.h>
#include "base/glsupport.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "materials/Material.hpp"
#include "core/Scene.hpp"
#include "core/Camera.hpp"
#include "entities/Model.hpp"
#include "core/Light.hpp"
#include "programs/ColorShader.h"
#include "controls/Trackball.hpp"

class Shader;

int screenWidth = 600;
int screenHeight = 600;

Trackball trackball;

void display(void) {
    Scene::render();
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

    auto colorShader = std::make_shared<ColorShader>();
    colorShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    auto camera = make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);

    auto light0 = std::make_shared<Light>();
    light0->setPosition(1, 5, -5);

    Scene::setLight0(light0);

    auto model0 = std::make_shared<Model>("assets/models/torus/catmark_torus_creases0.obj", "model0", "assets/models/torus/");
    model0->material->setColor(0.6, 0.6, 0.6);
    model0->setPosition(Cvec3(0, 0, -5));
    model0->setRotation(Quat::makeYRotation(30) * Quat::makeXRotation(-30));
    model0->setShader(colorShader);
    Scene::addChild(model0);

    //set trackball params
    trackball.setScreenSize(screenWidth, screenHeight);
    trackball.setRadius(screenWidth < screenHeight ? screenWidth / 2 : screenHeight / 2);
    trackball.setSpeed(3.0f);
    trackball.setTarget(model0);

    // genereate vbo/ibo for the geometry of each Entity.
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
            std::cout << "position: " << model->getPosition() << std::endl;
            std::cout << "rotation: " << model->getRotation() << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Trackball");
    
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

