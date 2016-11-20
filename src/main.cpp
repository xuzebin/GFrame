/**
 * A small program that can look at a car from 4 different positions by pressing 'q' to switch different eye views on the keyboard.
 * A car hierarchy was built that has a car body as the parent, and 4 wheels as its sons.
 * A Baymax was made that has a head, eyes, body and limbs, which was also based on a proper hierarchy with over 3 objects.
 * Some randomly positioned boxes are generated.
 * Some randomly positioned and randomly colored spheres are generated.
 * A poster(plane) is placed at the end of the wall.
 * A skybox was created that contains all the scene. Note that the skybox here use a bit different Geometry than Cube, because it needs to flip normals so that we can see lighting inside skybox.
 *
 * Control the car using UP/DOWN keys for speed up and slow down(can go backward when < 0, but has a threshold),
 * LEFT/RIGHT keys for translating to the left or right handside (it is a bit weired now because I haven't added rotations, this will be done in the future).
 * When u speed up to a specific speed, u can just release the key and the car will keep moving at a constant speed that you last speeded up to.
 * 
 * What I have tried doing, but it's not used in this assignment.
 * 1. view the scene using an fps camera.
 * 2. pick objects in the scene using ray casting.
 *
 */

#include <glut.h>
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

const char* VERTEX_SHADER = "vertex_shader.glsl";
const char* FRAGMENT_SHADER = "fragment_shader.glsl";

ShaderProgram* shaderProgram;

Camera camera(Cvec3(0, 0, 0), Quat::makeXRotation(0));
//Scene scene;

//Entity* carBody = NULL;
std::vector<Entity*> wheels;
double speed = 0.0;

void moveCar(double speed);//forward declaration


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Entity* carBody = Scene::getEntity("carBody");
    if (carBody != NULL) {
        //simple way to bound the car inside the skybox
        if (carBody->getPosition()[2] <= -60 && carBody->getPosition()[2] >= -980) {
            moveCar(speed);
        }
    }
    
//    Scene::render(&camera, shaderProgram);
    Scene::render(shaderProgram);
    
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    //skybox is used and because it's hard to change the vertex order of the Cube geometry,
    //so I just disable it to make sure we can see inside the cube.
//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);


    Scene::setCamera(&camera);
    Light* light0 = new Light();
    light0->setPosition(0, 5, 45);
    Scene::setLight0(light0);
    shaderProgram = new ShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    
    /********************************** Baymax **********************************/
    
    //All components of Baymax sharing this sphere.
    Geometry* sphereGeo = new Sphere(2, 30, 30);
    
    //head
    Material* matHead = new Material();
    matHead->color = Cvec3f(1.0, 1.0, 1.0);
    Entity* head = new Entity(sphereGeo, matHead);
    head->setPosition(Cvec3(20, 20, -200));
    head->setRotation(Quat::makeYRotation(-30));
    head->setScale(Cvec3(1, 0.8, 1));

    //face
    Material* eyeMat = new Material();
    eyeMat->color = Cvec3f(0.0, 0.0, 0.0);
    Entity* leftEye = new Entity(sphereGeo, eyeMat);
    leftEye->setPosition(Cvec3(-0.7, 0, 2));
    leftEye->setScale(Cvec3(0.1, 0.1, 0.04));
    Entity* rightEye = new Entity(sphereGeo, eyeMat);
    rightEye->setPosition(Cvec3(0.7, 0, 2));
    rightEye->setScale(Cvec3(0.1, 0.1, 0.04));
    leftEye->parent = head; //build hierarchy
    rightEye->parent = head;
    
    //line
    Entity* line = new Entity(new Cube(2), eyeMat);
    line->setPosition(Cvec3(0, 0, 2));
    line->setScale(Cvec3(0.6, 0.02, 0.02));
    line->parent = head;//build hierarchy
    
    //body
    Material* bodyMat = new Material();
    bodyMat->color = Cvec3f(1.0, 1.0, 1.0);
    Entity* body = new Entity(sphereGeo, bodyMat);
    body->setPosition(Cvec3(0.0, -7, 0.0));
    body->setScale(Cvec3(2, 3, 1.5));
    body->parent = head; //build hierarchy
    
    //limbs
    Material* limbMat = new Material();
    limbMat->color = Cvec3f(1.0, 1.0, 1.0);
    Entity* leftArm = new Entity(sphereGeo, limbMat);
    Entity* rightArm = new Entity(sphereGeo, limbMat);
    Entity* leftLeg = new Entity(sphereGeo, limbMat);
    Entity* rightLeg = new Entity(sphereGeo, limbMat);
  
    leftArm->setPosition(Cvec3(-4.2, 0, 0));
    leftArm->setScale(Cvec3(1, 2, 1));
    rightArm->setPosition(Cvec3(4.2, 0, 0));
    rightArm->setScale(Cvec3(1, 2, 1));
    
    leftLeg->setPosition(Cvec3(-2, -7, 0));
    leftLeg->setScale(Cvec3(1, 1.5, 1));
    rightLeg->setPosition(Cvec3(2, -7, 0));
    rightLeg->setScale(Cvec3(1, 1.5, 1));

    //build hierarchy
    leftArm->parent = body;
    rightArm->parent = body;
    leftLeg->parent = body;
    rightLeg->parent = body;
    
    //add entites to the scene.
    Scene::addChild(head);
    Scene::addChild(leftEye);
    Scene::addChild(rightEye);
    Scene::addChild(line);
    Scene::addChild(body);
    Scene::addChild(leftArm);
    Scene::addChild(rightArm);
    Scene::addChild(leftLeg);
    Scene::addChild(rightLeg);
    
    
    /************************ skybox ************************/
    Geometry* boxGeo = new SkyBox(1000);
    Material* boxMat = new Material();
