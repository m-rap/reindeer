#include "ModelObject.h"
#include "objloader.h"

ModelObject::ModelObject(const char* modelPath) : BaseObject()
{
	renderer = new OpenGLModelRenderer(this);
	LoadModel(modelPath);
	renderer->BuildBuffers();
}

ModelObject::~ModelObject(void)
{
	delete renderer;
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