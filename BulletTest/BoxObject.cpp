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
	string key = "box " + to_string((long double)width) + " " + to_string((long double)height) + " " + to_string((long double)length);
	if (CollisionShapes.find(key) == CollisionShapes.end())
	{
		CollisionShapes[key] = new btBoxShape(
			btVector3(
				(width / 2)  * PHYSICS_WORLD_SCALE,
				(height / 2) * PHYSICS_WORLD_SCALE,
				(length / 2) * PHYSICS_WORLD_SCALE
			)
		);
	}

	collisionShape = CollisionShapes[key];
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
#endif

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