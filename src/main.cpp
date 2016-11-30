#include <glut.h>
#include <queue>
#include "glsupport.h"
#include "geometrymaker.h"
#include "matrix4.h"
#include "quat.h"
#include "Geometry.h"
#include "Entity.h"
#include "Transform.h"
#include "Material.h"
#include "Camera.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Util.h"
#include "Cube.h"
#include "Plane.h"
#include "Skybox.h"
#include "Model.h"
#include "Sphere.h"
#include "Light.h"


int screenWidth = 500;
int screenHeight = 500;

ShaderProgram* shaderProgram;

Camera camera(Cvec3(0, 0, 0), Quat::makeXRotation(0));

std::string currentModel = "model0";

Light* currentMovingLight = Scene::getLight(0);



class BtnEventListener : public ClickEventListener {

public:
    BtnEventListener() {}
    void onClick(Entity* button) {
        button->setPosition(button->initState.transform.getPosition() + Cvec3(0, 0, -0.2));
    }
    void onHover(Entity* button) {
        button->setPosition(button->initState.transform.getPosition());
        button->setScale(button->initState.transform.getScale() * 1.1);
        button->material->color = button->initState.color + Cvec3f(0.2, 0, 0);
    }
    void onIdle(Entity* button) {
        button->setPosition(button->initState.transform.getPosition());
        button->setScale(button->initState.transform.getScale());
        button->material->color = button->initState.color;
    }
};

class ModelSwitchBtnEventListener : public BtnEventListener {
private:
    int modelSelected = 0;
public:
    void onClick(Entity* button) {
        BtnEventListener::onClick(button);
        modelSelected = (modelSelected + 1) % 2;
        switch(modelSelected) {
            case 0:
            {
                Entity* monk = Scene::getEntity("model0");
                Entity* spiderman = Scene::getEntity("model1");
                monk->setVisible(true);
                spiderman->setVisible(false);
                currentModel = "model0";
                break;
            }
            case 1:
            {
                Entity* monk = Scene::getEntity("model0");
                Entity* spiderman = Scene::getEntity("model1");
                monk->setVisible(false);
                spiderman->setVisible(true);
                currentModel = "model1";
                break;
            }
            default:
                throw std::string("no matched model selected");
        }
    }
    void onHover(Entity* button) {
        BtnEventListener::onHover(button);
    }
    void onIdle(Entity* button) {
        BtnEventListener::onIdle(button);
    }
};

class LightColorBtnEventListener : public BtnEventListener {
private:
    int lightColor0 = 0;
    int lightColor1 = 0;
public:
    void onClick(Entity* button) {
        BtnEventListener::onClick(button);
        
        if (button->getName() == "button1") {
            lightColor0 = (lightColor0 + 1) % 2;
            if (lightColor0 == 0) {
                currentMovingLight = Scene::getLight(0);
                Scene::light0->lightColor = Cvec3f(1, 1, 1);
                button->initState.color = Cvec3f(0.8, 0, 0);
            } else {
                currentMovingLight = NULL;
                Scene::light0->lightColor = Cvec3f(0, 0, 0);
                button->initState.color = Cvec3f(0.1, 0, 0);
            }
        } else {
            lightColor1 = (lightColor1 + 1) % 2;
            if (lightColor1 == 0) {
                currentMovingLight = Scene::getLight(1);
                Scene::light1->lightColor = Cvec3f(1, 1, 1);
                button->initState.color = Cvec3f(0.8, 0, 0);
            } else {
                currentMovingLight = NULL;
                Scene::light1->lightColor = Cvec3f(0, 0, 0);
                button->initState.color = Cvec3f(0.1, 0, 0);
            }
        }
        
    }
    void onHover(Entity* button) {
        BtnEventListener::onHover(button);
    }
    void onIdle(Entity* button) {
        BtnEventListener::onIdle(button);
    }
};

class SpecularLightColorBtnEventListener : public BtnEventListener {
private:
    bool spcularLightColorOn0 = false;
    bool spcularLightColorOn1 = false;
public:
    void onClick(Entity* button) {
        BtnEventListener::onClick(button);
        
        if (button->getName() == "button3") {
            spcularLightColorOn0 = !spcularLightColorOn0;
            if(spcularLightColorOn0) {
                Scene::light0->specularLightColor = Cvec3f(0, 0, 0);
                button->material->color = Cvec3f(0.1, 0, 0);
                button->initState.color = Cvec3f(0.1, 0, 0);
            } else {
                Scene::light0->specularLightColor = Cvec3f(1, 1, 1);
                button->material->color = Cvec3f(0.6, 0, 0);
                button->initState.color = Cvec3f(0.6, 0, 0);
            }
        } else {
            spcularLightColorOn1 = !spcularLightColorOn1;
            if(spcularLightColorOn1) {
                Scene::light1->specularLightColor = Cvec3f(0, 0, 0);
                button->material->color = Cvec3f(0.1, 0, 0);
                button->initState.color = Cvec3f(0.1, 0, 0);
            } else {
                Scene::light1->specularLightColor = Cvec3f(1, 1, 1);
                button->material->color = Cvec3f(0.6, 0, 0);
                button->initState.color = Cvec3f(0.6, 0, 0);
            }
        }
    }
    void onHover(Entity* button) {
        BtnEventListener::onHover(button);
    }
    void onIdle(Entity* button) {
        BtnEventListener::onIdle(button);
    }
};



