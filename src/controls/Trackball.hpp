#ifndef Trackball_hpp
#define Trackball_hpp

#include "../base/cvec.h"
#include "../base/quat.h"

/**
 * Usage: 
 * 1. set radius and screen size before rendering or update when reshape callback is called.
 * 2. record the firstly pressed mouse position & object rotation by calling record(x, y) in the mouse callback function.
 * 3. get current rotation by calling getRotation(x, y) passing in the current mouse position in the motion callback function.
 * 
 * A brief sample:
 * Trackball trackball;
 * void init() {
 *     //GL initialization
 *     //...
 *
 *     trackball.setRadius(screenWidth < screenHeight ? screenWidth / 2 : screenHeight / 2);
 *     trackball.setScreenSize(screenWidth, screenHeight);
 * }
 * void reshape(int w, int h) {
 *     //update screensize
 *     //...
 *
 *     trackball.setRadius(screenWidth < screenHeight ? screenWidth / 2 : screenHeight / 2);
 *     trackball.setScreenSize(screenWidth, screenHeight);
 * }
 * bool mouseLeftDown = false;
 * void mouse(int button, int state, int x, int y) {
 *     if (buton == GLUT_LEFT_BUTTON) {
 *         if (state == GLUT_DOWN) {
 *             trackball.record(x, y);
 *             mouseLeftDown = true;
 *         } else if (state == GLUT_UP) {
 *             mouseLeftDown = false;
 *         }
 *     }
 * }
 * void moition(int x, int y) {
 *    if (mouseLeftDown) {
 *        Quat rotation = trackball.getRotation(x, y);
 *        //rotate the object by setting this rotation and we are all done!
 *        Entity* model = Scene::getEntity("modelName");
 *        model->setRotation(rotation);
 *    }
 * }
 * 
 *
 */
class Trackball {

public:
    Trackball();
    Trackball(float radius, int screenWidth, int screenHeight);
    ~Trackball();
    
    void setRadius(float radius) { this->radius = radius; }
    void setScreenSize(int screenWidth, int screenHeight);
    void setRadiusAndScreenSize(float radius, int screenWidth, int screenHeight);
    void setSpeed(float speed)   { this->speed = speed; }
    
    float getRadius() const      { return radius; }
    int getScreenWidth() const   { return screenWidth; }
    int getScreenHeight() const  { return screenHeight; }
    float getSpeed() const       { return speed; }

    Quat getQuatBetween(const Cvec3& v1, const Cvec3& v2);
    
    Cvec3 getVector(int x, int y) const;
    Cvec3 getUnitVector(int x, int y) const;
    
    /** record the mouse position and rotation of the previous state **/
    void record(int x, int y);

    /** get current rotation based on previous and current mouse position **/
    const Quat& getRotation(int x, int y);
    
private:
    void recordMousePosition(int x, int y);
    void setRotationOld() { preRotation = curRotation; }

    float radius;
    int screenWidth;
    int screenHeight;
    float speed;

    int preX, preY;
    Quat curRotation, preRotation;
};

#endif /* Trackball_hpp */
