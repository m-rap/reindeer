#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include "PhysicalObject.h"
#include "Drawable.h"
#include "BaseRenderer.h"

class BoxObject :
    public PhysicalObject,
	public Drawable
{
protected:
	RDRVEC3 min, max;
	float width, height, length;
	BaseRenderer* boxRenderer;

	virtual void BuildShape();
	void BuildWidthHeightLength();

public:
	BoxObject();
	virtual ~BoxObject();

	void SetMin(const RDRVEC3& min);
	void SetMax(const RDRVEC3& max);
	void SetMinMax(const RDRVEC3& min, const RDRVEC3& max);

	RDRVEC3 GetMin();
	RDRVEC3 GetMax();
    float GetWidth();
    float GetHeight();
    float GetLength();

	virtual void RenderShadow(Light* light);
	virtual void Draw(Camera* camera, Light* light);
};

#endif /* BOXOBJECT_H_ */
