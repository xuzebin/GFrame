#include "Entity.hpp"


int Entity::id_counter = 0;

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

