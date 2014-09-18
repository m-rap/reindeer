#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include <iostream>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include "Camera.h"
#include "Drawable.h"

using namespace std;

class BoxObject :
    public BaseObject,
    public Drawable
{
protected:

	GLuint indices[36];

	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;

	GLuint programId;
	GLuint positionId;
	GLuint normalId;
	GLuint mvpId;
	GLuint matrixId;
	GLuint viewId;
	GLuint viewInvId;
	GLuint normalMatId;

	glm::vec3 min, max;
	float width, height, length;

	void BuildWidthHeightLength();
	void BuildBuffers();
	void BuildNormalsIndices();
	void BuildRigidBody();
	void DeleteRigidBody();
public:
	BoxObject();
	virtual ~BoxObject();

    btRigidBody* rigidBody;
    btDiscreteDynamicsWorld* dynamicsWorld;

	void SetMin(const glm::vec3& min);
	glm::vec3 GetMin();

	void SetMax(const glm::vec3& max);
	glm::vec3 GetMax();

	void SetMinMax(const glm::vec3& min, const glm::vec3& max);

	void SetProgramId(const GLuint& programId);

	void SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);

	virtual void Draw(Camera& camera);
	void Update();
};

#endif /* BOXOBJECT_H_ */
