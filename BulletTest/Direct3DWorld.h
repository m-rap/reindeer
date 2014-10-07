#pragma once

#ifndef DIRECT3DWORLD_H
#define DIRECT3DWORLD_H

#include "world.h"

class Direct3DWorld :
	public World
{
private:
	LPDIRECT3D9 d3d;

protected:	
	virtual void Init3d();
	virtual void PreUpdate();
	virtual void PostUpdate();
	virtual void PostRender();

public:
	Direct3DWorld(void);
	virtual ~Direct3DWorld(void);
};

#endif