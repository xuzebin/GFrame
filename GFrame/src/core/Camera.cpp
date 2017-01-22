#include "Camera.hpp"

const Cvec3 Camera::UP_VEC = Cvec3(0, 1, 0);
const double Camera::Z_NEAR = -0.1;
const double Camera::Z_FAR = -800;

Camera::Camera() : position(0, 0, 0)
{
    updateView(Cvec3(0, 0, 0));
    setProjectionMatrix(fov, 1.0, Z_NEAR, Z_FAR);
}

Camera::Camera(const Cvec3& position, const Quat& rotation) : position(position),
                                                              rotation(rotation)
{
    setViewMatrix(position, rotation);
    setProjectionMatrix(fov, 1.0, Z_NEAR, Z_FAR);
}

void Camera::rotate(const Quat& rotation) {
    this->rotation = this->rotation * rotation;
    updateViewMatrix();
}
void Camera::setPosition(const Cvec3& position) {
    this->position = position;
    updateViewMatrix();
}

void Camera::setViewMatrix(const Cvec3& position, const Quat& rotation) {
    viewMatrix = Matrix4::makeTranslation(position) * quatToMatrix(rotation);
}

void Camera::updateViewMatrix() {
    setViewMatrix(position, rotation);
}

void Camera::setProjectionMatrix(const double fov,
                                 const double aspectRatio,
                                 const double zNear,
                                 const double zFar) {
    projectionMatrix = Matrix4::makeProjection(fov, aspectRatio, zNear, zFar);
}