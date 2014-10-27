#include "BaseRenderer.h"


BaseRenderer::BaseRenderer(BaseObject* parent, bool isIndexed)
{
	this->parent = parent;
	this->isIndexed = isIndexed;
}


BaseRenderer::~BaseRenderer(void)
{
}
