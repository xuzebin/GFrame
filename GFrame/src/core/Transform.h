#ifndef Transform_h
#define Transform_h

#include "RigidbodyTransform.h"

/**
 * Transform that manages position, rotation and scale of an object.
 * Calculating modelmatrix only when any of the position, rotation or scale changes.
 */
class Transform : public RigidBodyTransform {
    
private:
    Cvec3 scale;
    
    Matrix4 modelMatrix;
    
    void calculateMatrix() {
        rigidBodyMatrix = Matrix4::makeTranslation(position) * quatToMatrix(rotation);
        modelMatrix = rigidBodyMatrix * Matrix4::makeScale(scale);
        isMatrixDirty = false;
    }
    
public:
    
    Transform() : RigidBodyTransform(), scale(1.0, 1.0, 1.0) {}
    
    void setScale(Cvec3 scale) {
        this->scale = scale;
        isMatrixDirty = true;
    }
    
    const Cvec3& getScale() {
        return scale;
    }

    const Matrix4& getModelMatrix() {
        if (isMatrixDirty) {
            calculateMatrix(); //Calculate matrix only when Transform is changed.
        }
        return modelMatrix;
    }
    
    Transform& operator = (const Transform& t) {
        rotation = t.rotation;
        position = t.position;
        scale = t.scale;
        isMatrixDirty = t.isMatrixDirty;
        rigidBodyMatrix = t.rigidBodyMatrix;
        modelMatrix = t.modelMatrix;
        return *this;
    }
    
};

#endif /* Transform_h */
