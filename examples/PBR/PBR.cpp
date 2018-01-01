/**
 * Physically based rendering.
 * Pressing 'p' will switch to control the rusted sphere. 
 * Pressing space + 'p' will switch to control the 7x7 spheres.
 * Drag and scroll to change their orientations and positions.
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "base/glsupport.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "materials/Material.hpp"
#include "materials/Color.h"
#include "core/Scene.hpp"
#include "core/Camera.hpp"
#include "entities/Model.hpp"
#include "core/Light.hpp"
#include "programs/ColorShader.h"
#include "controls/Trackball.hpp"
#include "geometries/Cube.h"
#include "geometries/Sphere.h"
#include "geometries/Plane.h"
#include "PBRShader.h"
#include "PBRTextureShader.h"

#include <stdio.h>

class Shader;

using namespace std;

int screenWidth = 600;
int screenHeight = 600;

Trackball trackball(screenWidth, screenHeight);
float lineWidth = 0.5f;
bool wireframe = true;
bool solid = false;

char* modelPath = NULL;

std::shared_ptr<PBRShader> pbrShader(nullptr);

std::vector<std::shared_ptr<PBRShader> > shaderList;
std::shared_ptr<PBRTextureShader> pbrTexShader;

std::string vertexShader = "./examples/PBR/pbr_vs.glsl";
std::string fragmentShader = "./examples/PBR/pbr_fs.glsl";
std::string fragmentTexShader = "./examples/PBR/pbr_texture_fs.glsl";

std::shared_ptr<Entity> controledObj;

void controls(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_A) {
        lineWidth += 0.5f;
    } else if (key == GLFW_KEY_S) {
        lineWidth -= 0.5f;
    }

    if (key == GLFW_KEY_Z) {
        solid = true;
        wireframe = true;
    } else if (key == GLFW_KEY_X) {
        solid = true;
        wireframe = false;
    } else if (key == GLFW_KEY_C) {
        solid = false;
        wireframe = true;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        //enable/disable gamma correction
        int enable = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ? 1 : 0;

        if (controledObj->getName() == "rusted_sphere") {//rusted sphere
            pbrTexShader->gammaCorrect = enable;
        } else {
            for (int i = 0; i < shaderList.size(); i++) {
                shaderList[i]->gammaCorrect = enable;
            }
        }
    } else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        //open/disable HDR
        int enable = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ? 1 : 0;
        if (controledObj->getName() == "rusted_sphere") {//rusted sphere
            pbrTexShader->hdr = enable;
        } else {
            for (int i = 0; i < shaderList.size(); i++) {
                shaderList[i]->hdr = enable;
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        //switch controled object
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            controledObj = Scene::getEntity("plane");
        } else {
            controledObj = Scene::getEntity("rusted_sphere");
        }
    }

//     auto camera = Scene::getCamera();
//     auto model = Scene::getEntity("plane");
//     if (key == GLFW_KEY_UP) {
//         camera->setPosition(camera->getPosition() + Cvec3(0, 1, 0));
//         camera->updateView(model->getPosition());
//     } else if (key == GLFW_KEY_DOWN) {
//         camera->setPosition(camera->getPosition() + Cvec3(0, -1, 0));
//         camera->updateView(model->getPosition());
//     } else if (key == GLFW_KEY_LEFT) {
//         camera->setPosition(camera->getPosition() + Cvec3(-1, 0, 0));
//         camera->updateView(model->getPosition());
//     } else if (key == GLFW_KEY_RIGHT) {
//         camera->setPosition(camera->getPosition() + Cvec3(1, 0, 0));
//         camera->updateView(model->getPosition());
//     }

    //not used
//     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//         if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
//             // increase ao
//             pbrShader->ao += 0.1;
//             pbrShader->ao = pbrShader->ao > 1.0 ? 1.0 : pbrShader->ao;
//             cout << "ao: " << pbrShader->ao << endl;
//         } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
//             // increase roughness
//             pbrShader->roughness += 0.1;
//             pbrShader->roughness = pbrShader->roughness > 1.0 ? 1.0 : pbrShader->roughness;
//             cout << "roughness: " << pbrShader->roughness << endl;
//         } else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
//             // increase metallic
//             pbrShader->metallic += 0.1;
//             pbrShader->metallic = pbrShader->metallic > 1.0 ? 1.0 : pbrShader->metallic;
//             cout << "metallic: " << pbrShader->metallic << endl;
//         }
//     } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//         if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
//             // decrease ao
//             pbrShader->ao -= 0.1;
//             pbrShader->ao = pbrShader->ao < 0.0 ? 0.0 : pbrShader->ao;
//         } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
//             // decrease roughness
//             pbrShader->roughness -= 0.1;
//             pbrShader->roughness = pbrShader->roughness < 0.0 ? 0.0 : pbrShader->roughness;
//             cout << "roughness: " << pbrShader->roughness << endl;
//         } else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
//             // decrease metallic
//             pbrShader->metallic -= 0.1;
//             pbrShader->metallic = pbrShader->metallic < 0.0 ? 0.0 : pbrShader->metallic;
//             cout << "metallic: " << pbrShader->metallic << endl;
//         }
//     }

}
bool mouseLeftDown = false;
double mousex = 0.0, mousey = 0.0;
void mouse(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            trackball.record(mousex, mousey);
            mouseLeftDown = true;
        } else if (action == GLFW_RELEASE) {
            mouseLeftDown = false;
        }
    }
}

void curson_pos(GLFWwindow* window, double xpos, double ypos) {
    mousex = xpos;
    mousey = ypos;
    if (mouseLeftDown) {
        auto model = controledObj;
        if (model != NULL) {
            Quat rotation = trackball.getRotation(xpos, ypos);
            model->setRotation(rotation);
            std::cout << "position: " << model->getPosition() << std::endl;
            std::cout << "rotation: " << model->getRotation() << std::endl;
        }
    }
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
    auto model = controledObj;
    model->translate(Cvec3(0, 0, yoffset));
}

void display(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glLineWidth(lineWidth);
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth*2, windowHeight*2);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 49; i++) {
            auto model = Scene::getEntity("model" + std::to_string(i));
            model->draw(Scene::getCamera(), model->getShader(), Scene::getLight(0), Scene::getLight(1));
        }

        auto model = Scene::getEntity("rusted_sphere");
        model->draw(Scene::getCamera(), model->getShader(), Scene::getLight(0), Scene::getLight(1));

        //update screen
        glfwSwapBuffers(window);
        //Check for any input, or window movement
        glfwPollEvents();
    }
}

GLuint loadTexture(const char *filePath, int textureUnit) {
    int  w,h,comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);

    if(image == nullptr) {
        std::cout << "Unable to load image. Make sure the image is in the same path as the executable.\n";
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return retTexture;
}


void setUpModels() {
    auto plane = std::make_shared<Entity>(std::make_shared<Plane>(10), std::make_shared<Material>(Cvec3f(0.5, 0.0, 0.0)), "plane");
    plane->setPosition(Cvec3(0,0, -12));
    plane->setShader(pbrShader);
    Scene::addChild(plane);
    trackball.setInitRotation(plane->getRotation());
    controledObj = plane;//set controled object
    
    int sphereNum = 49;//7x7
    auto material = std::make_shared<Material>(Cvec3f(0.5, 0.0, 0.0));
    auto geometry = std::make_shared<Sphere>(1,50,50);
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            cout << "setting up " << "model" + std::to_string(7*i+j) << endl;
            auto model = std::make_shared<Entity>(geometry, material, ("model" + std::to_string(7*i+j)));
            model->setPosition(Cvec3(i*1.2 - 3.6, j*1.2 - 3.6, 0));
            model->setScale(Cvec3(0.5, 0.5, 0.5));

            auto shader = std::make_shared<PBRShader>();
            shader->createProgram(vertexShader.c_str(), fragmentShader.c_str());
            shader->ao = 1.0;
            shader->metallic = j / 6.0;
            shader->roughness = i / 6.0;
            shader->metallic = shader->metallic < 0.001 ? 0.025 : shader->metallic;
            shader->roughness = shader->roughness < 0.001 ? 0.025 : shader->roughness;

            model->setShader(shader);
            model->setParent(plane);//set plane as parent
            Scene::addChild(model);
            shaderList.push_back(shader);
        }
    }

    cout << "setting up " << "rusted_sphere" << endl;
    auto shader = std::make_shared<PBRTextureShader>();
    shader->createProgram(vertexShader.c_str(), fragmentTexShader.c_str());
    auto model = std::make_shared<Entity>(geometry, material, "rusted_sphere");
    model->setPosition(Cvec3(0, 0, -8));
    model->setScale(Cvec3(0.5, 0.5, 0.5));
    //loading textures
    shader->albedoTexture = loadGLTexture("./examples/PBR/tex/albedo.png", true, false);
    shader->normalTexture = loadGLTexture("./examples/PBR/tex/normal.png", true, false);
    shader->metallicTexture = loadGLTexture("./examples/PBR/tex/metallic.png", true, false);
    shader->roughnessTexture = loadGLTexture("./examples/PBR/tex/roughness.png", true, false);
    shader->aoTexture = loadGLTexture("./examples/PBR/tex/ao.png", true, false);
    model->setShader(shader);
    pbrTexShader = shader;
    Scene::addChild(model);
}

GLFWwindow* initWindow(const int resX, const int resY) {
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 8); // 8x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(resX, resY, "Physically Based Rendering", NULL, NULL);

    if(window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_FALSE;

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("glerror: %u!\n", error);
    }
    GLenum glewRes = glewInit();
    if(glewRes != GLEW_OK){
        printf("GLEW was not initialized successfully! %u\n", glewRes);
        exit(EXIT_FAILURE);
    }
    else{
        printf("GLEW was initialized successfully!\n");
    }
    glfwSetKeyCallback(window, controls);
    glfwSetMouseButtonCallback(window, mouse);
    glfwSetCursorPosCallback(window, curson_pos);
    glfwSetScrollCallback(window, scroll);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    int texture_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
    cout << "max texture units: " << texture_units << endl;

    glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);

    pbrShader = std::make_shared<PBRShader>();
    pbrShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    auto camera = make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    camera->setProjectionMatrix(45.0, float(screenWidth) / screenHeight, -0.01, -200);
    Scene::setCamera(camera);

    auto light0 = std::make_shared<Light>();
    light0->setPosition(4, -2, 0);
    //    light0->lightColor = Cvec3f(23.47, 21.31, 20.79);
    light0->lightColor = Cvec3f(300.0f, 300.0f, 300.0f);

    auto light1 = std::make_shared<Light>();
    light1->setPosition(-4, 2, 0);
    //    light1->lightColor = Cvec3f(23.47, 21.31, 20.79);
    light1->lightColor = Cvec3f(300.0f, 300.0f, 300.0f);


    Scene::setLight0(light0);
    Scene::setLight1(light1);


//     std::shared_ptr<Entity> model0;
//     if (modelPath == NULL) {
//         //sphere
//         model0 = std::make_shared<Entity>(std::make_shared<Sphere>(2,40,40), std::make_shared<Material>(Cvec3f(1.0, 0.0, 0.0)), "model0");
//     } else {
//         //model
//         model0 = std::make_shared<Model>(modelPath, "model0");
//     }
//     model0->translate(Cvec3(0, 0, -3));
//     model0->setShader(pbrShader);
//     Scene::addChild(model0);
//     //set trackball params
//     trackball.setInitRotation(model0->getRotation());
    setUpModels();

    // genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();
    return window;
}

// whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    trackball.updateScreenSize(width, height);
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char **argv) {
    if (argc == 2) {
        modelPath = argv[1];
    }
    glfwSetErrorCallback(glfw_error_callback);
    GLFWwindow* window = initWindow(screenWidth, screenHeight);
    if( NULL != window ) {
        display( window );
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
