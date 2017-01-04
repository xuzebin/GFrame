//
//  Entity.hpp
//  TemplateProject
//
//  Created by xuzebin on 11/17/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <list>
#include "../core/Transform.h"
#include "../geometries/Geometry.hpp"
#include "../materials/Material.h"
#include "../core/Camera.hpp"
#include "../programs/Shader.h"
#include "../core/Light.h"



class Entity;

enum EventType {
    CLICK = 0,
    HOVER = 1,
    IDLE = 2,
    SCROLL = 3
};



class ClickEventListener {
public:
    virtual void onClick(Entity* entity) = 0;
    virtual void onHover(Entity* entity) = 0;
    virtual void onIdle(Entity* entity) = 0;

protected:
    ClickEventListener() {}
    virtual ~ClickEventListener(){}
};


struct InitState {
    Transform transform;
    Cvec3f color;
};


/**
 * The object/Entity to be rendered.
 */
class Entity {
    
public:
    Transform transform;
    Geometry* geometry;
    Material* material;
    
    InitState initState;
    
protected:
    
    static int id_counter;
    
    std::string name;
    
    inline void setName(int counter) {
        if (name.empty()) {
            name = "Entity" + std::to_string(counter);
        }
    }
    
    bool visible;
    bool depthTest;
    
    //indicate which shaderprogram to use
    int programId;
    
    //currently we have only one event listener for each entity.
    ClickEventListener* clickEventListener;

    //by default: open 2 lights(..00011)
    int lightSwitch = 3;//0001 indicates light0 opens, 0010 indicates light1 opens, 0011 indicates light0 and light1 open, and so on.


public:
    Entity* parent;
    
    Entity() : Entity(NULL, NULL) {}
    
    Entity(Geometry* geometry_, Material* material_)
    : Entity("", geometry_, material_, NULL) {}
    
    Entity(std::string name_, Geometry* geometry_, Material* material_)
    : Entity(name_, geometry_, material_, NULL) {}
    
    Entity(Geometry* geometry_, Material* material_, Entity* parent_)
    : Entity("", geometry_, material_, parent_) {}
    
    Entity(std::string name_, Geometry* geometry_, Material* material_, Entity* parent_)
    :name(name_), geometry(geometry_), material(material_), parent(parent_), clickEventListener(NULL) {
        
        ++id_counter;
        visible = true;
        depthTest = true;
        
        setName(id_counter);
    }
    
    ~Entity() {
        delete material;
    }
    
    void setProgram(int programId);
    int getProgram();
    void createMesh();
    void draw(Camera* camera, Shader* shader, Light* light0, Light* light1);
    std::string getName();
    void setVisible(bool visible);
    bool isVisible() const;
    void setDepthTest(bool enable);
    bool depthTestEnabled() const;
    void setPosition(Cvec3 position);    
    void translate(Cvec3 translation);
    const Cvec3& getPosition();
    void setRotation(Quat rotation);
    void rotate(Quat rotation);
    const Quat& getRotation();
    void setScale(Cvec3 scale);
    const Cvec3& getScale();
    const Matrix4& getModelMatrix();

    void acceptLight(int lightID) {
        if (lightID < 0 || lightID > 2) {
            throw std::string("invalid lightID");
        }
        int mask = 0;
        mask <<= (lightID + 1);

        lightSwitch |= mask;
    }
    void rejectLight(int lightID) {
        if (lightID < 0 || lightID > 2) {
            throw std::string("invalid lightID");
        }
        int mask = 0;
        mask <<= (lightID + 1);
        mask = !mask;

        lightSwitch &= mask;
    }
    void rejectAllLights() {
        lightSwitch = 0;
    }

    bool isLightOn(int lightID) {
        if (lightID < 0 || lightID > 2) {
            throw std::string("invalid lightID");
        }
        int mask = 0;
        mask <<= (lightID + 1);

        int bit = lightSwitch & mask;
        return (bit != 0);
    }


    inline void registerClickEventListener(ClickEventListener* listener) {
        if (listener == NULL) {
            throw std::string("ClickEventListener object NULL");
        }
        clickEventListener = listener;
    }
    inline void removeClickEventListener() {
        clickEventListener = NULL;
    }
    bool clickEventListenerRegistered() {
        return clickEventListener != NULL;
    }
    
    void notify(EventType type) {
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
};



#endif /* Entity_hpp */
