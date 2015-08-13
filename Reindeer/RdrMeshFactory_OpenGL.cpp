#include "RdrMeshFactory_OpenGL.h"
#include "objloader.h"

unordered_map<string, RdrMesh*> RdrMeshFactory_OpenGL::Cache;

RdrMeshFactory_OpenGL::~RdrMeshFactory_OpenGL()
{
    for (auto kv : Cache) {
        delete kv.second;
    }
}

RdrMesh* RdrMeshFactory_OpenGL::CreateBoxMesh(const RDRVEC3& min, const RDRVEC3& max)
{
    float width  = max.x - min.x;
    float height = max.y - min.y;
    float length = max.z - min.z;

    return CreateBoxMesh(width, height, length);
}

RdrMesh* RdrMeshFactory_OpenGL::CreateBoxMesh(const float& width, const float& height, const float& length)
{
    string key("box ");
	char buff[30];
	snprintf(buff, 30, "%f,", width);
	key += string(buff);
	snprintf(buff, 30, "%f,", height);
	key += string(buff);
	snprintf(buff, 30, "%f", length);
	key += string(buff);

	if (RdrMeshFactory_OpenGL::Cache.find(key) == RdrMeshFactory_OpenGL::Cache.end()) {

		RdrMesh* mesh = new RdrMesh_OpenGL(true);

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

        mesh->BuildBuffers(
            vertices, normals, indices, NULL,
            72, 36, 0
        );

        RdrMeshFactory_OpenGL::Cache[key] = mesh;
	}

	return RdrMeshFactory_OpenGL::Cache[key];
}

RdrMesh* RdrMeshFactory_OpenGL::CreateComplexMesh(const char* path)
{
    string key = string(path);
	if (RdrMeshFactory_OpenGL::Cache.find(key) == RdrMeshFactory_OpenGL::Cache.end()) {

		RdrMesh* mesh = new RdrMesh_OpenGL(false, true);

		vector<RDRVEC3> vertices;
		vector<RDRVEC2> uvs;
		vector<RDRVEC3> normals;
        loadOBJ(path, vertices, uvs, normals);

        mesh->BuildBuffers(
            &vertices[0], &normals[0], NULL, &uvs[0],
            vertices.size(), 0, uvs.size()
        );

        RdrMeshFactory_OpenGL::Cache[key] = mesh;
    }
	return RdrMeshFactory_OpenGL::Cache[key];
}
