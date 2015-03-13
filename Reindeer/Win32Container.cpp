#include "Win32Container.h"

unordered_map<HWND, Win32Container*> Win32Container::ContainerMap;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Win32Container* that = NULL;
	if (Win32Container::ContainerMap.find(hWnd) != Win32Container::ContainerMap.end())
		that = Win32Container::ContainerMap[hWnd];
	
	//RECT rcClient;                 // client area rectangle 
	//POINT ptClientUL;              // client upper left corner 
	//POINT ptClientLR;              // client lower right corner
	static POINTS pts; // mouse points

	if (message == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}

	if (that == NULL)
		return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_LBUTTONDOWN:
		//// Capture mouse input. 
		//
		//SetCapture(hWnd);
		//
		//// Retrieve the screen coordinates of the client area, 
		//// and convert them into client coordinates. 
		//
		//GetClientRect(hWnd, &rcClient);
		//ptClientUL.x = rcClient.left;
		//ptClientUL.y = rcClient.top;
		//
		//// Add one to the right and bottom sides, because the 
		//// coordinates retrieved by GetClientRect do not 
		//// include the far left and lowermost pixels. 
		//
		//ptClientLR.x = rcClient.right + 1;
		//ptClientLR.y = rcClient.bottom + 1;
		//ClientToScreen(hWnd, &ptClientUL);
		//ClientToScreen(hWnd, &ptClientLR);
		//
		//// Copy the client coordinates of the client area 
		//// to the rcClient structure. Confine the mouse cursor 
		//// to the client area by passing the rcClient structure 
		//// to the ClipCursor function. 
		//
		//SetRect(&rcClient, ptClientUL.x, ptClientUL.y,
		//	ptClientLR.x, ptClientLR.y);
		//ClipCursor(&rcClient);
		//
		//// Convert the cursor coordinates into a POINTS 
		//// structure, which defines the beginning point of the 
		//// line drawn during a WM_MOUSEMOVE message. 

		pts = MAKEPOINTS(lParam);
		that->MouseLeftButtonPressed(pts.x, pts.y);
		return 0;
	case WM_LBUTTONUP:
		pts = MAKEPOINTS(lParam);
		that->MouseLeftButtonReleased(pts.x, pts.y);
		return 0;
	case WM_MBUTTONDOWN:
		pts = MAKEPOINTS(lParam);
		that->MouseMiddleButtonPressed(pts.x, pts.y);
		return 0;
	case WM_MBUTTONUP:
		pts = MAKEPOINTS(lParam);
		that->MouseMiddleButtonReleased(pts.x, pts.y);
		return 0;
	case WM_RBUTTONDOWN:
		pts = MAKEPOINTS(lParam);
		that->MouseRightButtonPressed(pts.x, pts.y);
		return 0;
	case WM_RBUTTONUP:
		pts = MAKEPOINTS(lParam);
		that->MouseRightButtonReleased(pts.x, pts.y);
		return 0;
	case WM_MOUSEMOVE:
		pts = MAKEPOINTS(lParam);
		that->MouseMoved(pts.x, pts.y);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Win32Container::Win32Container() : Container()
{
}

Win32Container::~Win32Container()
{
}

int Win32Container::KEY_LEFT() { return 0; }
int Win32Container::KEY_UP() { return 0; }
int Win32Container::KEY_RIGHT() { return 0; }
int Win32Container::KEY_DOWN() { return 0; }


int Win32Container::ShouldClose()
{
	if (msg.message == WM_QUIT)
		return 1;

	return 0;
}

void Win32Container::Init()
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Reindeer", WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, GetModuleHandle(0), NULL);
	hDC = GetDC(hWnd);

	Win32Container::ContainerMap[hWnd] = this;

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

void Win32Container::PreUpdate()
{

}

void Win32Container::PostUpdate()
{
	SwapBuffers(hDC);
}

void Win32Container::ReadInput()
{
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}