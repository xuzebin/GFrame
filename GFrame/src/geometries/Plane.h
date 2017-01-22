#ifndef Plane_h
#define Plane_h

class Plane : public Geometry {
    
protected:
    float size;
    
public:
    Plane(float size) {
        this->size = size;
        boundingBoxLength = size;
    }
    
    void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getPlaneVbIbLen(vertexBufferLen, indexBufferLen);
        
        std::vector<Vertex> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makePlane(size, vtx.begin(), idx.begin());
        
        Geometry::createVBOs(vtx, idx);
    }
};

#endif /* Plane_h */
