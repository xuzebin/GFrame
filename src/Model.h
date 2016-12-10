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
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Texture.hpp"
#include "Mesh.hpp"


class Model : public Entity {
    
public:
    
    Model(const std::string fileName, std::string name) {
        this->name = name;
        
        material = new Material();
        
        std::vector<Vertex> vtx;
        std::vector<unsigned short> idx;
        loadFromFile(fileName, vtx, idx);
        geometry = new Mesh(vtx, idx);

    }
    
    void calcFaceTangent(const Cvec3f& v1, const Cvec3f& v2, const Cvec3f& v3, const Cvec2f& texcoord1, const Cvec2f& texcoord2, const Cvec2f& texcoord3, Cvec3f& tangent, Cvec3f& binormal) {
        
        Cvec3f side0 = v1 - v2;
        Cvec3f side1 = v3 - v1;
        Cvec3f normal = cross(side1, side0);
        normalize(normal);
        
        float deltaV0 = texcoord1[1] - texcoord2[1];
        float deltaV1 = texcoord3[1] - texcoord1[1];
        tangent = side0 * deltaV1 - side1 * deltaV0;
        normalize(tangent);
        
        float deltaU0 = texcoord1[0] - texcoord2[0];
        float deltaU1 = texcoord3[0] - texcoord1[0];
        binormal = side0 * deltaU1 - side1 * deltaU0;
//        normalize(binormal);

        Cvec3f tangentCross = cross(tangent, binormal);
        if (dot(tangentCross, normal) < 0.0f) {
            tangent = tangent * -1;
        }
    }
    
    void loadFromFile(const std::string& fileName, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
        
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);

        material->setDiffuseTexture(materials[0].diffuse_texname);
        material->setSpecularTexture(materials[0].specular_texname);
        material->setNormalTexture(materials[0].normal_texname);
        
        if (ret) {
            for (int i = 0; i < shapes.size(); i++) {
                for (int j = 0; j < shapes[i].mesh.indices.size(); j++) {
                    unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
                    unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
                    unsigned int texCoordOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
                    
                    Vertex v;
                    v.position[0] = attrib.vertices[vertexOffset];
                    v.position[1] = attrib.vertices[vertexOffset + 1];
                    v.position[2] = attrib.vertices[vertexOffset + 2];
                    v.normal[0] = attrib.normals[normalOffset];
                    v.normal[1] = attrib.normals[normalOffset + 1];
                    v.normal[2] = attrib.normals[normalOffset + 2];
                    v.texCoord[0] = attrib.texcoords[texCoordOffset];
                    v.texCoord[1] = 1.0 - attrib.texcoords[texCoordOffset + 1];
                    
                    vertices.push_back(v);
                    indices.push_back(vertices.size() - 1);
                }
            }
            
            for (int i = 0; i < vertices.size(); i += 3) {
                Cvec3f tangent;
                Cvec3f binormal;
                calcFaceTangent(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position,
                                vertices[i].texCoord, vertices[i + 1].texCoord, vertices[i + 2].texCoord, tangent, binormal);
                
                vertices[i].tangent = tangent;
                vertices[i + 1].tangent = tangent;
                vertices[i + 2].tangent = tangent;
                
                vertices[i].binormal = binormal;
                vertices[i + 1].binormal = binormal;
                vertices[i + 2].binormal = binormal;
            }
            
        } else {
            std::cout << err << std::endl;
            assert(false);
        }
    }

};


#endif /* Model_h */
