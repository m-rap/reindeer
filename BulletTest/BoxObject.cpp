#include "BoxObject.h"
#ifdef USE_D3D9
#include "D3d9Renderer.h"
#else
#include "OpenGLRenderer.h"
#include "LegacyOpenGLRenderer.h"
#endif

BoxObject::BoxObject() : PhysicalObject() {
	min.x = min.y = min.z = 0;
	max.x = max.y = max.z = 0;

#ifdef USE_D3D9
	boxRenderer = new D3d9Renderer(this, true);
#else
	if (!USE_LEGACY)
	{
		boxRenderer = new OpenGLRenderer(this, true);
	}
	else
	{
		boxRenderer = new LegacyOpenGLRenderer(this, true);
	}
#endif

	BuildWidthHeightLength();
}

BoxObject::~BoxObject() {
	delete boxRenderer;
}

void BoxObject::BuildShape()
{
	string key("box ");
	char buff[50];
	snprintf(buff, 50, "%f ", width);
	key += string(buff);
	snprintf(buff, 50, "%f ", height);
	key += string(buff);
	snprintf(buff, 50, "%f", length);
	key += string(buff);

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

	RDRVEC3 vertices[] = {
		RDRVEC3(-width / 2, -height / 2,  length / 2),    // side 1
		RDRVEC3( width / 2, -height / 2,  length / 2),
		RDRVEC3(-width / 2,  height / 2,  length / 2),
		RDRVEC3( width / 2,  height / 2,  length / 2),
		
		RDRVEC3(-width / 2, -height / 2, -length / 2),    // side 2
		RDRVEC3(-width / 2,  height / 2, -length / 2),
		RDRVEC3( width / 2, -height / 2, -length / 2),
		RDRVEC3( width / 2,  height / 2, -length / 2),
		
		RDRVEC3(-width / 2,  height / 2, -length / 2),    // side 3
		RDRVEC3(-width / 2,  height / 2,  length / 2),
		RDRVEC3( width / 2,  height / 2, -length / 2),
		RDRVEC3( width / 2,  height / 2,  length / 2),
		
		RDRVEC3(-width / 2, -height / 2, -length / 2),    // side 4
		RDRVEC3( width / 2, -height / 2, -length / 2),
		RDRVEC3(-width / 2, -height / 2,  length / 2),
		RDRVEC3( width / 2, -height / 2,  length / 2),
		
		RDRVEC3( width / 2, -height / 2, -length / 2),    // side 5
		RDRVEC3( width / 2,  height / 2, -length / 2),
		RDRVEC3( width / 2, -height / 2,  length / 2),
		RDRVEC3( width / 2,  height / 2,  length / 2),
		
		RDRVEC3(-width / 2, -height / 2, -length / 2),    // side 6
		RDRVEC3(-width / 2, -height / 2,  length / 2),
		RDRVEC3(-width / 2,  height / 2, -length / 2),
		RDRVEC3(-width / 2,  height / 2,  length / 2),
	};

	unsigned short indices[] = {
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	RDRVEC3 normals[] = {
		 RDRVEC3( 0.0f,  0.0f,  1.0f),
		 RDRVEC3( 0.0f,  0.0f,  1.0f),
		 RDRVEC3( 0.0f,  0.0f,  1.0f),
		 RDRVEC3( 0.0f,  0.0f,  1.0f),

		 RDRVEC3( 0.0f,  0.0f, -1.0f),
		 RDRVEC3( 0.0f,  0.0f, -1.0f),
		 RDRVEC3( 0.0f,  0.0f, -1.0f),
		 RDRVEC3( 0.0f,  0.0f, -1.0f),

		 RDRVEC3( 0.0f,  1.0f,  0.0f),
		 RDRVEC3( 0.0f,  1.0f,  0.0f),
		 RDRVEC3( 0.0f,  1.0f,  0.0f),
		 RDRVEC3( 0.0f,  1.0f,  0.0f),

		 RDRVEC3( 0.0f, -1.0f,  0.0f),
		 RDRVEC3( 0.0f, -1.0f,  0.0f),
		 RDRVEC3( 0.0f, -1.0f,  0.0f),
		 RDRVEC3( 0.0f, -1.0f,  0.0f),

		 RDRVEC3( 1.0f,  0.0f,  0.0f),
		 RDRVEC3( 1.0f,  0.0f,  0.0f),
		 RDRVEC3( 1.0f,  0.0f,  0.0f),
		 RDRVEC3( 1.0f,  0.0f,  0.0f),

		 RDRVEC3(-1.0f,  0.0f,  0.0f),
		 RDRVEC3(-1.0f,  0.0f,  0.0f),
		 RDRVEC3(-1.0f,  0.0f,  0.0f),
		 RDRVEC3(-1.0f,  0.0f,  0.0f),
	};

	RDRVEC2* uvs = NULL;

	BuildRigidBody();
	boxRenderer->BuildBuffers(
		vertices, normals, indices, NULL,
		72, 36, 0
	);
}
