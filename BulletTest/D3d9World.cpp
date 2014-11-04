#include "D3d9World.h"

LPDIRECT3DDEVICE9 d3ddev;

D3d9World::D3d9World(void) : World()
{
}


D3d9World::~D3d9World(void)
{
}

void D3d9World::Init3d()
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


	D3DLIGHT9 light;
    D3DMATERIAL9 material;

    ZeroMemory(&light, sizeof(light));
    light.Type = D3DLIGHT_POINT;    // make the light type 'point light'
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    light.Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
    light.Range = 100.0f;    // a range of 100
    light.Attenuation0 = 0.0f;    // no constant inverse attenuation
    light.Attenuation1 = 0.125f;    // only .125 inverse attenuation
    light.Attenuation2 = 0.0f;    // no square inverse attenuation

    d3ddev->SetLight(0, &light);
    d3ddev->LightEnable(0, TRUE);

    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    material.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

    d3ddev->SetMaterial(&material);

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
    //d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255 / 5, 255 / 5, 255 / 5));    // ambient light
    d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    // handle normals in scaling
}

void D3d9World::PreUpdate()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();
}

void D3d9World::PostUpdate()
{
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void D3d9World::PostRender()
{
	// clean d3d
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
}