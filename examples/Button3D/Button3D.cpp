#include <GLUT/glut.h>
#include "base/glsupport.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "entities/Entity.hpp"
#include "materials/Material.hpp"
#include "materials/Color.h"
#include "geometries/Sphere.h"
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "core/Light.hpp"
#include "programs/Shader.h"
#include "programs/ColorShader.h"

int screenWidth = 600;
int screenHeight = 600;

class ButtonListener : public ClickEventListener {
    double color = 0.5;
public:
    ButtonListener() {}
    void onClick(Entity& button) {
        button.setPosition(button.initState.transform.getPosition() + Cvec3(0, 0, -0.2));
        //change background color
        if (0.5 - color < 1e-8) {
            color = 0.1;
        } else {
            color = 0.5;
        }
        glClearColor(color, color, color, 1.0);
    }
    void onHover(Entity& button) {
        button.setPosition(button.initState.transform.getPosition());
        button.setScale(button.initState.transform.getScale() * 1.1);
        button.material->setColor(button.initState.color + Cvec3f(0.1, 0.1, 0.1));
    }
    void onIdle(Entity& button) {
        button.setPosition(button.initState.transform.getPosition());
        button.setScale(button.initState.transform.getScale());
        button.material->setColor(button.initState.color);
    }
};

void display(void) {
    Scene::render();
}

void init(void) {
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    
    std::string vertexShader = "shaders/vertex_shader_simple.glsl";
    std::string fragmentShader = "shaders/fragment_shader_color.glsl";

    /** Shader config **/
    auto colorShader = std::make_shared<ColorShader>();
    colorShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    /** Camera config **/
    auto camera = std::make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);

    /** Lights config **/
    auto light0 = std::make_shared<Light>();
    light0->setPosition(1, 5, -5);
    auto light1 = std::make_shared<Light>();
    light1->setPosition(-1, 0, -4);

    Scene::setLight0(light0);
    Scene::setLight1(light1);

    /** Button config **/
    auto sphere = std::make_shared<Sphere>(2, 40, 40);
    auto material = std::make_shared<Material>(Color::RED);
    auto button = std::make_shared<Entity>(sphere, material, "button");
    button->setPosition(Cvec3(0, 0, -5));
    button->setScale(Cvec3(0.2, 0.2, 0.2));
    button->registerClickEventListener(std::unique_ptr<ButtonListener>(new ButtonListener()));
    button->setShader(colorShader);
    Scene::addChild(button);

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
    glutCreateWindow("Interative Button");
    
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

