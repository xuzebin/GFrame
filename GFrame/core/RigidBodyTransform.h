#ifndef RigidBodyTransform_h
#define RigidBodyTransform_h

#include "../base/quat.h"
#include "../base/cvec.h"
#include "../base/matrix4.h"

/**
 * Rigid Body Transform (RBT) excludes scaling.
 * Preserve dot products between vectors, handedness of basis and distance between points.
 */
class RigidBodyTransform {
    
protected:
    Quat rotation;
    Cvec3 position;
    
    bool isMatrixDirty;
    
    Matrix4 rigidBodyMatrix;
    
    void calculateMatrix() {
        rigidBodyMatrix = Matrix4::makeTranslation(position) * quatToMatrix(rotation);
        isMatrixDirty = false;
    }
    
public:
    
    RigidBodyTransform() : isMatrixDirty(true) {}
    
    void setRotation(Quat rotation) {
        this->rotation = rotation;
        isMatrixDirty = true;
    }
    void setPosition(Cvec3 position) {
        this->position = position;
        isMatrixDirty = true;
    }
    
    void translate(Cvec3 translation) {
        this->position += translation;
        isMatrixDirty = true;
    }
    void rotate(Quat rotation) {
        this->rotation = this->rotation * rotation;
        isMatrixDirty = true;
    }
    
    const Cvec3& getPosition() {
        return position;
    }
    const Quat& getRotation() {
        return rotation;
    }
    
    const Matrix4& getRigidBodyMatrix() {
        if (isMatrixDirty) {
            calculateMatrix();
        }
        return rigidBodyMatrix;
    }
    
    RigidBodyTransform& operator = (const RigidBodyTransform& t) {
        rotation = t.rotation;
        position = t.position;
        isMatrixDirty = t.isMatrixDirty;
        rigidBodyMatrix = t.rigidBodyMatrix;
        return *this;
    }
    
};

#endif /* RigidBodyTransform_h */
