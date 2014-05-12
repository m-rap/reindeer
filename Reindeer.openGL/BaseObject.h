#ifndef BASEOBJECT_H_
#define BASEOBJECT_H_

#define GLM_FORCE_RADIANS
#include <math.h>
#include <GL/gl.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/vec3.hpp>
#include <GL/glm/mat4x4.hpp>
#include <GL/glm/gtc/quaternion.hpp>
#include <GL/glm/gtx/quaternion.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>

#include <stdio.h>

#define ToRadian( degree ) ((degree) * (M_PI / 180.0f))
#define ToDegree( radian ) ((radian) * (180.0f / M_PI))

const glm::vec4 VECTOR_UP(0, 1, 0, 0);

class BaseObject {
protected:
	glm::vec3 position;
	glm::vec3 euler;
	glm::vec3 scale;
	glm::quat quaternion;

	glm::mat4 rotationMatrix;
	glm::mat4 world;
public:
	BaseObject();
	~BaseObject();

	virtual void SetPosition(const glm::vec3& position, bool silent = false);
	virtual glm::vec3 GetPosition();

	virtual void SetEuler(const glm::vec3& euler, bool silent = false);
	virtual glm::vec3 GetEuler();

	void BuildWorld();
};

#endif /* BASEOBJECT_H_ */
