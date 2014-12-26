#include "ModelObject.h"
#include "objloader.h"
#ifdef USE_D3D9
#include "D3d9Renderer.h"
#else
#include "OpenGLRenderer.h"
#include "LegacyOpenGLRenderer.h"
#endif

ModelObject::ModelObject(const char* modelPath) : PhysicalObject()
{
#ifdef USE_D3D9
	renderer = new D3d9Renderer(this, false, true);
#else
	if (!USE_LEGACY)
	{
		renderer = new OpenGLRenderer(this, false, true);
	}
	else
	{
		renderer = new LegacyOpenGLRenderer(this, false, true);
	}
#endif
	this->modelPath = (char*)modelPath;
	LoadModel(modelPath);
	BuildRigidBody();
	size_t uvCount = uvs.size();
	renderer->BuildBuffers(
		&vertices[0], &normals[0], NULL, &uvs[0],
		vertexCount, 0, uvCount
	);
}

ModelObject::~ModelObject(void)
{
	delete renderer;
}

void ModelObject::BuildShape()
{
	string key = "convex_hull " + string(modelPath);
	if (CollisionShapes.find(key) == CollisionShapes.end())
	{
		btScalar* vertices = new btScalar[vertexCount * 3];
		for (size_t i = 0; i < vertexCount; i++)
		{
			vertices[3 * i + 0] = this->vertices[i].x * PHYSICS_WORLD_SCALE;
			vertices[3 * i + 1] = this->vertices[i].y * PHYSICS_WORLD_SCALE;
			vertices[3 * i + 2] = this->vertices[i].z * PHYSICS_WORLD_SCALE;
		}
		CollisionShapes[key] = new btConvexHullShape(&vertices[0], vertexCount, 3 * sizeof(btScalar));
		delete[] vertices;
	}
	collisionShape = CollisionShapes[key];
}

void ModelObject::LoadModel(const char* modelPath)
{
	loadOBJ(modelPath, vertices, uvs, normals);
	vertexCount = vertices.size();
}

void ModelObject::RenderShadow(Light* light)
{
	renderer->RenderShadow(light);
}

void ModelObject::Draw(Camera* camera, Light* light)
{
	renderer->Draw(camera, light);
}