//    boxMat->diffuseTexture = loadGLTexture("wall.jpg");
    Entity* box = new Entity(boxGeo, boxMat);
    box->setPosition(Cvec3(0, 500, -500));
    
    Scene::addChild(box);

    /************************ poster ************************/
    Geometry* plane = new Plane(100);
    Material* m = new Material();
//    m->diffuseTexture = loadGLTexture("zxy.jpg");
    Entity* poster = new Entity(plane, m);
    poster->setPosition(Cvec3(0, 120, -995));
    poster->setRotation(Quat::makeXRotation(90));
    poster->setScale(Cvec3(2, 2, 2));

    Scene::addChild(poster);

    
    /******************* A car including a car body and 4 wheels *******************/
    
    Geometry* carBodyGeo = new Cube(8);
    Geometry* wheelGeo = new Sphere(4, 20, 20);
    
    Material* carBodyMat = new Material(Cvec3f(1, 0, 0));
    Material* wheelMat = new Material("chessboard.png");
    
    //car body is the parent of all other components
    Entity* carBody = new Entity("carBody", carBodyGeo, carBodyMat);
    
    //carBody is passed in as the parent of this 4 wheels
    Entity* leftFrontWheel = new Entity(wheelGeo, wheelMat, carBody);
    Entity* rightFrontWheel = new Entity(wheelGeo, wheelMat, carBody);
    Entity* leftRearWheel = new Entity(wheelGeo, wheelMat, carBody);
    Entity* rightRearWheel = new Entity(wheelGeo, wheelMat, carBody);
    
    carBody->setPosition(Cvec3(0, 8, -80));
    leftFrontWheel->setPosition((Cvec3(-4, -4, -6)));
    rightFrontWheel->setPosition((Cvec3(4, -4, -6)));
    leftRearWheel->setPosition((Cvec3(-4, -4, 6)));
    rightRearWheel->setPosition((Cvec3(4, -4, 6)));
    
    carBody->setScale(Cvec3(1, 0.8, 2.5));
    leftFrontWheel->setScale(Cvec3(0.5, 1, 1));
    rightFrontWheel->setScale(Cvec3(0.5, 1, 1));
    leftRearWheel->setScale(Cvec3(0.5, 1, 1));
    rightRearWheel->setScale(Cvec3(0.5, 1, 1));
    
    Scene::addChild(carBody);
    Scene::addChild(leftFrontWheel);
    Scene::addChild(rightFrontWheel);
    Scene::addChild(leftRearWheel);
    Scene::addChild(rightRearWheel);
    
    wheels.push_back(leftFrontWheel);
    wheels.push_back(rightFrontWheel);
    wheels.push_back(leftRearWheel);
    wheels.push_back(rightRearWheel);
    
    
    /******************* Randomly created boxes and spheres just to show the advantage of using Entity *******************/
    srand((unsigned)time(0));
    //The following generated entity shares the same Geometry,
    //thus they share vbo.
    Geometry* sp = new Sphere(10, 40, 40);
    for (int i = 0; i < 100; ++i) {
        int x = rand() % 800 - 400;
        int y = rand() % 400 + 15;
        int z = -rand() % 800 - 100;
        
        Material* spMat = new Material();
        spMat->color = Cvec3f(x % 256 / 255.0f, y % 256 / 255.0f, z % 256 / 255.0f);
        Entity* ball = new Entity(sp, spMat);
        ball->setPosition(Cvec3(x, y, z));
        Scene::addChild(ball);
    }

    Geometry* targetGeo = new Cube(10);
    Material* targetMat = new Material();
