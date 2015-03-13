#include "D3d9Renderer.h"
#include "ModelObject.h"

D3d9Renderer::D3d9Renderer(BaseObject* parent, bool isIndexed, bool useTexture) : BaseRenderer(parent, isIndexed, useTexture)
{
	vertexBuffer = NULL;
	indexBuffer = NULL;

	if (useTexture)
	{
		D3DXCreateTextureFromFile(d3ddev, L"../uvmap.DDS", &texture);
	}
}

D3d9Renderer::~D3d9Renderer(void)
{
	if (vertexBuffer != NULL)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}
	if (indexBuffer != NULL)
	{
		indexBuffer->Release();
		indexBuffer = NULL;
	}

	if (useTexture)
	{
		texture->Release();
		texture = NULL;
	}
}

void D3d9Renderer::BuildBuffers(
	RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
	size_t vertexCount, size_t indexCount, size_t uvCount
)
{	
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;
	this->uvCount = uvCount;

	VOID* pVoid;    // a void pointer

	// create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX* tempVertices = new CUSTOMVERTEX[vertexCount];
	size_t j;
	for (size_t i = 0; i < vertexCount; i++)
	{
		j = i;
		if (!isIndexed)
		{
			if (i % 3 == 1)
				j++;
			else if (i % 3 == 2)
				j--;
		}
		
		tempVertices[j].X = vertices[i].x;
		tempVertices[j].Y = vertices[i].y;
		tempVertices[j].Z = vertices[i].z;
		tempVertices[j].NORMAL.x = normals[i].x;
		tempVertices[j].NORMAL.y = normals[i].y;
		tempVertices[j].NORMAL.z = normals[i].z;

		if (useTexture)
		{
			tempVertices[j].u = uvs[i].x;
			tempVertices[j].v = uvs[i].y;
		}
	}

	if (vertexBuffer == NULL)
	{
		// create a vertex buffer interface called v_buffer
		d3ddev->CreateVertexBuffer(vertexCount * sizeof(CUSTOMVERTEX),
			0,
			CUSTOMFVF,
			D3DPOOL_MANAGED,
			&vertexBuffer,
			NULL);
	}

    // lock v_buffer and load the vertices into it
    vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, tempVertices, vertexCount * sizeof(CUSTOMVERTEX));
    vertexBuffer->Unlock();

	delete[] tempVertices;

	if (isIndexed && indexCount > 0)
	{
		unsigned short* tempIndices = new unsigned short[indexCount];
		for (size_t i = 0; i < indexCount; i += 3)
		{
			tempIndices[i] = indices[i];
			tempIndices[i + 2] = indices[i + 1];
			tempIndices[i + 1] = indices[i + 2];
		}

		if (indexBuffer == NULL)
		{
			// create an index buffer interface called i_buffer
			d3ddev->CreateIndexBuffer(indexCount * sizeof(unsigned short),
									  0,
									  D3DFMT_INDEX16,
									  D3DPOOL_MANAGED,
									  &indexBuffer,
									  NULL);
		}

		// lock i_buffer and load the indices into it
		indexBuffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, tempIndices, indexCount * sizeof(unsigned short));
		indexBuffer->Unlock();

		delete[] tempIndices;
	}
}

void D3d9Renderer::Draw(Camera* camera, Light* light)
{
	RDRMAT4& world = *parent->GetWorld();

	d3ddev->SetFVF(CUSTOMFVF);
	
    d3ddev->SetTransform(D3DTS_VIEW, camera->GetView());    // set the view transform
    d3ddev->SetTransform(D3DTS_PROJECTION, camera->GetProjection());    // set the projection
    d3ddev->SetTransform(D3DTS_WORLD, &world);    // set the world transform

	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	material.Power = 128;
	
	d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	if (useTexture)
	{
		d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		d3ddev->SetTexture(0, texture);
	}
	else
	{
		d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
    d3ddev->SetMaterial(&material);

	d3ddev->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	if (isIndexed)
	{
		d3ddev->SetIndices(indexBuffer);
		d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, indexCount / 3);
	}
	else
	{
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertexCount / 3);
	}
}