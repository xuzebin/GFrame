//
//  Mesh.cpp
//  TemplateProject
//
//  Created by xuzebin on 11/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Mesh.hpp"

void Mesh::createVBOs() {
    if (vtx.empty() || idx.empty()) {
        throw std::string("vertex or index array NULL");
    }
    
    Geometry::createVBOs(vtx, idx);
    
    clearDataInMemoery();
}

