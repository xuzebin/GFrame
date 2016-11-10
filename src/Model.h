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

class Model : public Geometry {
private:
    const std::string fileName;
public:
    Model(const std::string fileName_) : fileName(fileName_) {}
    
    void createVBOs() {
        std::vector<Vertex> vertices;
        std::vector<unsigned short> indices;
        loadFromFile(fileName, vertices, indices);
        
        Geometry::createVBOs(vertices, indices);
    }
    
private:
    
    void loadFromFile(const std::string& fileName, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
        
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
        
        if (ret) {
            for (int i = 0; i < attrib.vertices.size(); i += 3) {
                Vertex v;
                for (int j = 0; j < 3; ++j) {
                    v.position[j] = attrib.vertices[i + j];
                    v.normal[j]   = attrib.normals[i + j];
                }
                vertices.push_back(v);
            }
            for (int i = 0; i < shapes.size(); ++i) {
                for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
                    indices.push_back(shapes[i].mesh.indices[j].vertex_index);
                }
            }
        } else {
            std::cout << err << std::endl;
            assert(false);
        }
    }
};


#endif /* Model_h */