//    targetMat->diffuseTexture = loadGLTexture("box1.jpg");
    Material* targetMat2 = new Material();
//    targetMat2->diffuseTexture = loadGLTexture("box2.jpg");
    for (int i = 0; i < 200; ++i) {
        int x = rand() % 1000 - 500;
        int z = -rand() % 1000;
        
        Entity* ball;
        if (i % 2 == 0) {//we have two textures, so randomly bind one.
            ball = new Entity(targetGeo, targetMat);
        } else {
            ball = new Entity(targetGeo, targetMat2);
        }
        
        ball->setPosition(Cvec3(x, 5, z));
        Scene::addChild(ball);
    }
    
    
    Scene::removeAll();
//    Geometry* model = new Model("lucy.obj");
//    Geometry* model = new Model("Monk_Giveaway_Fixed.obj");
//    Material* modelMat = new Material();
//    std::cout << "diffuse tex path:" << ((Model*)model)->getDiffuseTexName().c_str() << std::endl;
//    modelMat->diffuseTexture = loadGLTexture(((Model*)model)->getDiffuseTexName().c_str());
//    Material* modelMat = new Material(Cvec3f(1, 0, 0));
//    Material* modelMat = new Material("Monk_D.tga");
//    Material* modelMat = new Material("chessboard.png");
//    modelMat->specularTexture = loadGLTexture("box2.jpg");
//    Entity* lucy = new Entity(model, modelMat);
//    lucy->setPosition(Cvec3(0, -3, -10));
//    lucy->setScale(Cvec3(30, 30, 30));
//    scene.addChild(lucy);
    
//    Model* model = new Model("cinema11.obj", "model");
//    model->setRotation(Quat::makeYRotation(180));
//    model->setPosition(Cvec3(0, -2.7, -2.8));
//    model->setScale(Cvec3(0.05, 0.05, 0.05));
    
//    Geometry* ge = new Sphere(4, 60, 60);
//    Geometry* ge = new Cube(6);
//    Material* ma = new Material(Cvec3f(1.0, 0, 0));
//    Entity* entity = new Entity("model", ge, ma);
//    entity->setPosition(Cvec3(0, -2, -20));
//    entity->setRotation(Quat::makeYRotation(30));
//    scene.addChild(entity);
    
//    Model* model = new Model("lucy.obj", "model");
//    model->setScale(Cvec3(30, 30, 30));
//    model->setPosition(Cvec3(0, -3, -10));
    
    
//    Sphere* p = new Sphere(2, 40, 40, true);
//    Material* mp = new Material("andes.jpg");
//    Entity* pano = new Entity("panorama", p, mp);
//    pano->setPosition(Cvec3(0, 0, -20));

    
    Model* model = new Model("Monk_Giveaway_Fixed.obj", "model");
//    model->setScale(Cvec3(0.03, 0.03, 0.03));
    model->setScale(Cvec3(0.36, 0.36, 0.36));
    model->setPosition(Cvec3(-1.5, -2.4, -7));
    model->setRotation(Quat::makeYRotation(40));
    Scene::addChild(model);
//
//
//    model->setPosition(Cvec3(0, -10, -30));
//    Mesh* mesh = new Mesh("Spiderman.obj");
//    Material* material = new Material();
//    Model* model0 = new Model(mesh, material, "model0");
//    model0->setPosition(Cvec3(0, -2, -8));
//    scene.addChild(model0);
//
//    Model* model1 = new Model(mesh, material, "model1");
//    model1->setPosition(Cvec3(1, -2, -8));
//    scene.addChild(model1);
    
    
    
    Model* model2 = new Model("Spiderman.obj", "model2");
    model2->setScale(Cvec3(1.4, 1.4, 1.4));
    model2->setPosition(Cvec3(1, -2.4, -7));
    model2->setRotation(Quat::makeYRotation(-40));
    Scene::addChild(model2);
