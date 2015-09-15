#ifndef RDRMESHFACTORY_H
#define RDRMESHFACTORY_H

#include "RdrMesh.h"

class RdrMeshFactory
{
public:
	virtual RdrMesh* CreateBoxMesh(const RDRVEC3& min, const RDRVEC3& max) = 0;
	virtual RdrMesh* CreateBoxMesh(const float& width, const float& height, const float& length) = 0;
	virtual RdrMesh* CreateComplexMesh(const char* path) = 0;
};

#endif // RDRMESHFACTORY_H
