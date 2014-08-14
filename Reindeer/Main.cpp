// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "BoxObject.h"

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;

// function prototypes
void initD3D(HWND hWnd);
void init_light(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // the WindowProc function prototype

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen( "CON", "w", stdout );

    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
                          WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

    initD3D(hWnd);   // set up and initialize Direct3D
	init_light();    // call the function to initialize the light and material


	// [rdr_note] Rdr objects instantiated here

	BoxObject box(d3ddev);
	D3DVECTOR min = {-3.0f, -3.0f, -3.0f};
	D3DVECTOR max = { 3.0f,  3.0f,  3.0f};
	box.SetMinMax(min, max);
	D3DVECTOR position = {10.0f, 0.0f, 0.0f}, rotation = {30, 0, 0};
	box.SetPosition(position);

	Camera camera;
	position.x = 0;
	position.y = 30;
	position.z = -30;
	camera.SetPosition(position, true);
	camera.SetEuler(rotation, true);
	camera.BuildWorld();
	

    // enter the main loop:

    MSG msg;

	float i = 0;
    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;

		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		d3ddev->BeginScene();


		// [rdr_note] modify Rdr objects behavior

		D3DVECTOR tempPosition = {(float)sin(i) * 12.0f, 0.0f, (float)cos(i) * 25.0f}, tempRotation = box.GetEuler();
		box.SetPosition(tempPosition, true);
		box.BuildWorld();
		

		// [rdr_note] draw Rdr objects
		
		box.Draw(camera);


		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
		i += 0.03f;
    }

	// clean d3d
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
}


// this is the function that sets up the lights and materials
void init_light(void)
{
    D3DLIGHT9 light;
    D3DMATERIAL9 material;

    ZeroMemory(&light, sizeof(light));
    light.Type = D3DLIGHT_POINT;    // make the light type 'point light'
    light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
    light.Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
    light.Range = 100.0f;    // a range of 100
    light.Attenuation0 = 0.0f;    // no constant inverse attenuation
    light.Attenuation1 = 0.125f;    // only .125 inverse attenuation
    light.Attenuation2 = 0.0f;    // no square inverse attenuation

    d3ddev->SetLight(0, &light);
    d3ddev->LightEnable(0, TRUE);

    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    d3ddev->SetMaterial(&material);

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
    d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));    // ambient light
    d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    // handle normals in scaling
}
