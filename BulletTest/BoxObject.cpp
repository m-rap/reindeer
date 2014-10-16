#include "BoxObject.h"

BoxObject::BoxObject() : BaseObject() {
	min.x = min.y = min.z = 0;
	max.x = max.y = max.z = 0;

	rigidBody = NULL;
	collisionShape = NULL;
	dynamicsWorld = NULL;

#ifdef USE_D3D9
	boxRenderer = new D3d9BoxRenderer(this);
#else
	if (GLEW_VERSION_1_5)
	{
		boxRenderer = new OpenGLBoxRenderer(this);
	}
	else
	{
		boxRenderer = new LegacyOpenGLBoxRenderer(this);
	}
#endif

	boxRenderer->BuildNormalsIndices();
	BuildWidthHeightLength();
}

BoxObject::~BoxObject() {
	delete boxRenderer;
	DeleteRigidBody();
}

void BoxObject::SetMin(const RDRVEC3& value) {
	this->min = value;
	BuildWidthHeightLength();
}

RDRVEC3 BoxObject::GetMin() {
	return min;
}

void BoxObject::SetMax(const RDRVEC3& value) {
	this->max = value;
	BuildWidthHeightLength();
}


RDRVEC3 BoxObject::GetMax() {
	return max;
}

void BoxObject::SetMinMax(const RDRVEC3& min, const RDRVEC3& max) {
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

#ifdef USE_OPENGL
void BoxObject::SetProgramId(const GLuint& programId) {
    boxRenderer->SetProgramId(programId);
}

inline void glDrawVector(const btVector3& v) { glVertex3d(v[0], v[1], v[2]); }
#endif

void BoxObject::BoxShapeDrawer(Camera* camera)
{
#ifdef USE_OPENGL
	glm::mat4& projection = *camera->GetProjection();
    glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glm::mat4& view = *camera->GetView();
	glm::mat4 modelview = view * glm::mat4();
	glLoadMatrixf(glm::value_ptr(modelview));

    btBoxShape* boxShape = (btBoxShape*)rigidBody->getCollisionShape();

    btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
    btVector3 org(world[3][0], world[3][1], world[3][2]);
    btVector3 dx(world[0][0], world[0][1], world[0][2]);
	btVector3 dy(world[1][0], world[1][1], world[1][2]);
	btVector3 dz(world[2][0], world[2][1], world[2][2]);
	dx *= halfExtent[0];
	dy *= halfExtent[1];
	dz *= halfExtent[2];
	glBegin(GL_LINE_LOOP);
	glDrawVector(org - dx - dy - dz);
	glDrawVector(org + dx - dy - dz);
	glDrawVector(org + dx + dy - dz);
	glDrawVector(org - dx + dy - dz);
	glDrawVector(org - dx + dy + dz);
	glDrawVector(org + dx + dy + dz);
	glDrawVector(org + dx - dy + dz);
	glDrawVector(org - dx - dy + dz);
	glEnd();
	glBegin(GL_LINES);
	glDrawVector(org + dx - dy - dz);
	glDrawVector(org + dx - dy + dz);
	glDrawVector(org + dx + dy - dz);
	glDrawVector(org + dx + dy + dz);
	glDrawVector(org - dx - dy - dz);
	glDrawVector(org - dx + dy - dz);
	glDrawVector(org - dx - dy + dz);
	glDrawVector(org - dx + dy + dz);
	glEnd();
#endif
}

void BoxObject::Draw(Camera* camera) {
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
	btTransform tr;
    tr.setIdentity();
    tr.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
    tr.setOrigin(btVector3(position.x * PHYSICS_WORLD_SCALE, position.y * PHYSICS_WORLD_SCALE, position.z * PHYSICS_WORLD_SCALE));

    btDefaultMotionState* motionState = new btDefaultMotionState(tr);

    DeleteRigidBody();

    collisionShape = new btBoxShape(
		btVector3(
			(width / 2) * PHYSICS_WORLD_SCALE,
			(height / 2) * PHYSICS_WORLD_SCALE,
			(length / 2) * PHYSICS_WORLD_SCALE
		)
	);

    float mass = 1.0f;
    btVector3 inertia;
    collisionShape->calculateLocalInertia(mass, inertia);
    btBoxShape* temp = ((btBoxShape*)collisionShape);

    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape, inertia);
    constructionInfo.m_friction = 1.0f;
    constructionInfo.m_restitution = 0.0f;
    rigidBody = new btRigidBody(constructionInfo);
}

void BoxObject::SetRigidBodyTransform()
{
	btTransform tr;
	tr.setIdentity();
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
    	delete collisionShape;
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
