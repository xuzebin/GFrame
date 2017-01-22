#ifndef Camera_hpp
#define Camera_hpp

#include "../base/quat.h"
#include "../base/cvec.h"

/**
 * The camera object holds the view matrix as well as projection matrix.
 */
class Camera {

public:
    static const Cvec3 UP_VEC;
    static const double Z_NEAR;
    static const double Z_FAR;

    Camera();
    Camera(const Cvec3& position, const Quat& rotation);

    const Matrix4& getViewMatrix() const       { return viewMatrix; }
    const Matrix4& getProjectionMatrix() const { return projectionMatrix; }
    const Cvec3& getPosition() const           { return position; }
    double getFov() const                      { return fov; }

    void updateView(const Cvec3& target)       { viewMatrix = setLookat(position, target, UP_VEC); }
    
    void rotate(const Quat& rotation);
    void setPosition(const Cvec3& position);

private:
    void setViewMatrix(const Cvec3& position, const Quat& rotation);
    void updateViewMatrix();
    void setProjectionMatrix(const double fov,
                             const double aspectRatio,
                             const double zNear,
                             const double zFar);

    Cvec3 position;
    Quat rotation;
    
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;

    double fov = 45.0;
};

#endif /* Camera_hpp */
