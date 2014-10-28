#include "D3d9Renderer.h"
#include "ModelObject.h"

D3d9Renderer::D3d9Renderer(BaseObject* parent, bool isIndexed) : BaseRenderer(parent, isIndexed)
{
	vertexBuffer = NULL;
	indexBuffer = NULL;
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
}

void D3d9Renderer::BuildBuffers(
	RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
	size_t vertexCount, size_t indexCount, size_t uvCount
)
{	
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;

	VOID* pVoid;    // a void pointer

	// create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX* tempVertices = new CUSTOMVERTEX[vertexCount];
	for (size_t i = 0; i < vertexCount; i++)
	{
		tempVertices[i].X = vertices[i].x;
		tempVertices[i].Y = vertices[i].y;
		tempVertices[i].Z = vertices[i].z;
		tempVertices[i].NORMAL.x = normals[i].x;
		tempVertices[i].NORMAL.y = normals[i].y;
		tempVertices[i].NORMAL.z = normals[i].z;
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

	if (indexCount > 0)
	{
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
		memcpy(pVoid, indices, indexCount * sizeof(unsigned short));
		indexBuffer->Unlock();
	}
}

void D3d9Renderer::Draw(Camera* camera)
{
	RDRMAT4& world = *parent->GetWorld();

	d3ddev->SetFVF(CUSTOMFVF);
	
    d3ddev->SetTransform(D3DTS_VIEW, camera->GetView());    // set the view transform
    d3ddev->SetTransform(D3DTS_PROJECTION, camera->GetProjection());    // set the projection
    d3ddev->SetTransform(D3DTS_WORLD, &world);    // set the world transform

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