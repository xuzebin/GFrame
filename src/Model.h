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

class CallbackInterface {
public:
    virtual void setDiffuseTexture(std::string diffuseTexName) = 0;
};


class Model;


class Mesh : public Geometry {

private:
    const std::string fileName;
    std::string diffuseTexName;
    
    CallbackInterface* callback;
    
    void setDiffuseTexName(std::string diffuseTexName) {
        this->diffuseTexName = diffuseTexName;
        
        if (callback != NULL) {
            callback->setDiffuseTexture(diffuseTexName);
        }
    }
    
    void loadFromFile(const std::string& fileName, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
        
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
        
        if (materials.size() > 0) {
            setDiffuseTexName(materials[0].diffuse_texname);
        }
        
        
        for (int i = 0; i < materials.size(); ++i) {
            std::cout << materials[i].diffuse_texname << std::endl;
        }
        
        
        if (ret) {
            for (int i = 0; i < shapes.size(); ++i) {
                for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
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
                    
                    if (attrib.texcoords.size() > 0) {
                        v.texCoord[0] = attrib.texcoords[texCoordOffset];
                        v.texCoord[1] = 1.0 - attrib.texcoords[texCoordOffset + 1];
                    }
                    
                    vertices.push_back(v);
                    indices.push_back(vertices.size() - 1);
                }
            }
        } else {
            std::cout << err << std::endl;
            assert(false);
        }
//
//                if (ret) {
//                    for (int i = 0; i < attrib.vertices.size(); i += 3) {
//                        Vertex v;
//                        for (int j = 0; j < 3; ++j) {
//                            v.position[j] = attrib.vertices[i + j];
//                            v.normal[j]   = attrib.normals[i + j];
//                        }
//        
//                        vertices.push_back(v);
//                    }
//                    for (int i = 0; i < shapes.size(); ++i) {
//                        for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
//                            indices.push_back(shapes[i].mesh.indices[j].vertex_index);
//                        }
//                    }
//                } else {
//                    std::cout << err << std::endl;
//                    assert(false);
//                }
    }
    
    
public:
    
    Mesh(const std::string fileName_) : fileName(fileName_) {}
    
    void addCallback(CallbackInterface* callback) {
        this->callback = callback;
    }
    
    std::string getDiffuseTexName() {
        return diffuseTexName;
    }
    
    void createVBOs() {
        std::vector<Vertex> vtx;
        std::vector<unsigned short> idx;
        loadFromFile(fileName, vtx, idx);
        
        Geometry::createVBOs(vtx, idx);
    }
};

class Model : public Entity, public CallbackInterface {

private:
    void setDiffuseTexture(std::string diffuseTexName) {
        if (material == NULL) {
            throw std::string("material NULL");
        }
        material->diffuseTexture = loadGLTexture(diffuseTexName.c_str());
    }
    
public:
    Model(const std::string fileName) {
        geometry = new Mesh(fileName);
        material = new Material();
        
        ((Mesh*)geometry)->addCallback(this);
    }
};


#endif /* Model_h */

//class Model : public Geometry {
//private:
//    const std::string fileName;
//    std::string diffuseTexName;
//    
//    
//public:
//    Model(const std::string fileName_) : fileName(fileName_) {}
//    
//    void createVBOs() {
//        std::vector<Vertex> vertices;
//        std::vector<unsigned short> indices;
//        loadFromFile(fileName, vertices, indices);
//        
//        Geometry::createVBOs(vertices, indices);
//    }
//    std::string getDiffuseTexName() {
//        return diffuseTexName;
//    }
//
//    
//private:
//    
//    void setDiffuseTexName(std::string diffuseTexName) {
//        this->diffuseTexName = diffuseTexName;
//    }
//
//    
//    void loadFromFile(const std::string& fileName, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices) {
//        tinyobj::attrib_t attrib;
//        std::vector<tinyobj::shape_t> shapes;
//        std::vector<tinyobj::material_t> materials;
//        std::string err;
//        
//        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
//        
//        setDiffuseTexName(materials[0].diffuse_texname);
//        for (int i = 0; i < materials.size(); ++i) {
//            std::cout << materials[i].diffuse_texname << std::endl;
//        }
//
//        
//        if (ret) {
//            for (int i = 0; i < shapes.size(); ++i) {
//                for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
//                    unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
//                    unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
//                    unsigned int texCoordOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
//                    
//                    Vertex v;
//                    v.position[0] = attrib.vertices[vertexOffset];
//                    v.position[1] = attrib.vertices[vertexOffset + 1];
//                    v.position[2] = attrib.vertices[vertexOffset + 2];
//                    
//                    v.normal[0] = attrib.normals[normalOffset];
//                    v.normal[1] = attrib.normals[normalOffset + 1];
//                    v.normal[2] = attrib.normals[normalOffset + 2];
//                    
//                    if (attrib.texcoords.size() > 0) {
//                        v.texCoord[0] = attrib.texcoords[texCoordOffset];
//                        v.texCoord[1] = 1.0 - attrib.texcoords[texCoordOffset + 1];
//                    }
//                    
//                    vertices.push_back(v);
//                    indices.push_back(vertices.size() - 1);
//                }
//            }
//        } else {
//            std::cout << err << std::endl;
//            assert(false);
//        }
//        
////        if (ret) {
////            for (int i = 0; i < attrib.vertices.size(); i += 3) {
////                Vertex v;
////                for (int j = 0; j < 3; ++j) {
////                    v.position[j] = attrib.vertices[i + j];
////                    v.normal[j]   = attrib.normals[i + j];
////                }
////
////                vertices.push_back(v);
////            }
////            for (int i = 0; i < shapes.size(); ++i) {
////                for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) {
////                    indices.push_back(shapes[i].mesh.indices[j].vertex_index);
////                }
////            }
////        } else {
////            std::cout << err << std::endl;
////            assert(false);
////        }
//    }
//};
//
//
//#endif /* Model_h */