void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Scene::render(shaderProgram);
    
    glutSwapBuffers();
}

void init() {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    
    shaderProgram = new ShaderProgram();
    shaderProgram->createProgram("vertex_shader.glsl", "fragment_shader.glsl");
    glUseProgram(shaderProgram->programId);

    Scene::setCamera(&camera);
    Light* light0 = new Light();
    light0->setPosition(1, 5, -5);//1,5,-5
    Light* light1 = new Light();
    light1->setPosition(-1, 0, -4);//0,0,-6  , 1,5,0
    Scene::setLight0(light0);
    Scene::setLight1(light1);

    
    Model* model0 = new Model("Monk_Giveaway_Fixed.obj", "model0");
    model0->setScale(Cvec3(0.5, 0.5, 0.5));
    model0->setPosition(Cvec3(0, -3.4, -10));
    model0->setRotation(Quat::makeYRotation(20));
    Scene::addChild(model0);
//
//    //TODO add different shaders
//    Model* model1 = new Model("Spiderman.obj", "model1");
//    model1->setScale(Cvec3(1.3, 1.3, 1.3));
//    model1->setPosition(Cvec3(0, -2.3, -7));
//    model1->setRotation(Quat::makeYRotation(20));
//    model1->setVisible(false);
//    Scene::addChild(model1);
//    
//    
//    /************ model swtich button ************/
//    Geometry* buttonG = new Sphere(2, 40, 40);
//    Material* buttonM = new Material(Cvec3f(0.6, 0.0, 0.0));
//    Entity* btn0 = new Entity("button0", buttonG, buttonM);
//    btn0->setPosition(Cvec3(-1.8, 1.9, -5));
//    btn0->setScale(Cvec3(0.05, 0.05, 0.05));
//    btn0->registerClickEventListener(new ModelSwitchBtnEventListener());
//    Scene::addChild(btn0);
//    
//    /************ light color buttons ************/
//    for (int i = 0; i < 2; ++i) {
//        Material* buttonM = new Material(Cvec3f(0.6, 0.0, 0.0));
//        Entity* btn = new Entity("button" + std::to_string(i + 1), buttonG, buttonM);
//        btn->setPosition(Cvec3(-1.8, 1.9 - (i + 1) / 3.0, -5));
//        btn->setScale(Cvec3(0.05, 0.05, 0.05));
//        btn->registerClickEventListener(new LightColorBtnEventListener());
//        Scene::addChild(btn);
//    }
//    
//    /************ specular light color buttons ************/
//    for (int i = 0; i < 2; ++i) {
//        Material* buttonM = new Material(Cvec3f(0.6, 0.0, 0.0));
//        Entity* btn = new Entity("button" + std::to_string(i + 3), buttonG, buttonM);
//        btn->setPosition(Cvec3(-1.5, 1.9 - (i + 1) / 3.0, -5));
//        btn->setScale(Cvec3(0.05, 0.05, 0.05));
//        btn->registerClickEventListener(new SpecularLightColorBtnEventListener());
//        Scene::addChild(btn);
//    }
//    
    /************** ground ****************/
//    Entity* ground = new Entity("ground", new Plane(8), new Material("rock.jpg"));
//    ground->setPosition(Cvec3(0, -3.5, -12));
//    Scene::addChild(ground);
    
    
//    Entity* screen = new Entity("screen", new Plane(2), new Material());
    


    Scene::createMeshes();//this call genereate vbo/ibo for the geometry of each Entity.
}



//control light position in x and z coordinates
void specialInput(int key, int x, int y) {
    if (currentMovingLight == NULL) {
        return;
    }
    switch (key) {
        case GLUT_KEY_RIGHT:
        {
            currentMovingLight->moveX(1);
            break;
        }
        case GLUT_KEY_DOWN:
        {
            currentMovingLight->moveZ(1);
            break;
        }
        case GLUT_KEY_LEFT:
        {
            currentMovingLight->moveX(-1);
            break;
        }
        case GLUT_KEY_UP:
        {
            currentMovingLight->moveZ(-1);
            break;
        }
        default:
            break;
    }
    std::cout << "light pos: " << currentMovingLight->getPosition()[0] << "," << currentMovingLight->getPosition()[1] << "," << currentMovingLight->getPosition()[2]<< std::endl;
}


