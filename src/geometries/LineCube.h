#ifndef LineCube_h
#define LineCube_h

#include "Geometry.hpp"

class LineCube : public Geometry {
public:
    
    LineCube(float size) {
        
        this->size = size;

        indicesNum = 0;
        boundingBoxLength = size;

        this->useIndex = false;
    }
    
    void createVBOs() {
        std::vector<Vertex> vtx(12);
        
        createCubeVertices(vtx);
        
        Geometry::createVBOs(vtx);
    }

protected:
    inline void createCubeVertices(std::vector<Vertex>& vtx) {
        //LEFT face
        vtx[0].position = Cvec3f(-1, -1, -1);
        vtx[1].position = Cvec3f(-1, -1, 1);
        vtx[2].position = Cvec3f(-1, 1, 1);

        vtx[3].position = Cvec3f(-1, -1, -1);
        vtx[4].position = Cvec3f(-1, 1, 1);
        vtx[5].position = Cvec3f(-1, 1, -1);        

        //RIGHT face
        vtx[6].position = Cvec3f(1, -1, -1);
        vtx[7].position = Cvec3f(1, -1, 1);
        vtx[8].position = Cvec3f(1, 1, 1);

        vtx[9].position = Cvec3f(1, -1, -1);
        vtx[10].position = Cvec3f(1, 1, 1);
        vtx[11].position = Cvec3f(1, 1, -1);

/*         //TOP face */
/*         vtx[8].position = Cvec3f(1, 1, 1); */
/*         vtx[9].position = Cvec3f(1, 1, -1); */
/*         vtx[10].position = Cvec3f(-1, 1, -1); */
/*         vtx[11].position = Cvec3f(-1, 1, 1); */

/*         //BOTTOM face */
/*         vtx[12].position = Cvec3f(1, -1, 1); */
/*         vtx[13].position = Cvec3f(1, -1, -1); */
/*         vtx[14].position = Cvec3f(-1, -1, -1); */
/*         vtx[15].position = Cvec3f(-1, -1, 1); */

        //FRONT face
/*         vtx[16].position = Cvec3f(1, -1, 1); */
/*         vtx[17].position = Cvec3f(1, 1, 1); */
/*         vtx[18].position = Cvec3f(-1, 1, 1); */
/*         vtx[19].position = Cvec3f(-1, -1, 1); */

/*         //REAR face */
/*         vtx[20].position = Cvec3f(1, -1, -1); */
/*         vtx[21].position = Cvec3f(1, 1, -1); */
/*         vtx[22].position = Cvec3f(-1, 1, -1); */
/*         vtx[23].position = Cvec3f(-1, -1, -1); */
    }

    float size;
};

#endif /* LineCube_h */
