#ifndef RDRCOLLISIONSHAPEFACTORY_H
#define RDRCOLLISIONSHAPEFACTORY_H

#include "reindeer.h"
#include "RdrNode.h"

class RdrCollisionShapeFactory
{
private:
    RdrCollisionShapeFactory() {}
    RdrCollisionShapeFactory(const RdrCollisionShapeFactory&);
    void operator=(const RdrCollisionShapeFactory&);

public:
    virtual ~RdrCollisionShapeFactory();

    static RdrCollisionShapeFactory& GetInstance()
    {
        static RdrCollisionShapeFactory instance;
        return instance;
    }

    static unordered_map<string, btCollisionShape*> Cache;
    static unordered_map<RdrNode*, btCompoundShape*> CompoundShapes;

    btCollisionShape* CreateBoxShape(const RDRVEC3& min, const RDRVEC3& max);
    btCollisionShape* CreateBoxShape(const float& width, const float& height, const float& length);
    btCollisionShape* CreateComplexShape(const char* path);

    static btCompoundShape* CreateCompoundShape(RdrNode* composite);
    static void ClearCompoundShapes();
};

#endif // RDRCOLLISIONSHAPEFACTORY_H
