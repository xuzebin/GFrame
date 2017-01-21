#include "Entity.hpp"


int Entity::id_counter = 0;

void Entity::setProgram(int programId) {
    this->programId = programId;
}

int Entity::getProgram() {
    return programId;
}

void Entity::createMesh()  {
    if (geometry == NULL) {
        throw string("Geometry NULL");
    }
    geometry->createVBOs();
    
    
    initState.transform = transform;
    initState.color = material->getColor();
}

void Entity::draw(Camera* camera, Shader* shader, Light* light0, Light* light1) {
    if (!isVisible()) {
        return;
    }
    if (material == NULL) {
        throw string("Material NULL");
    }
    if (geometry == NULL) {
        throw std::string("Geometry NULL");
    }
    
    shader->setLocationsAndDraw(this, camera, light0, light1);
}

std::string Entity::getName() {
    return name;
}

void Entity::setVisible(bool visible) {
    this->visible = visible;
}

bool Entity::isVisible() const {
    return this->visible;
}

void Entity::setDepthTest(bool enable) {
    depthTest = enable;
}

bool Entity::depthTestEnabled() const {
    return depthTest;
}

void Entity::setPosition(Cvec3 position) {
    transform.setPosition(position);
}

void Entity::translate(Cvec3 translation) {
    transform.translate(translation);
}

const Cvec3& Entity::getPosition() {
    return transform.getPosition();
}

void Entity::setRotation(Quat rotation) {
    transform.setRotation(rotation);
}

void Entity::rotate(Quat rotation) {
    transform.rotate(rotation);
}

const Quat& Entity::getRotation() {
    return transform.getRotation();
}

void Entity::setScale(Cvec3 scale) {
    transform.setScale(scale);
}

const Cvec3& Entity::getScale() {
    return transform.getScale();
}

const Matrix4& Entity::getModelMatrix() {
    return transform.getModelMatrix();
}

void Entity::setModelMatrix(const Matrix4& m) {
    transform.setModelMatrix(m);
}
