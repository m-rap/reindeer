#include "BoxObject.h"

BoxObject::BoxObject() : BaseObject() {
	min.x = min.y = min.z = 0;
	max.x = max.y = max.z = 0;

	rigidBody = NULL;
	dynamicsWorld = NULL;

	if (GLEW_VERSION_1_5)
	{
		boxRenderer = new OpenGLBoxRenderer(this);
	}
	else
	{
		boxRenderer = new LegacyOpenGLBoxRenderer(this);
	}

	boxRenderer->BuildNormalsIndices();
	BuildWidthHeightLength();
}

BoxObject::~BoxObject() {
	delete boxRenderer;
	DeleteRigidBody();
}

void BoxObject::SetMin(const glm::vec3& value) {
	this->min = value;
	BuildWidthHeightLength();
}

glm::vec3 BoxObject::GetMin() {
	return min;
}

void BoxObject::SetMax(const glm::vec3& value) {
	this->max = value;
	BuildWidthHeightLength();
}


glm::vec3 BoxObject::GetMax() {
	return max;
}

void BoxObject::SetMinMax(const glm::vec3& min, const glm::vec3& max) {
	this->min = min;
	this->max = max;
	BuildWidthHeightLength();
}

float BoxObject::GetWidth()
{
	return width;
}

float BoxObject::GetHeight()
{
	return height;
}

float BoxObject::GetLength()
{
	return length;
}

void BoxObject::SetProgramId(const GLuint& programId) {
    boxRenderer->SetProgramId(programId);
}

void BoxObject::Draw(Camera* camera) {
    Update();
    boxRenderer->Draw(camera);
}

void BoxObject::BuildWidthHeightLength() {
	width  = max.x - min.x;
	height = max.y - min.y;
	length = max.z - min.z;

	BuildRigidBody();
	boxRenderer->BuildBuffers();
}

void BoxObject::BuildRigidBody()
{
    btMotionState* motionState = new btDefaultMotionState(
        btTransform(
            btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
            btVector3(0.0f, 0.0f, 0.0f)
        )
    );

    btCollisionShape* shape = new btBoxShape(
		btVector3(
			(width / 2) * PHYSICS_WORLD_SCALE,
			(height / 2) * PHYSICS_WORLD_SCALE,
			(length / 2) * PHYSICS_WORLD_SCALE
		)
	);

    float mass = 1.0f;
    btVector3 inertia;
    shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, inertia);
    constructionInfo.m_friction = 2.0f;
    constructionInfo.m_restitution = 0.5f;

    DeleteRigidBody();
    rigidBody = new btRigidBody(constructionInfo);
    SetRigidBodyTransform();
}

void BoxObject::SetRigidBodyTransform()
{
	btTransform tr;
    tr.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
    tr.setOrigin(btVector3(position.x * PHYSICS_WORLD_SCALE, position.y * PHYSICS_WORLD_SCALE, position.z * PHYSICS_WORLD_SCALE));
    rigidBody->setCenterOfMassTransform(tr);
}

void BoxObject::DeleteRigidBody()
{
	if (rigidBody != NULL)
    {
        delete rigidBody->getMotionState();
        delete rigidBody;
    }
}

void BoxObject::SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->dynamicsWorld = dynamicsWorld;
}

void BoxObject::Update()
{
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    btVector3 origin = trans.getOrigin();
    btQuaternion rot = trans.getRotation();
    this->position.x = origin.getX() / PHYSICS_WORLD_SCALE;
    this->position.y = origin.getY() / PHYSICS_WORLD_SCALE;
    this->position.z = origin.getZ() / PHYSICS_WORLD_SCALE;
    this->quaternion = glm::quat(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
    BuildWorld();
}
