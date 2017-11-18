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
#include "base/cvec.h"

int screenWidth = 600;
int screenHeight = 600;

bool wireframe = true;
bool enableAnimation = true;


void display(void) {
    //    Scene::render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto ball = Scene::getEntity("button");
    auto ground = Scene::getEntity("ground");
    auto shadow = Scene::getEntity("shadow");

    //draw ground
    glDepthMask(GL_FALSE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    glEnable(GL_POLYGON_OFFSET_FILL);
    //    glPolygonOffset(1, 1);
    ground->draw(Scene::getCamera(), ground->getShader(), Scene::getLight(0), Scene::getLight(1));
    //    glDisable(GL_POLYGON_OFFSET_FILL);

    glDepthMask(GL_TRUE);
    //draw sphere
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    ball->draw(Scene::getCamera(), ball->getShader(), Scene::getLight(0), Scene::getLight(1));

    //draw shadow

    //first update shadow's modelview matrix
    shadow->setModelMatrix(ball->getModelMatrix());

    auto shadowShader = shadow->getShader();
    GLfloat modelMat[16], viewMat[16];
    shadow->getModelMatrix().writeToColumnMajorMatrix(modelMat);
    const Matrix4& invViewMat = inv(Scene::getCamera()->getViewMatrix());
    invViewMat.writeToColumnMajorMatrix(viewMat);

    glUseProgram(shadowShader->getProgramId());
    GLint modelMatLoc = glGetUniformLocation(shadowShader->getProgramId(), "uModelMatrix");
    GLint viewMatLoc = glGetUniformLocation(shadowShader->getProgramId(), "uViewMatrix");

    glUniformMatrix4fv(modelMatLoc, 1, false, modelMat);
    glUniformMatrix4fv(viewMatLoc, 1, false, viewMat);

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    shadow->draw(Scene::getCamera(), shadow->getShader(), Scene::getLight(0), Scene::getLight(1));

    glutSwapBuffers();
}

void createShadowMatrix(const Cvec3& lightPosition, const Cvec3& groundPosition, Matrix4& shadowMatrix) {
    //assume ground is at (0,0,0)
    Cvec4 light(lightPosition, 1.0);

    shadowMatrix(0,0) = light[1];
    shadowMatrix(0,1) = -light[0];
    shadowMatrix(0,2) = 0;
    shadowMatrix(0,3) = 0;

    shadowMatrix(1,0) = 0;
    shadowMatrix(1,1) = 0;
    shadowMatrix(1,2) = 0;
    shadowMatrix(1,3) = 0;

    shadowMatrix(2,0) = 0;
    shadowMatrix(2,1) = -light[2];
    shadowMatrix(2,2) = light[1];
    shadowMatrix(2,3) = 0;

    shadowMatrix(3,0) = 0;
    shadowMatrix(3,1) = -light[3];
    shadowMatrix(3,2) = 0;
    shadowMatrix(3,3) = light[1];
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

    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    light0->setPosition(-5, 10, -10);
    auto light1 = std::make_shared<Light>();
    light1->setPosition(-1, 0, -4);

    Scene::setLight0(light0);
    //    Scene::setLight1(light1);

    /** sphere **/
    auto sphere = std::make_shared<Sphere>(1, 20, 20);
    auto material = std::make_shared<Material>(Color::RED);
    auto button = std::make_shared<Entity>(sphere, material, "button");
    button->setShader(colorShader);
    button->setPosition(Cvec3(0, 1, 0));
    Scene::addChild(button);

    // ground
    auto plane = std::make_shared<Plane>(2);
    auto material2 = std::make_shared<Material>(Cvec3f(0.0, 1.0, 0.0));
    auto ground = std::make_shared<Entity>(plane, material2, "ground");
    ground->setScale(Cvec3(3, 3, 3));
    ground->setShader(colorShader);
    Scene::addChild(ground);

    /** sphere shadow  **/
    std::string shadowShaderFile = "shaders/vertex_shader_shadow.glsl";
    auto shadowShader = std::make_shared<ColorShader>();
    shadowShader->createProgram(shadowShaderFile.c_str(), fragmentShader.c_str());
    auto materialShadow = std::make_shared<Material>(Cvec3f(0.25, 0.25, 0.25));
    auto sphereShadow = std::make_shared<Entity>(sphere, materialShadow, "shadow");
    sphereShadow->setShader(shadowShader);
    Scene::addChild(sphereShadow);
    Matrix4 shadowMatrix;
    GLfloat shadowMat[16];
    createShadowMatrix(light0->getPosition(), ground->getPosition(), shadowMatrix);
    shadowMatrix.writeToColumnMajorMatrix(shadowMat);

    glUseProgram(shadowShader->getProgramId());
    GLint shadowMatrixLocation = glGetUniformLocation(shadowShader->getProgramId(), "uShadowMatrix");
    glUniformMatrix4fv(shadowMatrixLocation, 1, false, shadowMat);
    checkGlErrors(__FILE__, __LINE__);

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
    float dif = (float)glutGet(GLUT_ELAPSED_TIME)/30.0f - timeSinceStart/30.0f;
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
        case 'p':
        case 'P':
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
        case 'i':
        case 'I':
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
        case 'x':
        {
            auto camera = Scene::getCamera();
            camera->setPosition(camera->getPosition() + Cvec3(-1, 0, 0));
            camera->updateView(Cvec3(0, 0, 0));
            break;
        }
        case 'X':
        {
            auto camera = Scene::getCamera();
            camera->setPosition(camera->getPosition() + Cvec3(1, 0, 0));
            camera->updateView(Cvec3(0, 0, 0));
            break;
        }
        case 'y':
        {
            auto camera = Scene::getCamera();
            camera->setPosition(camera->getPosition() + Cvec3(0, -1, 0));
            camera->updateView(Cvec3(0, 0, 0));
            break;
        }
        case 'Y':
        {
            auto camera = Scene::getCamera();
            camera->setPosition(camera->getPosition() + Cvec3(0, 1, 0));
            camera->updateView(Cvec3(0, 0, 0));
            break;
        }
        case 'z':
        {
            auto camera = Scene::getCamera();
            camera->setPosition(camera->getPosition() + Cvec3(0, 0, -1));
            camera->updateView(Cvec3(0, 0, 0));
            break;
        }
        case 'Z':
        {
            auto camera = Scene::getCamera();
            camera->setPosition(camera->getPosition() + Cvec3(0, 0, 1));
            camera->updateView(Cvec3(0, 0, 0));
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

