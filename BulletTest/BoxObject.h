#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include "Camera.h"
#include "Drawable.h"
#include "BoxRenderer.h"
#include "OpenGLBoxRenderer.h"
#include "LegacyOpenGLBoxRenderer.h"

using namespace std;

class BoxObject :
    public BaseObject,
    public Drawable
{
protected:
	glm::vec3 min, max;
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

    virtual void SetPosition(const glm::vec3& position, bool silent = false)
    {
    	BaseObject::SetPosition(position, silent);
    	SetRigidBodyTransform();
    }

	virtual void SetEuler(const glm::vec3& euler, bool silent = false)
	{
		BaseObject::SetEuler(euler, silent);
		SetRigidBodyTransform();
	}

	void SetMin(const glm::vec3& min);
	void SetMax(const glm::vec3& max);
	void SetMinMax(const glm::vec3& min, const glm::vec3& max);

	glm::vec3 GetMin();
	glm::vec3 GetMax();
    float GetWidth();
    float GetHeight();
    float GetLength();

	void SetProgramId(const GLuint& programId);
	void SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	virtual void Draw(Camera* camera);
	void Update();
};

#endif /* BOXOBJECT_H_ */