//
//    
//    Model* model3 = new Model("Spiderman.obj", "model3");
//    model3->setScale(Cvec3(1.4, 1.4, 1.4));
//    model3->setPosition(Cvec3(0, -2.4, -7));
//    model3->setRotation(Quat::makeYRotation(0));
//    scene.addChild(model3);
    
//    srand((unsigned)time(0));
//    for (int i = 0; i < 10; ++i) {
//        Model* model = new Model("Spiderman.obj", "model4");
////        model3->setScale(Cvec3(1.4, 1.4, 1.4));
//        
//        model->setScale(Cvec3(0.5, 0.5, 0.5));
//        int x = rand() % 5 - 2;
//        int y = rand() % 5 - 2;
//        int z = -rand() % 5;
//        
//        model3->setPosition(Cvec3((float)x/10.0, (float)y/10.0, (float)z/10.0));
//        model3->setRotation(Quat::makeYRotation(rand() % 360));
//        scene.addChild(model);
//    }
//    camera.setPosition(Cvec3(0, 10, -10));
//    camera.rotate(Quat::makeXRotation(45));
    
//    Model* model = new Model("Farmhouse.obj", "model");
//    model->setScale(Cvec3(0.1, 0.1, 0.1));
//    model->setPosition(Cvec3(0, -2, -9));
//    model->setRotation(Quat::makeYRotation(45));
    
    
//    scene.addChild(model);
//    scene.addChild(pano);
    
//    model->setScale(Cvec3(0.1, 0.1, 0.1));
//    model->setPosition(Cvec3(0, -8, -28));
    
//    Model* model = new Model("face.obj");
//    model->setScale(Cvec3(5, 5, 5));
//    model->setPosition(Cvec3(0, -3, -10));
    
//    SkyBox* envGeo = new SkyBox(20);
////    Cube* envGeo = new Cube(5);
//    Material* envMat = new Material();
//    Cubemap cubemap;
////    cubemap.load("cubemap/posx.jpg", "cubemap/negx.jpg",
////                 "cubemap/posy.jpg", "cubemap/negy.jpg",
////                 "cubemap/posz.jpg", "cubemap/negz.jpg");
//    cubemap.load("chessboard.png", "chessboard.png",
//                 "chessboard.png", "chessboard.png",
//                 "chessboard.png", "chessboard.png");
//    envMat->setCubemap(cubemap);
//    Entity* skybox = new Entity(envGeo, envMat);
////    skybox->setPosition(Cvec3(0, -4, -20));
//    skybox->setRotation(Quat::makeYRotation(45));
//    scene.addChild(skybox);

    Scene::createMeshes();//this call genereate vbo/ibo for the geometry of each Entity.
    
    
    


}
//bool pressed = false;
//int lastx, lasty;
//void mouse(int button, int state, int x, int y) {
//    camera.mousePressed(state, x, y);
//
//    arcball
//    if (state == GLUT_LEFT_BUTTON) {
//        lastx = x;
//        lasty = windowHeight - y;
//        pressed = true;
//    } else {
//        pressed = false;
//    }
//
//}

//TODO interpolation
float lerp(float v0, float v1, float t) {
    return (1.0 - t) * v0 + t * v1;
}

//TODO implement catmull-rom

//move car along x axis.
void moveCar(double speed, double carHorizontalMove) {
    
    Entity* carBody = Scene::getEntity("carBody");
    
    if (carBody == NULL) {
        return;
    }
    
    Cvec3 position = carBody->getPosition();
    if (position[2] > -60 || position[2] < -980) {
        position[2] = position[2] > -60 ? -60 : -980;
        carBody->setPosition(position);
        return;
    }
    //std::cout << "moving... car position: " << position[2] << std::endl;
    carBody->translate(Cvec3(carHorizontalMove, 0, -speed));
    Quat rotation = Quat::makeXRotation(-speed);
    for(std::vector<Entity*>::iterator it = wheels.begin(); it != wheels.end(); ++it) {
        (*it)->rotate(rotation);
    }
    camera.updateView(carBody->getPosition());
}

