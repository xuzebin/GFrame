#ifndef Transform_hpp
#define Transform_hpp

#include "RigidbodyTransform.hpp"

/**
 * Transform that manages position, rotation and scale of an object.
 * Calculating modelmatrix only when any of the position, rotation or scale changes.
 */
class Transform : public RigidBodyTransform {
public:
    
    Transform();
    ~Transform();
    
    void setScale(const Cvec3& scale);
    void setModelMatrix(const Matrix4& m);
    void setPivot(double x, double y, double z);

    const Cvec3& getPivot() const { return pivot; }
    const Cvec3& getScale() const { return scale; }
    const Matrix4& getModelMatrix();
    
    Transform& operator = (const Transform& t);

private:
    void calculateMatrix();

    Cvec3 scale;
    
    Matrix4 modelMatrix;

    Cvec3 pivot;//the offset to the center that the object rotate around
};

#endif /* Transform_hpp */
