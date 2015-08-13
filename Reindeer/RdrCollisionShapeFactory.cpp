#include "RdrCollisionShapeFactory.h"
#include "objloader.h"

unordered_map<string, btCollisionShape*> RdrCollisionShapeFactory::Cache;
unordered_map<RdrNode*, btCompoundShape*> RdrCollisionShapeFactory::CompoundShapes;

RdrCollisionShapeFactory::~RdrCollisionShapeFactory()
{
    for (auto kv : Cache) {
        delete kv.second;
    }
}

btCollisionShape* RdrCollisionShapeFactory::CreateBoxShape(const RDRVEC3& min, const RDRVEC3& max)
{
    float width  = max.x - min.x;
    float height = max.y - min.y;
    float length = max.z - min.z;

    return CreateBoxShape(width, height, length);
}

btCollisionShape* RdrCollisionShapeFactory::CreateBoxShape(const float& width, const float& height, const float& length)
{
    string key("box ");
	char buff[30];
	snprintf(buff, 30, "%f,", width);
	key += string(buff);
	snprintf(buff, 30, "%f,", height);
	key += string(buff);
	snprintf(buff, 30, "%f", length);
	key += string(buff);

	if (RdrCollisionShapeFactory::Cache.find(key) == RdrCollisionShapeFactory::Cache.end()) {
        RdrCollisionShapeFactory::Cache[key] = new btBoxShape(
			btVector3(
				(width / 2)  * PHYSICS_WORLD_SCALE,
				(height / 2) * PHYSICS_WORLD_SCALE,
				(length / 2) * PHYSICS_WORLD_SCALE
			)
		);
	}

	return RdrCollisionShapeFactory::Cache[key];
}

btCollisionShape* RdrCollisionShapeFactory::CreateComplexShape(const char* path)
{
    string key = string(path);
    if (RdrCollisionShapeFactory::Cache.find(key) == RdrCollisionShapeFactory::Cache.end()) {

        vector<RDRVEC3> vertices;
		vector<RDRVEC2> uvs;
		vector<RDRVEC3> normals;
        loadOBJ(path, vertices, uvs, normals);

        size_t vertexCount = vertices.size();
        btScalar* btVertices = new btScalar[vertexCount * 3];
		for (size_t i = 0; i < vertexCount; i++)
		{
			btVertices[3 * i + 0] = vertices[i].x * PHYSICS_WORLD_SCALE;
			btVertices[3 * i + 1] = vertices[i].y * PHYSICS_WORLD_SCALE;
			btVertices[3 * i + 2] = vertices[i].z * PHYSICS_WORLD_SCALE;
		}
		RdrCollisionShapeFactory::Cache[key] = new btConvexHullShape(&btVertices[0], vertexCount, 3 * sizeof(btScalar));
		delete[] btVertices;
    }

    return RdrCollisionShapeFactory::Cache[key];
}

btCompoundShape* RdrCollisionShapeFactory::CreateCompoundShape(RdrNode* composite)
{
    unordered_map<RdrNode*, btCompoundShape*>::const_iterator it = RdrCollisionShapeFactory::CompoundShapes.find(composite);
    if (it != RdrCollisionShapeFactory::CompoundShapes.end())
        delete it->second;

    btCompoundShape* cs = new btCompoundShape();
    RdrCollisionShapeFactory::CompoundShapes[composite] = cs;
    return cs;
}

void RdrCollisionShapeFactory::ClearCompoundShapes()
{
    RdrCollisionShapeFactory::CompoundShapes.clear();
}
