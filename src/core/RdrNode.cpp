#include "RdrNode.h"

RdrNode::RdrNode()
{
    parent = NULL;
    Mesh = NULL;
}

RdrNode::~RdrNode()
{
    for (deque<RdrRigidBodyInfo*>::iterator it = RigidBodyInfos.begin(); it != RigidBodyInfos.end(); it++) {
        delete *it;
    }
}

void RdrNode::AddChild(RdrNode* child)
{
    children.push_back(child);
}

void RdrNode::RemoveChild(RdrNode* child)
{
    int i = Find(child);
    children.erase(children.begin() + i);
}

int RdrNode::Find(RdrNode* child)
{
    int i = 0;
    for (deque<RdrNode*>::iterator it = children.begin(); it != children.end(); it++, i++) {
        if (*it == child)
            return i;
    }
    return -1;
}

void RdrNode::AddRigidBodyInfo(btCollisionShape* shape, float mass)
{
    RdrRigidBodyInfo* rb = new RdrRigidBodyInfo();
    rb->shape = shape;
    rb->mass = mass;
    RigidBodyInfos.push_back(rb);
}

void RdrNode::Update(const btTransform& tr)
{
    btQuaternion rot = tr.getRotation();
    btVector3 origin = tr.getOrigin();
    this->tr.SetPosition(RDRVEC3(origin.getX() / PHYSICS_WORLD_SCALE, origin.getY() / PHYSICS_WORLD_SCALE, origin.getZ() / PHYSICS_WORLD_SCALE), true);
    this->tr.SetRotation(RDRQUAT(rot.getW(), rot.getX(), rot.getY(), rot.getZ()), true);
    this->tr.BuildWorld();
}

void RdrNode::Update(const RdrTransform& tr)
{
    this->tr = tr;
    this->tr.BuildWorld();
}

void RdrNode::RenderShadow(RdrTransform* baseTr, RdrLight* light)
{
    RdrTransform relTr;
    if (baseTr != NULL) {
        relTr = *baseTr;
        relTr.Translate(*tr.GetPosition());
        relTr.Rotate(*tr.GetRotation());
    } else {
        relTr = tr;
    }
    if (Mesh != NULL)
        Mesh->RenderShadow(&relTr, light);

    for (deque<RdrNode*>::iterator it = children.begin(); it != children.end(); it++) {
        (*it)->RenderShadow(&relTr, light);
    }
}

void RdrNode::Draw(RdrTransform* baseTr, RdrCamera* camera, RdrLight* light)
{
    RdrTransform relTr;
    if (baseTr != NULL) {
        relTr = *baseTr;
        relTr.Translate(*tr.GetPosition());
        relTr.Rotate(*tr.GetRotation());
    } else {
        relTr = tr;
    }
    //printf("%f %f %f\n", relTr.GetPosition()->x, relTr.GetPosition()->y, relTr.GetPosition()->z);
    if (Mesh != NULL)
        Mesh->Draw(&relTr, camera, light);

    for (deque<RdrNode*>::iterator it = children.begin(); it != children.end(); it++) {
        (*it)->Draw(&relTr, camera, light);
    }
}
