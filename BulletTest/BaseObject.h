#ifndef BASEOBJECT_H_
#define BASEOBJECT_H_

#include "reindeer.h"

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
	virtual void SetEuler(const glm::vec3& euler, bool silent = false);
	virtual void SetScale(const glm::vec3& scale, bool silent = false);

	virtual glm::vec3* GetPosition();
	virtual glm::vec3* GetEuler();
	virtual glm::vec3* GetScale();
	glm::mat4* GetWorld();

	virtual void BuildWorld();
};

#endif /* BASEOBJECT_H_ */
