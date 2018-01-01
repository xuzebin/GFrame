/**
 * This example shows how to realize HDR Tone Mapping using GFrame.
 * Press key 't' to show the effect of tone mapping.
 * Press key 'f' to show the effect of not using tone mapping.
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
#include "programs/ModelShader.h"
#include "programs/ScreenShader.h"
#include "programs/ColorShader.h"
#include "glObjects/FrameBufferObject.hpp"
#include "controls/Trackball.hpp"

int screenWidth = 600;
int screenHeight = 600;

std::shared_ptr<Entity> screen;
std::shared_ptr<ScreenShader> screenShader, simpleShader;
std::shared_ptr<ModelShader> modelShader;
std::shared_ptr<ColorShader> wireframeShader;

Trackball trackball(screenWidth, screenHeight);
float lineWidth = 0.5f;
bool wireframe = false;
bool solid = true;

char* modelPath = NULL;

void controls(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::cout<< ("%d\n",key) << std::endl;
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

    switch (key) {
    case GLFW_KEY_T:
        {
            //Use Tone Mapping shader
            screen->setShader(screenShader);
            break;
        }
    case GLFW_KEY_F:
        {
            //Use Simple shader
            screen->setShader(simpleShader);
            break;
        }
    case GLFW_KEY_Q:
        {
            exit(0);
            break;
        }
    default:
        break;
    }
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
        auto model = Scene::getEntity("model0");
        if (model != NULL) {
            Quat rotation = trackball.getRotation(xpos, ypos);
            model->setRotation(rotation);
            std::cout << "position: " << model->getPosition() << std::endl;
            std::cout << "rotation: " << model->getRotation() << std::endl;
        }
    }
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
    auto model = Scene::getEntity("model0");
    model->translate(Cvec3(0, 0, yoffset));
}


void display(GLFWwindow* window) {
    auto model = Scene::getEntity("model0");

    while (!glfwWindowShouldClose(window)) {
        glLineWidth(lineWidth);
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth*2, windowHeight*2);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto model = Scene::getEntity("model0");
        if (solid && !wireframe) {
            model->setShader(modelShader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_SMOOTH);
            Scene::renderToTexture();// Render to the texture (FBO)
        } else if (wireframe && !solid) {
            model->setShader(wireframeShader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            model->material->setColor(0, 0, 0);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
            Scene::renderToTexture();// Render to the texture (FBO)
        } else if (wireframe && solid) {
            model->setShader(modelShader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_SMOOTH);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(-1, -1);
            Scene::renderToTexture();// Render to the texture (FBO)
        
            glDisable(GL_POLYGON_OFFSET_FILL);
            model->setShader(wireframeShader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            model->material->setColor(0, 0, 0);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
            Scene::renderLoop();// Render to the texture (FBO)
        }

        //render to screen
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth*2, windowHeight*2);
        screen->draw(Scene::getCamera(), screen->getShader(), Scene::getLight(0), Scene::getLight(1));

        //update screen
        glfwSwapBuffers(window);
        //Check for any input, or window movement
        glfwPollEvents();
    }
}

GLFWwindow* initWindow(const int resX, const int resY) {
    std::cout << "press t to enable tone mapping, f to disable" << std::endl;
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 8); // 8x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(resX, resY, "Post-Processing Tone Mapping", NULL, NULL);

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

    glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    
    std::string vertexShader = "shaders/vertex_shader_model.glsl";
    std::string fragmentShader = "shaders/fragment_shader_model.glsl";

    // Shader for model
    modelShader = std::make_shared<ModelShader>();
    modelShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    // shader for wireframe
    wireframeShader = std::make_shared<ColorShader>();
    wireframeShader->createProgram("shaders/vertex_shader_simple.glsl", "shaders/fragment_shader_color.glsl");

    // Tone mapping shader
    screenShader = std::make_shared<ScreenShader>();
    screenShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_tone_mapping.glsl");

    // Simple shader
    simpleShader = std::make_shared<ScreenShader>();
    simpleShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_offscreen.glsl");

    // Frame buffer object config
    auto fbo = std::make_shared<FrameBufferObject>(1024, 1024, true);
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
    light0->setPosition(1, 5, 0);
    auto light1 = std::make_shared<Light>();
    //    light1->setPosition(-1, 0, -2);
    light1->setPosition(-1, 0, 0);
    light1->lightColor = Cvec3f(3, 3, 3);
    Scene::setLight0(light0);
    Scene::setLight1(light1);

    // Model config
    if (modelPath == NULL) {
        modelPath = "assets/models/monk/Monk_Giveaway_Fixed.obj";
    }
    std::string filepath(modelPath);
    std::size_t found = filepath.find_last_of("/");
    std::string path = filepath.substr(0, found + 1);
    std::string file = filepath.substr(found + 1);
    auto model0 = std::make_shared<Model>(filepath, "model0", path);
    model0->setPosition(Cvec3(0, 0, -3));
    model0->setRotation(Quat::makeYRotation(20));
    model0->setShader(modelShader);
    Scene::addChild(model0);

    // Genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();

    return window;
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
