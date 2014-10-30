#include "BaseRenderer.h"


BaseRenderer::BaseRenderer(BaseObject* parent, bool isIndexed, bool useTexture)
{
	this->parent = parent;
	this->isIndexed = isIndexed;
	this->useTexture = useTexture;
}


BaseRenderer::~BaseRenderer(void)
{
}