//move car along z axis.
void moveCar(double speed) {
    
    Entity* carBody = Scene::getEntity("carBody");
    if (carBody == NULL) {
        return;
    }
    Cvec3 position = carBody->getPosition();
    if (position[2] > -60 || position[2] < -980) {
        position[2] = position[2] > -60 ? -60 : -980;
        carBody->setPosition(position);
        return;
    }
    //std::cout << "moving... car position: " << position[2] << std::endl;
    carBody->translate(Cvec3(0, 0, -speed));
    Quat rotation = Quat::makeXRotation(-5 * speed);
    for(std::vector<Entity*>::iterator it = wheels.begin(); it != wheels.end(); ++it) {
        (*it)->rotate(rotation);
    }
    camera.updateView(carBody->getPosition());
}

//control car
void specialInput(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_RIGHT:
            moveCar(speed, 2);
            break;
        case GLUT_KEY_DOWN:
            speed = speed > -2.0 ? speed - 0.1 : -2.0;
            moveCar(speed);
            break;
        case GLUT_KEY_LEFT:
            moveCar(speed, -2);
            break;
        case GLUT_KEY_UP:
            speed = speed < 2.0 ? speed + 0.1 : 2.0;
            moveCar(speed);
            break;
        default:
            break;
    }
}

//control camera to view from 4 different places.
void keyboard(unsigned char key, int x, int y) {
    camera.switchMode(key);
    
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
            Entity* model = Scene::getEntity("model");
            model->setScale(model->getScale() * 2);
            camera.updateView(model->getPosition());
            break;
        }
        case 'd':
        case 'D':
        {
            Entity* model = Scene::getEntity("model");
            model->setScale(model->getScale() / 2);
            camera.updateView(model->getPosition());
            break;
        }
        case 'G':
        case 'g':
        {
            Light* light0 = Scene::getLight0();
            light0->moveY(1);
            break;
        }
        case 'b':
        case 'B':
        {
            Light* light0 = Scene::getLight0();
            light0->moveY(-1);
            break;
        }
        case 'v':
        case 'V':
        {
            Light* light0 = Scene::getLight0();
            light0->moveX(-1);
            break;
        }
        case 'n':
        case 'N':
        {
            Light* light0 = Scene::getLight0();
            light0->moveX(1);
            break;
        }
        case 'f':
        case 'F':
        {
            Light* light0 = Scene::getLight0();
            light0->moveZ(1);
            Cvec3 pos = light0->getPosition();
            std::cout << pos[0] << "," << pos[1] << "," << pos[2] << std::endl;
            break;
        }
        case 'h':
        case 'H':
        {
            Light* light0 = Scene::getLight0();
            light0->moveZ(-1);
            break;
        }
        default:
            break;
    }
}


Cvec2 getScreenSpaceCoordFromEyeSpace(const Cvec3& eyeSpaceCoord, const Matrix4& projection, int screenWidth, int screenHeight) {
    if (eyeSpaceCoord[2] > -1e-8) {
        throw string("z > 0");
    }
    Cvec4 p = projection * Cvec4(eyeSpaceCoord, 1);
    Cvec3 clippedCoord = Cvec3(p[0], p[1], p[2]) / p[3];
    return Cvec2(clippedCoord[0] * screenWidth / 2.0 + (screenWidth - 1) / 2.0, clippedCoord[1] * screenHeight / 2.0 + (screenHeight - 1) / 2.0);
}

Cvec2 getScreenSpaceCoordFromWorldSpace(const Matrix4& modelMatrix, const Matrix4& projection, int screenWidth, int screenHeight) {
    Cvec4 eyeSpaceCoord = inv(camera.getViewMatrix()) * modelMatrix * Cvec4(0, 0, 0, 1);
    return getScreenSpaceCoordFromEyeSpace(Cvec3(eyeSpaceCoord), projection, screenWidth, screenHeight);
}

double getScreenToEyeScale(double z, double fovy, int screenHeight) {
    if (z > -1e-8) {
        throw string("z > 0");
    }
    return -(z * tan(fovy * CS175_PI / 360.0)) * 2 / screenHeight;
}

