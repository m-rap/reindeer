#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include "Camera.h"

class BoxObject: public BaseObject {
protected:
	GLfloat vertices[72];
	GLfloat normals[72];
	GLbyte indices[36];

	glm::vec3 min, max;
	float width, height, length;

	void BuildWidthHeightLength();
	void BuildBuffers();
	void BuildNormalsIndices();
public:
	BoxObject();
	virtual ~BoxObject();

	void SetMin(const glm::vec3& min);
	glm::vec3 GetMin();

	void SetMax(const glm::vec3& max);
	glm::vec3 GetMax();

	void SetMinMax(const glm::vec3& min, const glm::vec3& max);

	void Draw(Camera& camera);
};

#endif /* BOXOBJECT_H_ */
