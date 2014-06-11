#ifndef BASEOBJECT_H_
#define BASEOBJECT_H_

#define GLM_FORCE_RADIANS
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	virtual ~BaseObject();

	virtual void SetPosition(const glm::vec3& position, bool silent = false);
	virtual glm::vec3 GetPosition();

	virtual void SetEuler(const glm::vec3& euler, bool silent = false);
	virtual glm::vec3 GetEuler();

	virtual void SetScale(const glm::vec3& scale, bool silent = false);
	virtual glm::vec3 GetScale();

	void BuildWorld();
	glm::mat4 GetWorld();
};

#endif /* BASEOBJECT_H_ */
