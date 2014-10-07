#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include "Camera.h"
#include "Drawable.h"
#include "BoxRenderer.h"

#ifdef USE_D3D9
#include "D3d9BoxRenderer.h"
#else
#include "OpenGLBoxRenderer.h"
#include "LegacyOpenGLBoxRenderer.h"
#endif

using namespace std;

class BoxObject :
    public BaseObject,
    public Drawable
{
protected:
	RDRVEC3 min, max;
	float width, height, length;
	BoxRenderer* boxRenderer;

	void BuildWidthHeightLength();
	void BuildRigidBody();
	void DeleteRigidBody();
	void SetRigidBodyTransform();
	void BoxShapeDrawer(Camera* camera);
public:
	BoxObject();
	virtual ~BoxObject();

    btRigidBody* rigidBody;
    btCollisionShape* collisionShape;
    btDiscreteDynamicsWorld* dynamicsWorld;

    virtual void SetPosition(const RDRVEC3& position, bool silent = false)
    {
    	BaseObject::SetPosition(position, silent);
    	SetRigidBodyTransform();
    }

	virtual void SetEuler(const RDRVEC3& euler, bool silent = false)
	{
		BaseObject::SetEuler(euler, silent);
		SetRigidBodyTransform();
	}

	void SetMin(const RDRVEC3& min);
	void SetMax(const RDRVEC3& max);
	void SetMinMax(const RDRVEC3& min, const RDRVEC3& max);

	RDRVEC3 GetMin();
	RDRVEC3 GetMax();
    float GetWidth();
    float GetHeight();
    float GetLength();

#ifdef USE_OPENGL
	void SetProgramId(const GLuint& programId);
#endif
	void SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	virtual void Draw(Camera* camera);
	void Update();
};

#endif /* BOXOBJECT_H_ */
