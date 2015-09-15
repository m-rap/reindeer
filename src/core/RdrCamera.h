#ifndef RDRCAMERA_H_
#define RDRCAMERA_H_

#include "RdrNode.h"

class RdrCamera :
    public RdrNode {
protected:
	RDRMAT4 view, projection;
public:
	RdrCamera();
	virtual ~RdrCamera();

	virtual void BuildProjection(bool perspective = true);
	virtual void BuildView();

	virtual RDRMAT4* GetView();
	virtual RDRMAT4* GetProjection();
};

#endif /* RDRCAMERA_H_ */
