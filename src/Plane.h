//
//  Plane.h
//  TemplateProject
//
//  Created by xuzebin on 11/10/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Plane_h
#define Plane_h

class Plane : public Geometry {
    
protected:
    float size;
    
public:
    Plane(float size) {
        this->size = size;
    }
    
    virtual void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getPlaneVbIbLen(vertexBufferLen, indexBufferLen);
        
        std::vector<Vertex> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makePlane(size, vtx.begin(), idx.begin());
        
        Geometry::createVBOs(vtx, idx);
    }
};

#endif /* Plane_h */
