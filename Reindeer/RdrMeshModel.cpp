#include "RdrMeshModel.h"

RdrMeshModel::RdrMeshModel(RdrMesh* parent, bool isIndexed, bool useTexture)
{
	this->parent = parent;
	this->isIndexed = isIndexed;
	this->useTexture = useTexture;
}


RdrMeshModel::~RdrMeshModel(void)
{
}
