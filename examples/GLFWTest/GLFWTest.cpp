#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
#include "geometries/Cube.h"

#include <stdio.h>

class Shader;

int screenWidth = 600;
int screenHeight = 600;

Trackball trackball(screenWidth, screenHeight);
float lineWidth = 0.5f;
bool wireframe = true;
bool solid = false;

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

    auto model0 = Scene::getEntity("model0");
    if (key == GLFW_KEY_UP) {
        model0->translate(Cvec3(0, 0, 0.5));
    } else if (key == GLFW_KEY_DOWN) {
        model0->translate(Cvec3(0, 0, -0.5));
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
    //    Scene::render();
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
        if (solid) {
            model->material->setColor(1.0, 0.88, 0.74);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_SMOOTH);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(-1, -1);
            model->draw(Scene::getCamera(), model->getShader(), Scene::getLight(0), Scene::getLight(1));
        
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
            
        if (wireframe) {
            model->material->setColor(0, 0, 0);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            model->draw(Scene::getCamera(), model->getShader(), Scene::getLight(0), Scene::getLight(1));
        }

        //update screen
        glfwSwapBuffers(window);
        //Check for any input, or window movement
        glfwPollEvents();
    }
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
    GLFWwindow* window = glfwCreateWindow(resX, resY, "GLFW Test", NULL, NULL);

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


    //    glClearColor(0.7, 0.7, 0.7, 1.0);
    glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
    glClearDepth(0.0);
    //    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);


    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    std::string vertexShader = "shaders/vertex_shader_simple.glsl";
//    std::string vertexShader = "shaders/vertex_shader_model.glsl";
    std::string fragmentShader = "shaders/fragment_shader_color.glsl";
     //    std::string fragmentShader = "shaders/fragment_shader_model.glsl";

    auto colorShader = std::make_shared<ColorShader>();
    colorShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());
//     auto colorShader = std::make_shared<ModelShader>();
//     colorShader->createProgram(vertexShader.c_str(), fragmentShader.c_str());

    auto camera = make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    camera->setZNear(-0.001);
    camera->setZFar(-100);
    Scene::setCamera(camera);

    auto light0 = std::make_shared<Light>();
    light0->setPosition(0, 0, 5);
    light0->lightColor = Cvec3f(1, 1, 1);

    Scene::setLight0(light0);

    //    auto model0 = std::make_shared<Model>("assets/models/torus/catmark_torus_creases0.obj", "model0", "assets/models/torus/");
//     auto model0 = std::make_shared<Model>("assets/models/knot/texturedknot.obj", "model0", "assets/models/knot/");
//    auto model0 = std::make_shared<Model>("assets/models/head/head.obj", "model0", "assets/models/head/");
  //   auto model0 = std::make_shared<Model>("assets/models/sportsCar/sportsCar.obj", "model0", "assets/models/sportsCar/");
//     auto model0 = std::make_shared<Model>("assets/models/cup/cup_mesh_sim.obj", "model0", "assets/models/cup/");
//    auto model0 = std::make_shared<Model>("assets/models/mountain/mountain.obj", "model0", "assets/models/mountain/");
    //auto model0 = std::make_shared<Model>("assets/models/face/face.obj", "model0", "assets/models/face/");
    if (modelPath == NULL) {
        modelPath = "assets/models/head/head.obj";
    }
    auto model0 = std::make_shared<Model>(modelPath, "model0");
    model0->material->setColor(0.8, 0.8, 0.8);
    model0->translate(Cvec3(0, 0, -3));

    //    model0->setRotation(Quat::makeYRotation(30) * Quat::makeXRotation(-30));
    model0->setShader(colorShader);
//     model0->geometry->drawBoundingBox(true);
    Scene::addChild(model0);


//     /** Button config **/
//     auto cube = std::make_shared<Cube>(2);
//     auto material = std::make_shared<Material>();
//     auto box = std::make_shared<Entity>(cube, material, "box");
//     box->setPosition(Cvec3(0, 0, -4));
//     box->material->setColor(0.9, 0.9, 0.9);
//     //    box->registerClickEventListener(std::unique_ptr<ButtonListener>(new ButtonListener()));
//     box->setShader(colorShader);
//     Scene::addChild(box);

    //set trackball params
    trackball.setInitRotation(model0->getRotation());

    // genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();
    return window;
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

// void keyboard(unsigned char key, int x, int y) {
//     auto model = Scene::getEntity("model0");
//     switch (key) {
//         case 'q':
//         case 'Q':
//         {
//             exit(0);
//             break;
//         }
//         case 'w':
//             model->translate(Cvec3(0, 0.1, 0));
//             break;
//         case 's':
//             model->translate(Cvec3(0, -0.1, 0));
//             break;
//         case 'a':
//             model->translate(Cvec3(-0.1, 0, 0));
//             break;
//         case 'd':
//             model->translate(Cvec3(0.1, 0, 0));
//             break;
//         case 'z':
//             model->translate(Cvec3(0, 0, -0.1));
//             break;
//         case 'x':
//             model->translate(Cvec3(0, 0, 0.1));
//             break;
//         default:
//             break;
//     }
// }

// bool mouseLeftDown = false;

// void mouse(int button, int state, int x, int y) {
//     if (button == GLUT_LEFT_BUTTON) {
//         if (state == GLUT_DOWN) {
//             trackball.record(x, y);
//             mouseLeftDown = true;
//         } else if (state == GLUT_UP) {
//             mouseLeftDown = false;
//         }
//     }
// }

// void motion(int x, int y) {
//     if (mouseLeftDown) {
//         auto model = Scene::getEntity("model0");
//         if (model != NULL) {
//             Quat rotation = trackball.getRotation(x, y);
//             model->setRotation(rotation);
//             std::cout << "position: " << model->getPosition() << std::endl;
//             std::cout << "rotation: " << model->getRotation() << std::endl;
//         }
//     }
// }

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
// int main(int argc, char **argv) {
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
//     glutInitWindowSize(screenWidth, screenHeight);
//     glutCreateWindow("Trackball");
    
//     glutDisplayFunc(display);
//     glutReshapeFunc(reshape);
//     glutIdleFunc(idle);

//     glutKeyboardFunc(keyboard);
//     glutMouseFunc(mouse);
//     glutMotionFunc(motion);

//     glutSetOption(GLUT_MULTISAMPLE, 4);
//     glEnable(GL_MULTISAMPLE);


//     init();
//     glutMainLoop();
//     return 0;
// }

