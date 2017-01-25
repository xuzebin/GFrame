#include "Entity.hpp"

int Entity::id_counter = 0;

Entity::Entity(Geometry* geometry, Material* material, std::string name) : name(name),
                                                                           geometry(geometry),
                                                                           material(material),
                                                                           parent(NULL),
                                                                           clickEventListener(NULL),
                                                                           visible(true),
                                                                           depthTest(true)

{
    setName(id_counter++);
}

Entity::~Entity() {
    delete material;
    delete geometry;
    delete clickEventListener;
}

void Entity::createMesh()  {
    if (geometry == NULL) {
        throw string("Geometry NULL");
    }
    geometry->createVBOs();
    
    
    initState.transform = transform;
    initState.color = material->getColor();
}

void Entity::draw(std::shared_ptr<Camera> camera, Shader* shader, std::shared_ptr<Light> light0, std::shared_ptr<Light> light1) {
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

void Entity::acceptLight(int lightID) {
    if (lightID < 0 || lightID > 1) {
        throw std::string("invalid lightID");
    }
    int mask = 1;
    mask <<= lightID;

    lightSwitch |= mask;
}

void Entity::rejectLight(int lightID) {
    if (lightID < 0 || lightID > 1) {
        throw std::string("invalid lightID");
    }
    int mask = 1;
    mask <<= lightID;
    mask = !mask;

    lightSwitch &= mask;
}

bool Entity::isLightOn(int lightID) {
    if (lightID < 0 || lightID > 1) {
        throw std::string("invalid lightID");
    }
    int mask = 1;
    mask <<= lightID;

    int bit = lightSwitch & mask;
    return (bit != 0);
}

void Entity::registerClickEventListener(ClickEventListener* listener) {
    if (listener == NULL) {
        throw std::string("ClickEventListener object NULL");
    }
    clickEventListener = listener;
}

void Entity::notify(EventType type) {
    if (clickEventListener != NULL) {
        switch(type) {
            case EventType::CLICK:
                clickEventListener->onClick(this);
                break;
            case EventType::HOVER:
                clickEventListener->onHover(this);
                break;
            case EventType::IDLE:
                clickEventListener->onIdle(this);
                break;
            default:
                throw std::string("no matched event type");
        }
    }
}

void Entity::setName(int counter) {
    if (name.empty()) {
        name = "Entity" + std::to_string(counter);
    }
}
