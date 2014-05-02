#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include "Camera.h"

class BoxObject: public BaseObject {
protected:
	glm::vec3 vertices[8];
	short indices[36];
public:
	BoxObject();
	~BoxObject();

	void Draw(Camera& camera);
};

#endif /* BOXOBJECT_H_ */
