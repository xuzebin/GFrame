#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <vector>
#include "Geometry.hpp"

class Mesh : public Geometry {
    
public:
    // clean_data true if we want to clean the data aftering sending VBO
    // otherwise it will be saved in the memory
    Mesh(std::vector<Vertex> vtx, std::vector<unsigned short> idx, bool clean_data = true);
    ~Mesh();

    void createVBOs();

    virtual const std::vector<Vertex>& getVertices() {
        return vtx;
    }

private:
    std::vector<Vertex> vtx;
    std::vector<unsigned short> idx;

    bool clean_data;
    
    void clearDataInMemoery();
};


#endif /* Mesh_hpp */
