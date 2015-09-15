#include "RdrMesh.h"
#include "objloader.h"
#ifdef USE_D3D9
#include "D3d9Renderer.h"
#else
#endif

RdrMesh::RdrMesh(bool isIndexed, bool useTexture)
{
    this->isIndexed = isIndexed;
	this->useTexture = useTexture;
}

RdrMesh::~RdrMesh()
{
}
