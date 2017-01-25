#include <GLUT/glut.h>
#include "base/glsupport.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "materials/Material.hpp"
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "entities/Model.hpp"
#include "core/Light.hpp"
#include "programs/Shader.h"
#include "programs/ModelShader.h"

int screenWidth = 600;
int screenHeight = 600;


void display(void) {
    auto model = Scene::getEntity("model0");
    model->rotate(Quat::makeYRotation(0.5));
    Scene::render();
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

    auto modelShader = std::make_shared<ModelShader>();
    modelShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());
    Scene::addShader(modelShader);

    auto camera = std::make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);

    auto light0 = std::make_shared<Light>();
    light0->setPosition(1, 5, -5);

    auto light1 = std::make_shared<Light>();
    light1->setPosition(-1, 0, -4);

    Scene::setLight0(light0);
    Scene::setLight1(light1);

    auto model0 = std::make_shared<Model>("assets/models/monk/Monk_Giveaway_Fixed.obj", "model0", "assets/models/monk/");
    model0->setScale(Cvec3(0.5, 0.5, 0.5));
    model0->setPosition(Cvec3(0, -3.4, -9));
    model0->setRotation(Quat::makeYRotation(20));
    model0->setShader(modelShader);
    Scene::addChild(model0);

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
    glutCreateWindow("Textured ObjModel");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboard);

    init();
    glutMainLoop();
    return 0;
}

