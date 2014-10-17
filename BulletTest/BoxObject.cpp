#include "BoxObject.h"

BoxObject::BoxObject() : PhysicalObject() {
	min.x = min.y = min.z = 0;
	max.x = max.y = max.z = 0;

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
}

void BoxObject::BuildShape()
{
	collisionShape = new btBoxShape(
		btVector3(
			(width / 2) * PHYSICS_WORLD_SCALE,
			(height / 2) * PHYSICS_WORLD_SCALE,
			(length / 2) * PHYSICS_WORLD_SCALE
		)
	);
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