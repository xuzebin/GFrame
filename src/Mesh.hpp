//
//  Mesh.hpp
//  TemplateProject
//
//  Created by xuzebin on 11/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "cvec.h"
#include "tiny_obj_loader.h"
#include "Geometry.hpp"



class CallbackInterface {
public:
    virtual void setTextures(std::string diffuseTexName, std::string specularTexName, std::string normalTexName) = 0;
};

class Mesh : public Geometry {
    
private:
    const std::string fileName;
    
    CallbackInterface* callback;
    
    void calcFaceTangent(const Cvec3f& v1, const Cvec3f& v2, const Cvec3f& v3, const Cvec2f& texcoord1, const Cvec2f& texcoord2, const Cvec2f& texcoord3, Cvec3f& tangent, Cvec3f& binormal);
    void loadFromFile(const std::string& fileName, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices);
    
public:
    
    Mesh(const std::string fileName_) : fileName(fileName_) {}
    
    inline void addCallback(CallbackInterface* callback) {
        this->callback = callback;
    }
    
    void createVBOs();
};


#endif /* Mesh_hpp */
