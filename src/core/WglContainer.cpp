#include "WglContainer.h"


WglContainer::WglContainer()
{
}


WglContainer::~WglContainer()
{
}

void WglContainer::Init()
{
	Win32Container::Init();

	PIXELFORMATDESCRIPTOR pfd;
	HGLRC hRC;
	int pixelformat;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	pixelformat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelformat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
}