Cvec3 getPickedPointOnSphereInScreenSpace(Entity* model, int x, int y) {
    Cvec2 centerInScreenSpace = getScreenSpaceCoordFromWorldSpace(model->getModelMatrix(), camera.getProjectionMatrix(), screenWidth, screenHeight);
    std::cout << "screenPos: " << centerInScreenSpace[0] << "," << centerInScreenSpace[1] << std::endl;
    
    double r = screenWidth / 2;//30;//let the radius of the model is 4.
    double cr = r / getScreenToEyeScale(model->getPosition()[2], camera.getFov(), screenHeight);//here assume RBT in eye space.
//    double cr = r;
    
    std::cout << "model center in screen: " << centerInScreenSpace[0] << "," << centerInScreenSpace[1] << std::endl;
    std::cout << "clicked at: " << x << "," << y << std::endl;

    double x_cx = x - centerInScreenSpace[0];
    double y_cy = y - centerInScreenSpace[1];
    std::cout << "offset: " << x_cx << "," << y_cy << std::endl;
    std::cout << "scaled radius: " << cr << std::endl;
    
    double pickedZ2 = cr * cr - x_cx * x_cx - y_cy * y_cy;
    pickedZ2 = pickedZ2 < 0 ? 0 : pickedZ2;
    double pickedZ = sqrt(pickedZ2);
    std::cout << "picked z: " << pickedZ << std::endl;
    return Cvec3(x, y, pickedZ);
}

//Cvec3 getArcballVector(int x, int y) {
//    //normalize
//    Cvec3 np((float)x / ((float)screenWidth / 2.0f) - 1.0f, 1.0f - (float)y / ((float)screenHeight / 2.0f), 0);
//
//    float sqrtV1 = np[0] * np[0] + np[1] * np[1];
//    if (sqrtV1 <= 1) {
//        np[2] = sqrt(1 - sqrtV1);
//    }
//    np.normalize();
//    return np;
//}


Cvec3 prev_pos;
Cvec3 cur_pos;
bool rotating = false;

Cvec2 prev_p;
Cvec2 curr_p;
void mouse(int button, int state, int x, int y) {
    Entity* model = Scene::getEntity("model");
    if (model == NULL) {
        std::cerr << "model not found in the scene" << std::endl;
        return;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        std::cout << "down" << std::endl;
        cur_pos = getPickedPointOnSphereInScreenSpace(model, x, screenHeight - y);
        prev_pos = cur_pos;
        rotating = true;
//        
//        prev_p = Cvec2(x, y);
//        curr_p = prev_p;
    } else {
        rotating = false;
    }
    
}

void motion(int x, int y) {
    if (!rotating) {
        return;
    }
    
//    curr_p = Cvec2(x, y);
    
    
    
    std::cout << "moving: " << x << "," << y << std::endl;
    Entity* model = Scene::getEntity("model");
    if (model == NULL) {
        std::cerr << "model not found in the scene" << std::endl;
        return;
    }

    cur_pos = getPickedPointOnSphereInScreenSpace(model, x, screenHeight - y);
    
    std::cout << "prev pos: [" << prev_pos[0] << "," << prev_pos[1] << "," << prev_pos[2] << "]" << "cur pos: [" << cur_pos[0] << "," << cur_pos[1] <<
    "," << cur_pos[2] << "]" << std::endl;
    
    Cvec2 center = getScreenSpaceCoordFromWorldSpace(model->getModelMatrix(), camera.getProjectionMatrix(), screenWidth, screenHeight);
    Cvec3 c(center, 0);
    
    Cvec3 v1 = normalize(prev_pos - c);
    Cvec3 v2 = normalize(cur_pos - c);
    
    Quat q1(0, v2);
    Quat q2(0, -v1);
    
    Quat q = q1 * q2;
//    Quat q(dot(v1, v2), cross(v1, v2));
    std::cout << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << std::endl;
    model->rotate(q);

}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;
}

void idle(void) {
    glutPostRedisplay();
    
//    Entity* model = scene.getEntity("model");
//    if (model == NULL) {
//        std::cerr << "model not found in the scene" << std::endl;
//        return;
//    }
//
//    if (curr_p[0] != prev_p[0] || curr_p[1] != prev_p[1]) {
//        Cvec3 v1 = getArcballVector(prev_p[0], prev_p[1]);
//        Cvec3 v2 = getArcballVector(curr_p[0], curr_p[1]);
//        double angle = acos(min(1.0, dot(v1, v2)));
//        Cvec3 axis = cross(v1, v2);
//        Matrix4 eyeToObj = inv(camera.getViewMatrix()) * model->getModelMatrix();
//        Cvec3 axisInObjSpace = eyeToObj * Cvec4(axis, 0);
////        Cvec3 axisInObjectSpace = eyeToObj * Cvec4(axis, 0);
//        
//        
//        prev_p = curr_p;
//    }
    
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("CS-6533");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutSpecialFunc(specialInput);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    init();
    glutMainLoop();
    return 0;
}

