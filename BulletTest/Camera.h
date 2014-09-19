#ifndef CAMERA_H_
#define CAMERA_H_

#include "BaseObject.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Camera : public BaseObject {
protected:
	glm::mat4 view, projection;
public:
	Camera();
	virtual ~Camera();

	void BuildProjection();
	void BuildView();

	glm::mat4* GetView();
	glm::mat4* GetProjection();

	virtual void BuildWorld();
};

#endif /* CAMERA_H_ */
