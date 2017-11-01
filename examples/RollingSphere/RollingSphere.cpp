#include <GLUT/glut.h>
#include "base/glsupport.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "entities/Entity.hpp"
#include "materials/Material.hpp"
#include "materials/Color.h"
#include "geometries/Sphere.h"
#include "geometries/Plane.h"
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "core/Light.hpp"
#include "programs/Shader.h"
#include "programs/ColorShader.h"

int screenWidth = 600;
int screenHeight = 600;

bool wireframe = true;
bool enableAnimation = true;


void display(void) {
    //    Scene::render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto ball = Scene::getEntity("button");
    auto ground = Scene::getEntity("ground");

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    ball->draw(Scene::getCamera(), ball->getShader(), Scene::getLight(0), Scene::getLight(1));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ground->draw(Scene::getCamera(), ground->getShader(), Scene::getLight(0), Scene::getLight(1));

    glutSwapBuffers();
}

void init(void) {
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    std::string vertexShader = "shaders/vertex_shader_simple.glsl";
    std::string fragmentShader = "shaders/fragment_shader_color.glsl";

    /** Shader config **/
    auto colorShader = std::make_shared<ColorShader>();
    colorShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    /** Camera config **/
    auto camera = std::make_shared<Camera>(Cvec3(5, 5, 10), Quat::makeXRotation(-20) * Quat::makeYRotation(25));
    Scene::setCamera(camera);

    /** Lights config **/
    auto light0 = std::make_shared<Light>();
    light0->setPosition(0, 0, 10);
    auto light1 = std::make_shared<Light>();
    light1->setPosition(-1, 0, -4);

    Scene::setLight0(light0);
    Scene::setLight1(light1);

    /** Button config **/
    auto sphere = std::make_shared<Sphere>(1, 8, 8);
    auto material = std::make_shared<Material>(Color::RED);
    auto button = std::make_shared<Entity>(sphere, material, "button");
    button->setShader(colorShader);
    Scene::addChild(button);


    auto plane = std::make_shared<Plane>(2);
    auto material2 = std::make_shared<Material>(Cvec3f(0.0, 1.0, 0.0));
    auto ground = std::make_shared<Entity>(plane, material2, "ground");
    ground->setPosition(Cvec3(0, -1, 0));
    ground->setScale(Cvec3(3, 3, 3));
    ground->setShader(colorShader);
    Scene::addChild(ground);

    // genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;
}

double getD(double angle = 1.0) {
    return angle * 2 * 3.14159 / 360.0;
}

Quat lastOrietation;
Cvec3 currentDirection(1, 0, 1);
const Cvec3 upAxis(0, 1, 0);
double angle = 0.0;
int timeSinceStart = -1;
void resetOrientation() {
    lastOrietation = Quat::makeXRotation(0);
}
void resetAngles() {
    angle = 0.0;
}
float a = 0.5;
void idle(void) {
    auto btn = Scene::getEntity("button");
    if (timeSinceStart == -1) {
        timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    }
    float dif = (float)glutGet(GLUT_ELAPSED_TIME)/10.0f - timeSinceStart/10.0f;
    angle += dif;
//     float dif = 0.5;
//     angle += dif;
    timeSinceStart = (float)glutGet(GLUT_ELAPSED_TIME);
    btn->translate(currentDirection.normalize() * getD(dif));
    btn->setRotation(Quat::makeRotationAroundAxis(cross(currentDirection, upAxis), -angle) * lastOrietation);
    
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    auto btn = Scene::getEntity("button");
    switch (key) {
        case 'q':
        case 'Q':
        {
            exit(0);
            break;
        }
        case 'z':
        case 'Z':
        {
            wireframe = !wireframe;
            break;
        }
        case 'r':
        case 'R':
        {
            currentDirection[0] *= -1;
            currentDirection[2] *= -1;
            lastOrietation = btn->getRotation();
            resetAngles();
            break;
        }
        case 'n':
        case 'N':
        {
            enableAnimation = !enableAnimation;
            if (enableAnimation) {
                glutIdleFunc(idle);
                timeSinceStart = -1;//need to update to latest time
            } else {
                glutIdleFunc(NULL);
            }
            break;
        }
        case 'x':
        case 'X':
        {
            //reset to initial
            btn->setPosition(btn->initState.transform.getPosition());
            btn->setRotation(Quat::makeXRotation(0));
            currentDirection[0] = 1;
            currentDirection[1] = 0;
            currentDirection[2] = 1;
            resetOrientation();
            resetAngles();
            break;
        }
        case 'w':
        case 'W':
        {
            currentDirection[0] = 0;
            currentDirection[1] = 0;
            currentDirection[2] = -1;
            lastOrietation = btn->getRotation();
            resetAngles();
            break;
        }
        case 's':
        case 'S':
        {
            currentDirection[0] = 0;
            currentDirection[1] = 0;
            currentDirection[2] = 1;
            lastOrietation = btn->getRotation();
            resetAngles();
            break;
        }
        case 'a':
        case 'A':
        {
            currentDirection[0] = -1;
            currentDirection[1] = 0;
            currentDirection[2] = 0;
            lastOrietation = btn->getRotation();
            resetAngles();
            break;
        }
        case 'd':
        case 'D':
        {
            currentDirection[0] = 1;
            currentDirection[1] = 0;
            currentDirection[2] = 0;
            lastOrietation = btn->getRotation();
            resetAngles();
            break;
        }
        default:
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    Scene::updateMouseEvent(button, state, x, y, screenWidth, screenHeight);
}

bool insideWindow(int x, int y) {
    return (x >= 0 && y >= 0 && x <= screenWidth && y <= screenHeight);
}

void passiveMotion(int x, int y) {
    if (insideWindow(x, y)) {
         Scene::updateMousePassiveMotion(x, y, screenWidth, screenHeight);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Rolling Sphere");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);

    init();
    glutMainLoop();
    return 0;
}

