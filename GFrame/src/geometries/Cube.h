#ifndef Cube_h
#define Cube_h

class Geometry;

class Cube : public Geometry {
    
protected:
    float size;
    
public:
    
    Cube(float size) {
        this->size = size;
        boundingBoxLength = size;
    }
    
    void createVBOs() {
        int vertexBufferLen, indexBufferLen;
        getCubeVbIbLen(vertexBufferLen, indexBufferLen);
        
        std::vector<Vertex> vtx(vertexBufferLen);
        std::vector<unsigned short> idx(indexBufferLen);
        
        makeCube(size, vtx.begin(), idx.begin());
        
        Geometry::createVBOs(vtx, idx);
    }
};

#endif /* Cube_h */
