#ifndef BASEOBJECT_H_
#define BASEOBJECT_H_

#include "reindeer.h"

class BaseObject {
protected:
	RDRVEC3 position;
	RDRVEC3 scale;
	RDRQUAT rotation;

	RDRMAT4 rotationMatrix;
	RDRMAT4 world;

	virtual void TransformationChanged() {}

public:
	BaseObject();
	virtual ~BaseObject();

	virtual void SetPosition(const RDRVEC3& position, bool silent = false);
	virtual void SetRotation(const RDRQUAT& rotation, bool silent = false);
	virtual void SetScale(const RDRVEC3& scale, bool silent = false);

	void Rotate(const float& angle, const RDRVEC3& axis, bool silent = false);

	void RotateLocalX(const float& angle, bool silent = false);
	void RotateLocalY(const float& angle, bool silent = false);
	void RotateLocalZ(const float& angle, bool silent = false);

	void RotateGlobalX(const float& angle, bool silent = false);
	void RotateGlobalY(const float& angle, bool silent = false);
	void RotateGlobalZ(const float& angle, bool silent = false);

	virtual RDRVEC3* GetPosition();
	virtual RDRVEC3* GetScale();
	virtual RDRQUAT* GetRotation();
	RDRMAT4* GetWorld();

	virtual void BuildWorld();
};

#endif /* BASEOBJECT_H_ */
