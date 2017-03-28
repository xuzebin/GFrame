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

Trackball trackball(screenWidth, screenHeight);

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
//    std::string vertexShader = "shaders/vertex_shader_model.glsl";
    std::string fragmentShader = "shaders/fragment_shader_color.glsl";
     //    std::string fragmentShader = "shaders/fragment_shader_model.glsl";

    auto colorShader = std::make_shared<ColorShader>();
    colorShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());
//     auto colorShader = std::make_shared<ModelShader>();
//     colorShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    auto camera = make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);

    auto light0 = std::make_shared<Light>();
    light0->setPosition(0, 0, 5);
    light0->lightColor = Cvec3f(1, 1, 1);

    Scene::setLight0(light0);

//     auto model0 = std::make_shared<Model>("assets/models/torus/catmark_torus_creases0.obj", "model0", "assets/models/torus/");
    // auto model0 = std::make_shared<Model>("assets/models/knot/texturedknot.obj", "model0", "assets/models/knot/");
    auto model0 = std::make_shared<Model>("assets/models/head/head.obj", "model0", "assets/models/head/");
  //   auto model0 = std::make_shared<Model>("assets/models/sportsCar/sportsCar.obj", "model0", "assets/models/sportsCar/");

    model0->material->setColor(0.8, 0.8, 0.8);
    model0->translate(Cvec3(0, 0, -4));

    model0->setRotation(Quat::makeYRotation(30) * Quat::makeXRotation(-30));
    model0->setShader(colorShader);
    Scene::addChild(model0);

    //set trackball params
    trackball.setInitRotation(model0->getRotation());

    // genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;
    trackball.updateScreenSize(w, h);
}

void idle(void) {
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    auto model = Scene::getEntity("model0");
    switch (key) {
        case 'q':
        case 'Q':
        {
            exit(0);
            break;
        }
        case 'w':
            model->translate(Cvec3(0, 0.2, 0));
            break;
        case 's':
            model->translate(Cvec3(0, -0.2, 0));
            break;
        case 'a':
            model->translate(Cvec3(-0.2, 0, 0));
            break;
        case 'd':
            model->translate(Cvec3(0.2, 0, 0));
        case 'z':
            model->translate(Cvec3(0, 0, -0.2));//?
        case 'x':
            model->translate(Cvec3(0, 0, 0.2));
            break;
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
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