void keyboard(unsigned char key, int x, int y) {

    switch(key) {
        case 'l':
        case 'L':
            camera.rotate(Quat::makeYRotation(10));
            break;
        case 'k':
        case 'K':
            camera.rotate(Quat::makeYRotation(-10));
            break;
        case 's':
        case 'S':
        {
            Entity* model = Scene::getEntity(currentModel);
            model->setScale(model->getScale() * 1.1);
            break;
        }
        case 'd':
        case 'D':
        {
            Entity* model = Scene::getEntity(currentModel);
            model->setScale(model->getScale() / 1.1);
            break;
        }
        case 'G':
        case 'g':
        {
            if (currentMovingLight != NULL) {
                currentMovingLight->moveY(1);
            }
            break;
        }
        case 'b':
        case 'B':
        {
            if (currentMovingLight != NULL) {
                currentMovingLight->moveY(-1);
            }
            break;
        }
        default:
            break;
    }
}


Cvec2 getScreenSpaceCoordFromEyeSpace(const Cvec3& eyeSpaceCoord, const Matrix4& projection, int screenW, int screenH) {
    if (eyeSpaceCoord[2] > -1e-8) {
        throw string("z > 0");
    }
    Cvec4 p = projection * Cvec4(eyeSpaceCoord, 1);
    Cvec3 clippedCoord = Cvec3(p[0], p[1], p[2]) / p[3];
    return Cvec2(clippedCoord[0] * screenW / 2.0 + (screenW - 1) / 2.0, clippedCoord[1] * screenH / 2.0 + (screenH - 1) / 2.0);
}

Cvec2 getScreenSpaceCoordFromWorldSpace(const Matrix4& modelMatrix, const Matrix4& projection, int screenW, int screenH) {
    Cvec4 eyeSpaceCoord = inv(camera.getViewMatrix()) * modelMatrix * Cvec4(0, 0, 0, 1);
    return getScreenSpaceCoordFromEyeSpace(Cvec3(eyeSpaceCoord), projection, screenW, screenH);
}

double getScreenToEyeScale(double z, double fovy, int screenHeight) {
    if (z > -1e-8) {
        throw string("z > 0");
    }
    return -(z * tan(fovy * CS175_PI / 360.0)) * 2 / screenHeight;
}

Cvec3 getPickedPointOnSphereInScreenSpace(Entity* entity, int x, int y) {
    Cvec2 centerInScreenSpace = getScreenSpaceCoordFromWorldSpace(entity->getModelMatrix(), camera.getProjectionMatrix(), screenWidth, screenHeight);
    
    double r = screenWidth / 2;
    double cr = r / getScreenToEyeScale(entity->getPosition()[2], camera.getFov(), screenHeight);//here assume RBT in eye space.

    double x_cx = x - centerInScreenSpace[0];
    double y_cy = y - centerInScreenSpace[1];
    
    double pickedZ2 = cr * cr - x_cx * x_cx - y_cy * y_cy;
    pickedZ2 = pickedZ2 < 0 ? 0 : pickedZ2;
    double pickedZ = sqrt(pickedZ2);

    return Cvec3(x, y, pickedZ);
}

Cvec3 prev_pos;
Cvec3 cur_pos;
bool rotating = false;
void mouse(int button, int state, int x, int y) {
    Scene::updateMouseEvent(button, state, x, y, screenWidth, screenHeight);
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Entity* model = Scene::getEntity(currentModel);
        if (model == NULL) {
            std::cerr << "model not found in the scene" << std::endl;
            return;
        }
        
        cur_pos = getPickedPointOnSphereInScreenSpace(model, x, screenHeight - y);
        prev_pos = cur_pos;
        rotating = true;
    } else {
        rotating = false;
    }
    
}

void motion(int x, int y) {
    if (!rotating) {
        return;
    }

    Entity* model = Scene::getEntity(currentModel);
    if (model == NULL) {
        std::cerr << "model not found in the scene" << std::endl;
        return;
    }

    cur_pos = getPickedPointOnSphereInScreenSpace(model, x, screenHeight - y);
    
    Cvec2 center = getScreenSpaceCoordFromWorldSpace(model->getModelMatrix(), camera.getProjectionMatrix(), screenWidth, screenHeight);
    Cvec3 c(center, 0);
    
    Cvec3 v1 = normalize(prev_pos - c);
    Cvec3 v2 = normalize(cur_pos - c);
    
    Quat q1(0, v2);
    Quat q2(0, -v1);
    
    Quat q = q1 * q2;
//    Quat q(dot(v1, v2), cross(v1, v2));
    model->rotate(q);
}

bool insideWindow(int x, int y) {
    return (x >= 0 && y >= 0 && x <= screenWidth && y <= screenHeight);
}
void passiveMotion(int x, int y) {
    if (insideWindow(x, y)) {
        Scene::updateMousePassiveMotion(x, y, screenWidth, screenHeight);
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;
}

void idle(void) {
    glutPostRedisplay();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);// | GLUT_MULTISAMPLE);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("CS-6533");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutSpecialFunc(specialInput);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    
//    glEnable(GLUT_MULTISAMPLE);
    
    init();
    glutMainLoop();
    return 0;
}

