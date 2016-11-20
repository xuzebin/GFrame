//
//  Model.h
//  TemplateProject
//
//  Created by xuzebin on 11/10/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include "Geometry.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Texture.hpp"
#include "Mesh.hpp"


class Model : public Entity, public CallbackInterface {

private:

    //callback function
    void setTextures(std::string diffuseTexName, std::string specularTexName, std::string normalTexName) {
        if (material == NULL) {
            throw std::string("material NULL");
        }
        material->setTextures(diffuseTexName, specularTexName, normalTexName);
    }
    
public:
    
    Model(const std::string fileName, std::string name) {
        this->name = name;
        geometry = new Mesh(fileName);
        material = new Material();
        
        ((Mesh*)geometry)->addCallback(this);
    }
    
    //dependency injection
    Model(Mesh* mesh, Material* material, std::string name) {
        this->name = name;
        this->geometry = mesh;
        this->material = material;
        
        mesh->addCallback(this);
    }

};


#endif /* Model_h */
