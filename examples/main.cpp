#include <GLUT/glut.h>
#include "base/glsupport.h"
#include "base/geometrymaker.h"
#include "base/matrix4.h"
#include "base/quat.h"
#include "core/Transform.hpp"
#include "materials/Material.hpp"
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "geometries/Geometry.hpp"
#include "geometries/Cube.h"
#include "geometries/Plane.h"
#include "geometries/Skybox.h"
#include "geometries/Sphere.h"
#include "entities/Model.hpp"
#include "core/Light.hpp"
#include "programs/Shader.h"
#include "programs/ColorShader.h"
#include "programs/ModelShader.h"
#include "programs/TextureShader.h"
#include "programs/ScreenShader.h"
#include "geometries/Screen.h"
#include "glObjects/FrameBufferObject.hpp"
#include "materials/Color.h"
#include "materials/Cubemap.hpp"
#include "controls/Trackball.hpp"

int screenWidth = 600;
int screenHeight = 600;

std::shared_ptr<ColorShader> colorShader;
std::shared_ptr<ModelShader> modelShader;
std::shared_ptr<TextureShader> textureShader, cubemapShader, refractShader, reflectShader;
std::shared_ptr<ScreenShader> screenShader, grayShader, colorInvertShader, fxaaShader, toneMappingShader, hdrFxaaShader, horizontalBlurShader, verticalBlurShader, cartoonifyShader;


std::string currentModel = "model0";

std::shared_ptr<Light> currentMovingLight;

std::shared_ptr<Entity> screen(nullptr);//for post-processing

std::shared_ptr<FrameBufferObject> firstFBO(nullptr);
std::shared_ptr<FrameBufferObject> secondFBO(nullptr);

Trackball trackball(screenWidth, screenHeight);


