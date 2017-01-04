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
#include "../base/cvec.h"
#include "../base/tiny_obj_loader.h"
#include "Geometry.hpp"


class Mesh : public Geometry {
    
private:
    std::vector<Vertex> vtx;
    std::vector<unsigned short> idx;
    
    void clearDataInMemoery() {
        vtx.clear();
        idx.clear();
    }

public:
    
    Mesh(std::vector<Vertex> vtx, std::vector<unsigned short> idx) {
        this->vtx = vtx;
        this->idx = idx;
    }

    void createVBOs();
    

};


#endif /* Mesh_hpp */
