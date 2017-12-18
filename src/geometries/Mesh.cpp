#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vtx, std::vector<unsigned short> idx, bool clean_data) : vtx(vtx),
                                                                                        idx(idx),
                                                                                        clean_data(clean_data)
{
}

Mesh::~Mesh()
{
}

void Mesh::createVBOs() {
    if (vtx.empty() || idx.empty()) {
        throw std::string("vertex or index array NULL");
    }
    
    Geometry::createVBOs(vtx, idx);

    if (clean_data) {
        clearDataInMemoery();
    }
}

void Mesh::clearDataInMemoery() {
    vtx.clear();
    idx.clear();
}
