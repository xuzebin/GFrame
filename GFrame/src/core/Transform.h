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

    //the offset to the center that the object rotate around
    Cvec3 pivot;
    
    void calculateMatrix() {
        rigidBodyMatrix = Matrix4::makeTranslation(position) * Matrix4::makeTranslation(pivot) * quatToMatrix(rotation) * Matrix4::makeTranslation(-pivot);
        modelMatrix = rigidBodyMatrix * Matrix4::makeScale(scale);
        isMatrixDirty = false;
    }
    
public:
    
    Transform() : RigidBodyTransform(), scale(1.0, 1.0, 1.0), pivot(0, 0, 0) {}
    
    void setScale(Cvec3 scale) {
        this->scale = scale;
        isMatrixDirty = true;
    }

    void setModelMatrix(const Matrix4& m) {
        for (int i = 0; i < 16; i++) {
            modelMatrix[i] = m[i];
        }
    }

    void setPivot(double x, double y, double z) {
        pivot[0] = x;
        pivot[1] = y;
        pivot[2] = z;
        isMatrixDirty = true;
    }

    const Cvec3& getPivot() const {
        return pivot;
    }
    
    const Cvec3& getScale() const {
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