class BtnEventListener : public ClickEventListener {

public:
    BtnEventListener() {}
    void onClick(Entity& button) {
        button.setPosition(button.initState.transform.getPosition() + Cvec3(0, 0, -0.2));
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

class PostProcessingSwitchListener : public BtnEventListener {
private:
    int effectSelected = 0;
public:
    void onClick(Entity& button) {
        BtnEventListener::onClick(button);
        effectSelected = (effectSelected + 1) % 7;
        switch(effectSelected) {
            case 0:
            {
                screen->setShader(screenShader);
                break;
            }
            case 1:
            {
                screen->setShader(grayShader);
                break;
            }
            case 2:
            {
                screen->setShader(colorInvertShader);
                break;
            }
            case 3:
            {
                screen->setShader(fxaaShader);
                break;
            }
            case 4:
            {
                screen->setShader(toneMappingShader);
                break;
            }
            case 5:
            {
                screen->setShader(hdrFxaaShader);
                break;
            }
            case 6:
            {
                screen->setShader(cartoonifyShader);
                break;
            }
            default:
            {
                std:cerr << "no matched model selected" << std::endl;
                return;
            }
        }
    }
    void onHover(Entity& button) {
        BtnEventListener::onHover(button);
    }
    void onIdle(Entity& button) {
        BtnEventListener::onIdle(button);
    }
};

class ProgramSwitchBtnEventListener : public BtnEventListener {
private:
    int programSelected = 0;
public:
    void onClick(Entity& button) {
        BtnEventListener::onClick(button);
        programSelected = (programSelected + 1) % 4;
        auto monk = Scene::getEntity("model0");
        switch(programSelected) {
            case 0:
            {
                monk->setShader(refractShader);
                break;
            }
            case 1:
            {
                monk->setShader(reflectShader);
                break;
            }
            case 2:
            {
                monk->setShader(modelShader);
                break;
            }
            case 3:
            {
                monk->setShader(colorShader);
                break;
            }
            default:
                throw std::string("no matched program selected");
        }
    }
    void onHover(Entity& button) {
        BtnEventListener::onHover(button);
    }
    void onIdle(Entity& button) {
        BtnEventListener::onIdle(button);
    }
};

class LightColorBtnEventListener : public BtnEventListener {
private:
    int lightColor0 = 0;
    int lightColor1 = 0;
public:
    void onClick(Entity& button) {
        BtnEventListener::onClick(button);
        
        if (button.getName() == "button1") {
            lightColor0 = (lightColor0 + 1) % 2;
            if (lightColor0 == 0) {
                currentMovingLight = Scene::getLight(0);
                Scene::getLight(0)->lightColor = Color::WHITE;
                button.initState.color = Color::YELLOW;
            } else {
                currentMovingLight = NULL;
                Scene::getLight(0)->lightColor = Color::BLACK;
                button.initState.color = Color::BLACK;
            }
        } else {
            lightColor1 = (lightColor1 + 1) % 2;
            if (lightColor1 == 0) {
                currentMovingLight = Scene::getLight(1);
                Scene::getLight(1)->lightColor = Color::WHITE;
                button.initState.color = Color::YELLOW;
            } else {
                currentMovingLight = NULL;
                Scene::getLight(1)->lightColor = Color::BLACK;
                button.initState.color = Color::BLACK;
            }
        }
        
    }
    void onHover(Entity& button) {
        BtnEventListener::onHover(button);
    }
    void onIdle(Entity& button) {
        BtnEventListener::onIdle(button);
    }
};

class SpecularLightColorBtnEventListener : public BtnEventListener {
private:
    bool spcularLightColorOn0 = true;
    bool spcularLightColorOn1 = true;
public:
    void onClick(Entity& button) {
        BtnEventListener::onClick(button);
        
        if (button.getName() == "button3") {
            spcularLightColorOn0 = !spcularLightColorOn0;
            if(spcularLightColorOn0) {
                Scene::getLight(0)->specularLightColor = Color::WHITE;
                button.material->setColor(Color::WHITE);
                button.initState.color = Cvec3f(Color::WHITE);
            } else {
                Scene::getLight(0)->specularLightColor = Color::BLACK;
                button.material->setColor(Color::BLACK);
                button.initState.color = Cvec3f(Color::BLACK);
            }
        } else {
            spcularLightColorOn1 = !spcularLightColorOn1;
            if(spcularLightColorOn1) {
                Scene::getLight(1)->specularLightColor = Color::WHITE;
                button.material->setColor(Color::WHITE);
                button.initState.color = Color::WHITE;
            } else {
                Scene::getLight(1)->specularLightColor = Color::BLACK;
                button.material->setColor(Color::BLACK);
                button.initState.color = Color::BLACK;
            }
        }
    }
    void onHover(Entity& button) {
        BtnEventListener::onHover(button);
    }
    void onIdle(Entity& button) {
        BtnEventListener::onIdle(button);
    }
};


void display(void) {
    //    Scene::render();//render the scene directly to scren
    //render to texture and then to screen using one fbo

    //blur effect rendering using 2 fbos
    float time = (float) glutGet(GLUT_ELAPSED_TIME) / 1000.0f;//second passed
    float blurSize = 0.02f - 0.001f * time * 15.0f;

    if (blurSize < -1e-8) {
        std::cout << "not blurring" << std::endl;
        screen->material->setDiffuseTextureId(firstFBO->getFrameBufferTexture());
        Scene::renderToTexture();
        Scene::renderToScreen(screenWidth, screenHeight);
    } else {
        std::cout << "blur eefect" << std::endl;
        horizontalBlurShader->setBlurSize(blurSize);
        verticalBlurShader->setBlurSize(blurSize);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, firstFBO->getFrameBuffer());
        glViewport(0, 0, 1024, 1024);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene::renderLoop();//render the scene to the firstFBO

        glBindFramebuffer(GL_FRAMEBUFFER, secondFBO->getFrameBuffer());
        glViewport(0, 0, 1024, 1024);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (screen != nullptr) {
            screen->material->setDiffuseTextureId(firstFBO->getFrameBufferTexture());
            screen->setShader(horizontalBlurShader);
            screen->draw(Scene::getCamera(), horizontalBlurShader, Scene::getLight(0), Scene::getLight(1));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, screenWidth, screenHeight);
        if (screen != nullptr) {
            screen->material->setDiffuseTextureId(secondFBO->getFrameBufferTexture());
            screen->setShader(verticalBlurShader);
            screen->draw(Scene::getCamera(), verticalBlurShader, Scene::getLight(0), Scene::getLight(1));
        }
        
        glutSwapBuffers();
    }

}

void init() {
    glClearColor(0.3, 0.3, 0.5, 1.0);
    glClearDepth(0.0);
    glCullFace(GL_BACK);
//    glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    
    
    colorShader = std::make_shared<ColorShader>();
    colorShader->createProgram("shaders/vertex_shader_simple.glsl", "shaders/fragment_shader_color.glsl");
    
    modelShader = std::make_shared<ModelShader>();
    modelShader->createProgram("shaders/vertex_shader_model.glsl", "shaders/fragment_shader_model.glsl");
    
    textureShader = std::make_shared<TextureShader>();
    textureShader->createProgram("shaders/vertex_shader_simple.glsl", "shaders/fragment_shader_texture.glsl");

    cubemapShader = std::make_shared<TextureShader>();
    cubemapShader->createProgram("shaders/vertex_shader_cubemap.glsl", "shaders/fragment_shader_cubemap.glsl");
    
    reflectShader = std::make_shared<TextureShader>();
    reflectShader->createProgram("shaders/vertex_shader_simple.glsl", "shaders/fragment_shader_environment_reflect.glsl");
    
    refractShader = std::make_shared<TextureShader>();
    refractShader->createProgram("shaders/vertex_shader_simple.glsl", "shaders/fragment_shader_environment_refract.glsl");
    
    screenShader = std::make_shared<ScreenShader>();
    screenShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_offscreen.glsl");

    grayShader = std::make_shared<ScreenShader>();
    grayShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_blackwhite.glsl");

    colorInvertShader = std::make_shared<ScreenShader>();
    colorInvertShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_colorinvert.glsl");

    fxaaShader = std::make_shared<ScreenShader>();
    fxaaShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_fxaa.glsl");

    toneMappingShader = std::make_shared<ScreenShader>();
    toneMappingShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_tone_mapping.glsl");

    hdrFxaaShader = std::make_shared<ScreenShader>();
    hdrFxaaShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_tonemapping_fxaa.glsl");

    horizontalBlurShader = std::make_shared<ScreenShader>();
    horizontalBlurShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_horizontal_blur.glsl");

    verticalBlurShader = std::make_shared<ScreenShader>();
    verticalBlurShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_vertical_blur.glsl");

    cartoonifyShader = std::make_shared<ScreenShader>();
    cartoonifyShader->createProgram("shaders/vertex_shader_offscreen.glsl", "shaders/fragment_shader_postprocessing_cartoonify.glsl");

    auto camera = std::make_shared<Camera>(Cvec3(0, 0, 0), Quat::makeXRotation(0));
    Scene::setCamera(camera);
    
    auto light0 = std::make_shared<Light>();
    light0->setPosition(1, 5, 0);

    auto light1 = std::make_shared<Light>();
    light1->setPosition(-1, 0, 0);

    Scene::setLight0(light0);
    Scene::setLight1(light1);

    currentMovingLight = Scene::getLight(0);


    auto model0 = std::make_shared<Model>("assets/models/monk/Monk_Giveaway_Fixed.obj", "model0", "assets/models/monk/");
    model0->setPosition(Cvec3(0, 0, -1.5));
    model0->setRotation(Quat::makeYRotation(20));
    model0->material->setColor(0.0, 0.8, 0.8);
    //    model0->setShader(refractShader);
    model0->setShader(modelShader);
    //    model0->transform.setPivot(0, 3, 0);
    Scene::addChild(model0);

    
    /************ program swtich button ************/
    auto buttonG = std::make_shared<Sphere>(2, 40, 40);
    auto buttonM = std::make_shared<Material>(Color::RED);
    auto btn0 = std::make_shared<Entity>(buttonG, buttonM, "button0");
    btn0->setPosition(Cvec3(-1.8, 1.9, -5));
    btn0->setScale(Cvec3(0.1, 0.1, 0.1));
    btn0->registerClickEventListener(std::unique_ptr<ProgramSwitchBtnEventListener>(new ProgramSwitchBtnEventListener()));
    btn0->setShader(colorShader);
    Scene::addChild(btn0);

    /************ post-processing effect swtich button ************/
    auto btn1 = std::make_shared<Entity>(buttonG, buttonM, "button1");
    btn1->setPosition(Cvec3(-1.5, 1.9, -5));
    btn1->setScale(Cvec3(0.1, 0.1, 0.1));
    btn1->registerClickEventListener(std::unique_ptr<PostProcessingSwitchListener>(new PostProcessingSwitchListener()));
    btn1->setShader(colorShader);
    Scene::addChild(btn1);
    
    /************ light color buttons ************/
    for (int i = 0; i < 2; ++i) {
        auto buttonM = std::make_shared<Material>(Color::YELLOW);
        auto btn = std::make_shared<Entity>(buttonG, buttonM, ("button" + std::to_string(i + 1)));
        btn->setPosition(Cvec3(-1.8, 1.9 - (i + 1) / 3.0, -5));
        btn->setScale(Cvec3(0.1, 0.1, 0.1));
        btn->registerClickEventListener(std::unique_ptr<LightColorBtnEventListener>(new LightColorBtnEventListener()));
        btn->setShader(colorShader);
        Scene::addChild(btn);
    }
    
    /************ specular light color buttons ************/
    for (int i = 0; i < 2; ++i) {
        auto buttonM = std::make_shared<Material>(Color::WHITE);
        auto btn = std::make_shared<Entity>(buttonG, buttonM, ("button" + std::to_string(i + 3)));
        btn->setPosition(Cvec3(-1.5, 1.9 - (i + 1) / 3.0, -5));
        btn->setScale(Cvec3(0.1, 0.1, 0.1));
        btn->registerClickEventListener(std::unique_ptr<SpecularLightColorBtnEventListener>(new SpecularLightColorBtnEventListener()));
        btn->setShader(colorShader);
        Scene::addChild(btn);
    }

    Cubemap cubemap;
    cubemap.loadTextures("assets/cubemap/posx.jpg", "assets/cubemap/negx.jpg", "assets/cubemap/posy.jpg", "assets/cubemap/negy.jpg", "assets/cubemap/posz.jpg", "assets/cubemap/negz.jpg");

    auto cubemapM = std::make_shared<Material>();
    cubemapM->setCubemap(cubemap.getTexture());
    auto sb = std::make_shared<Cube>(100);
    auto skybox = std::make_shared<Entity>(sb, cubemapM);
    skybox->setShader(cubemapShader);
    skybox->setRotation(Quat::makeYRotation(180));
    // comment it out temperarily, because the model is not shown if skybox is rendered.
    //    Scene::addChild(skybox);


    firstFBO = std::make_shared<FrameBufferObject>(1024, 1024, true);
    Scene::setFrameBufferObject(firstFBO);


    secondFBO = std::make_shared<FrameBufferObject>();


    auto plane = std::make_shared<Screen>();
    auto planeM = std::make_shared<Material>();
    planeM->setDiffuseTextureId(firstFBO->getFrameBufferTexture());
    screen = std::make_shared<Entity>(plane, planeM);
    screen->setShader(screenShader);
    Scene::setScreen(screen);


    //set trackball params
    trackball.setInitRotation(model0->getRotation());

    //this call genereate vbo/ibo for the geometry of each Entity.
    Scene::createMeshes();
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
            Scene::getCamera()->rotate(Quat::makeYRotation(10));
            break;
        case 'k':
        case 'K':
            Scene::getCamera()->rotate(Quat::makeYRotation(-10));
            break;
        case 's':
        case 'S':
        {
            auto model = Scene::getEntity(currentModel);
            model->setScale(model->getScale() * 1.1);
            break;
        }
        case 'd':
        case 'D':
        {
            auto model = Scene::getEntity(currentModel);
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
        case 'q':
        case 'Q':
            exit(0);
            break;
        default:
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    Scene::updateMouseEvent(button, state, x, y, screenWidth, screenHeight);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        trackball.record(x, y);
    }
}

void motion(int x, int y) {
    auto model = Scene::getEntity(currentModel);
    if (model != nullptr) {
        Quat rotation = trackball.getRotation(x, y);
        model->setRotation(rotation);
    } else {
        std::cerr << "model not found in the scene" << std::endl;
    }
}

bool insideWindow(int x, int y) {
    return (x >= 0 && y >= 0 && x <= screenWidth && y <= screenHeight);
}
void passiveMotion(int x, int y) {
    if (insideWindow(x, y)) {
        if (cartoonifyShader != nullptr) {
            cartoonifyShader->updateMouseX((float)x / screenWidth);
        }
        Scene::updateMousePassiveMotion(x, y, screenWidth, screenHeight);
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h;

    //update trackball params
    trackball.updateScreenSize(screenWidth, screenHeight);
}

void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);// | GLUT_MULTISAMPLE);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Main Example");
    
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

