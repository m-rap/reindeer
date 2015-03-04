#ifndef CAMERA_H_
#define CAMERA_H_

#include "BaseObject.h"

class Camera : public BaseObject {
protected:
	RDRMAT4 view, projection;
public:
	Camera();
	virtual ~Camera();

	virtual void BuildProjection(bool perspective = true);
	virtual void BuildView();

	virtual RDRMAT4* GetView();
	virtual RDRMAT4* GetProjection();

	virtual void BuildWorld();
};

#endif /* CAMERA_H_ */
