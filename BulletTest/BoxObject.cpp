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
            btQuaternion(0, 0, 0, 1),
            btVector3(0, 0, 0)
            //btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w),
            //btVector3(position.x, position.y, position.z)
        )
    );

    btCollisionShape* shape = new btBoxShape(btVector3(width / 2, height / 2, length / 2));

    float mass = 1;
    btVector3 inertia;
    shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, inertia);
    constructionInfo.m_friction = 10.0;
    constructionInfo.m_restitution = 0.0;

    if (dynamicsWorld != NULL)
        dynamicsWorld->removeRigidBody(rigidBody);
    DeleteRigidBody();

    rigidBody = new btRigidBody(constructionInfo);
    SetRigidBodyTransform();

    if (dynamicsWorld != NULL)
        dynamicsWorld->addRigidBody(rigidBody);
}

void BoxObject::SetRigidBodyTransform()
{
	btTransform tr;
    tr.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
    tr.setOrigin(btVector3(position.x, position.y, position.z));
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
    BuildRigidBody();
}

void BoxObject::Update()
{
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    btVector3 origin = trans.getOrigin();
    btQuaternion rot = trans.getRotation();
    this->position.x = origin.getX();
    this->position.y = origin.getY();
    this->position.z = origin.getZ();
    this->quaternion = glm::quat(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
    BuildWorld();
}
