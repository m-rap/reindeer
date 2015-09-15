#ifndef RDRMESHFACTORY_OPENGL_H
#define RDRMESHFACTORY_OPENGL_H

#include "RdrMeshFactory.h"
#include "RdrMesh_OpenGL.h"

class RdrMeshFactory_OpenGL :
    public RdrMeshFactory
{
private:
    RdrMeshFactory_OpenGL() {}
    RdrMeshFactory_OpenGL(const RdrMeshFactory_OpenGL&);
    void operator=(const RdrMeshFactory_OpenGL&);

public:
    virtual ~RdrMeshFactory_OpenGL();

    static RdrMeshFactory_OpenGL& GetInstance()
    {
        static RdrMeshFactory_OpenGL instance;
        return instance;
    }

    static unordered_map<string, RdrMesh*> Cache;

	virtual RdrMesh* CreateBoxMesh(const RDRVEC3& min, const RDRVEC3& max);
	virtual RdrMesh* CreateBoxMesh(const float& width, const float& height, const float& length);
	virtual RdrMesh* CreateComplexMesh(const char* path);
};

#endif // RDRMESHFACTORY_OPENGL_H
