//
//  Mesh.cpp
//  TemplateProject
//
//  Created by xuzebin on 11/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Mesh.hpp"

void Mesh::createVBOs() {
    std::vector<Vertex> vtx;
    std::vector<unsigned short> idx;
    loadFromFile(fileName, vtx, idx);
    
    Geometry::createVBOs(vtx, idx);
}

void Mesh::calcFaceTangent(const Cvec3f& v1, const Cvec3f& v2, const Cvec3f& v3, const Cvec2f& texcoord1, const Cvec2f& texcoord2, const Cvec2f& texcoord3, Cvec3f& tangent, Cvec3f& binormal) {
    Cvec3f side0 = v1 - v2;
    Cvec3f side1 = v3 - v1;
    Cvec3f normal = cross(side1, side0);
    normal = normalize(normal);
    float deltaV0 = texcoord1[1] - texcoord2[1];
    float deltaV1 = texcoord3[1] - texcoord1[1];
    tangent = normalize(side0 * deltaV1 - side1 * deltaV0);
    
    float deltaU0 = texcoord1[0] - texcoord2[0];
    float deltaU1 = texcoord3[0] - texcoord1[0];
    binormal = normalize(side0 * deltaU1 - side1 * deltaU0);
    
    Cvec3f tangentCross = cross(tangent, binormal);
    tangent = (dot(tangentCross, normal) < 0.0f) ? (tangent * -1) : tangent;
}

void Mesh::loadFromFile(const std::string& fileName, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
    if (callback != NULL) {
        for (int i = 0; i < materials.size(); ++i) {
            callback->setTextures(materials[i].diffuse_texname, materials[i].specular_texname, materials[i].normal_texname);
        }
    }
    
    for (int i = 0; i < materials.size(); ++i) {
        std::cout << "diffuse texture: " << materials[i].diffuse_texname << std::endl;
        std::cout << "specular texture: " << materials[i].specular_texname << std::endl;
        std::cout << "bump(normal) texture: " << materials[i].bump_texname << std::endl;
        std::cout << "normal texture: " << materials[i].normal_texname << std::endl;
    }
    
    if (ret) {
        //            for (int i = 0; i < attrib.vertices.size(); i += 3) {
        //                Vertex v;
        //                for (int j = 0; j < 3; ++j) {
        //                    v.position[j] = attrib.vertices[i + j];
        //                    v.normal[j]   = attrib.normals[i + j];
        //                }
        //                vertices.push_back(v);
        //            }
        //            for (int i = 0; i < shapes.size(); ++i) {
        //                for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
        //                    indices.push_back(shapes[i].mesh.indices[j].vertex_index);
        //                }
        //            }
        for (int i = 0; i < shapes.size(); ++i) {
            for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
                unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
                unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
                unsigned int texCoordOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
                
                Vertex v;
                v.position[0] = attrib.vertices[vertexOffset];
                v.position[1] = attrib.vertices[vertexOffset + 1];
                v.position[2] = attrib.vertices[vertexOffset + 2];
                //                    std::cout << v.position[0] << "," << v.position[1] << "," << v.position[2] << std::endl;
                if (attrib.normals.size() > 0) {
                    v.normal[0] = attrib.normals[normalOffset];
                    v.normal[1] = attrib.normals[normalOffset + 1];
                    v.normal[2] = attrib.normals[normalOffset + 2];
                }
                
                
                if (attrib.texcoords.size() > 0) {
                    v.texCoord[0] = attrib.texcoords[texCoordOffset];
                    v.texCoord[1] = 1.0 - attrib.texcoords[texCoordOffset + 1];
                }
                
                vertices.push_back(v);
                indices.push_back(vertices.size() - 1);
            }
        }
        
        if (materials[0].bump_texname != "") {
            for (int i = 0; i < vertices.size(); i += 3) {
                Cvec3f tangent;
                Cvec3f binormal;
                calcFaceTangent(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position, vertices[i].texCoord, vertices[i + 1].texCoord, vertices[i + 2].texCoord, tangent, binormal);
                
                vertices[i].tangent = tangent;
                vertices[i + 1].tangent = tangent;
                vertices[i + 2].tangent = tangent;
                
                vertices[i].binormal = binormal;
                vertices[i + 1].binormal = binormal;
                vertices[i + 2].binormal = binormal;
            }
        }
        
    } else {
        std::cout << err << std::endl;
        assert(false);
    }
}
