#include "ModelObject.h"
#include "objloader.h"

ModelObject::ModelObject(const char* modelPath) : BaseObject()
{
	rigidBody = NULL;
	collisionShape = NULL;
	dynamicsWorld = NULL;

	renderer = new OpenGLModelRenderer(this);
	LoadModel(modelPath);
	BuildRigidBody();
	renderer->BuildBuffers();
}

ModelObject::~ModelObject(void)
{
	delete renderer;
	DeleteRigidBody();
}

#ifdef USE_OPENGL
void ModelObject::SetProgramId(GLuint programId)
{
	renderer->SetProgramId(programId);
}
#endif

void ModelObject::LoadModel(const char* modelPath)
{
	loadOBJ(modelPath, vertices, uvs, normals);
}

void ModelObject::BuildRigidBody()
{
	btTransform tr;
    tr.setIdentity();
    tr.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
    tr.setOrigin(btVector3(position.x * PHYSICS_WORLD_SCALE, position.y * PHYSICS_WORLD_SCALE, position.z * PHYSICS_WORLD_SCALE));

    btDefaultMotionState* motionState = new btDefaultMotionState(tr);

    DeleteRigidBody();

	btScalar* verts = new btScalar[vertices.size() * 3];
	for (size_t i = 0; i < vertices.size(); i++)
	{
		verts[i * 3] = vertices[i].x;
		verts[i * 3 + 1] = vertices[i].y;
		verts[i * 3 + 2] = vertices[i].z;
	}
    collisionShape = new btConvexHullShape(&vertices[0].x, vertices.size(), sizeof(RDRVEC3));
	delete[] verts;

    float mass = 1.0f;
    btVector3 inertia;
    collisionShape->calculateLocalInertia(mass, inertia);
    btBoxShape* temp = ((btBoxShape*)collisionShape);

    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape, inertia);
    constructionInfo.m_friction = 1.0f;
    constructionInfo.m_restitution = 0.0f;
    rigidBody = new btRigidBody(constructionInfo);
}

void ModelObject::SetRigidBodyTransform()
{
	btTransform tr;
	tr.setIdentity();
    tr.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
    tr.setOrigin(btVector3(position.x * PHYSICS_WORLD_SCALE, position.y * PHYSICS_WORLD_SCALE, position.z * PHYSICS_WORLD_SCALE));
    rigidBody->setCenterOfMassTransform(tr);
}

void ModelObject::DeleteRigidBody()
{
	if (rigidBody != NULL)
    {
        delete rigidBody->getMotionState();
        delete rigidBody;
    	delete collisionShape;
    }
}

void ModelObject::SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->dynamicsWorld = dynamicsWorld;
}

void ModelObject::Update()
{
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);

    btQuaternion rot = trans.getRotation();
#ifdef USE_D3D9
    this->quaternion = RDRQUAT(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
#else
	this->quaternion = RDRQUAT(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
#endif

    btVector3 origin = trans.getOrigin();
    this->position.x = origin.getX() / PHYSICS_WORLD_SCALE;
    this->position.y = origin.getY() / PHYSICS_WORLD_SCALE;
    this->position.z = origin.getZ() / PHYSICS_WORLD_SCALE;

    BuildWorld();
}

void ModelObject::Draw(Camera* camera)
{
	renderer->Draw(camera);
}
