#pragma once

#ifdef USE_D3D9

#ifndef D3D9WORLD_H
#define D3D9WORLD_H

#include "world.h"

class D3d9World :
	public World
{
private:
	LPDIRECT3D9 d3d;

protected:
	virtual void Init3d();
	virtual void PreRender();
	virtual void PreUpdate();
	virtual void PostUpdate();
	virtual void PostRender();

public:
	D3d9World(Container* container);
	virtual ~D3d9World(void);
};

#endif

#endif
