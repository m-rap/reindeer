#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include "Camera.h"
#include "PhysicalObject.h"
#include "Drawable.h"
#include "BoxRenderer.h"

class BoxObject :
    public PhysicalObject,
	public Drawable
{
protected:
	RDRVEC3 min, max;
	float width, height, length;
	BoxRenderer* boxRenderer;

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

#ifdef USE_OPENGL
	void SetProgramId(const GLuint& programId);
#endif
	virtual void Draw(Camera* camera);
};

#endif /* BOXOBJECT_H_ */
