#ifndef RDRCOMPONENT_H
#define RDRCOMPONENT_H

#include <deque>
#include "btBulletCollisionCommon.h"
#include "RdrTransform.h"
#include "RdrMesh.h"

using namespace std;

struct RdrRigidBodyInfo
{
    float mass;
    btCollisionShape* shape;
};

class RdrNode
{
protected:
    deque<RdrNode*> children;
    RdrNode* parent;
    RdrTransform tr;

public:
    RdrNode();
    virtual ~RdrNode();

    deque<RdrRigidBodyInfo*> RigidBodyInfos;
    RdrMesh* Mesh;

    RdrTransform* GetTransform() { return &tr; }
    RdrNode* GetParent() { return parent; }
    deque<RdrNode*>* GetChildren() { return &children; }

    void AddChild(RdrNode* child);
    void RemoveChild(RdrNode* child);
    int Find(RdrNode* child);

    void AddRigidBodyInfo(btCollisionShape* shape, float mass);

    void Update(const btTransform& tr);
    void Update(const RdrTransform& tr);

    void RenderShadow(RdrTransform* baseTr, RdrLight* light);
    void Draw(RdrTransform* baseTr, RdrCamera* camera, RdrLight* light);
};

#endif // RDRCOMPONENT_H
