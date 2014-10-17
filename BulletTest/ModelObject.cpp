#include "ModelObject.h"
#include "objloader.h"

ModelObject::ModelObject(const char* modelPath) : PhysicalObject()
{
#ifdef USE_D3D9
	renderer = new D3d9ModelRenderer(this);
#else
	renderer = new OpenGLModelRenderer(this);
#endif
	LoadModel(modelPath);
	BuildRigidBody();
	renderer->BuildBuffers();
}

ModelObject::~ModelObject(void)
{
	delete renderer;
	DeleteRigidBody();
}

void ModelObject::BuildShape()
{
	collisionShape = new btConvexHullShape(&vertices[0].x, vertices.size(), sizeof(RDRVEC3));
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

void ModelObject::Draw(Camera* camera)
{
	renderer->Draw(camera);
}