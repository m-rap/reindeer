#pragma once

#ifndef WIN32CONTAINER_H
#define WIN32CONTAINER_H

#include "Container.h"

#include <windows.h>
#include <windowsx.h>

class Win32Container :
	public Container
{
protected:
	MSG msg;

public:
	Win32Container();
	virtual ~Win32Container();

	HWND hWnd;
	HDC hDC;		// Private GDI Device Context

	static unordered_map<HWND, Win32Container*> ContainerMap;

	virtual int KEY_LEFT(); 
	virtual int KEY_RIGHT();
	virtual int KEY_UP();   
	virtual int KEY_DOWN(); 

	virtual int ShouldClose();
	virtual void Init();
	virtual void PreUpdate();
	virtual void PostUpdate();
	virtual void ReadInput();
};

#endif