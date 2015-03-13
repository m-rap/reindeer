#pragma once
#ifndef WGLCONTAINER_H
#define WGLCONTAINER_H

#include "Win32Container.h"
class WglContainer :
	public Win32Container
{
public:
	WglContainer();
	virtual ~WglContainer();

	virtual void Init();
};

#endif