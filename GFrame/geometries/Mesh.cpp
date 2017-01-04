#include "Mesh.hpp"

void Mesh::createVBOs() {
    if (vtx.empty() || idx.empty()) {
        throw std::string("vertex or index array NULL");
    }
    
    Geometry::createVBOs(vtx, idx);
    
    clearDataInMemoery();
}

