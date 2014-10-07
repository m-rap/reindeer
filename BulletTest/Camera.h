#ifndef CAMERA_H_
#define CAMERA_H_

#include "BaseObject.h"

class Camera : public BaseObject {
protected:
	RDRMAT4 view, projection;
public:
	Camera();
	virtual ~Camera();

	void BuildProjection();
	void BuildView();

	RDRMAT4* GetView();
	RDRMAT4* GetProjection();

	virtual void BuildWorld();
};

#endif /* CAMERA_H_ */
