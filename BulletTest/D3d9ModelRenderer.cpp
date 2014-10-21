#include "D3d9ModelRenderer.h"
#include "ModelObject.h"

D3d9ModelRenderer::D3d9ModelRenderer(BaseObject* parent) : BaseRenderer(parent)
{
	vertexBuffer = NULL;
}

D3d9ModelRenderer::~D3d9ModelRenderer(void)
{
	if (vertexBuffer != NULL)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}
}

void D3d9ModelRenderer::BuildBuffers()
{	
	ModelObject* parent = (ModelObject*)this->parent;

	vertexCount = parent->vertices.size();

	VOID* pVoid;    // a void pointer

	// create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX* vertices = new CUSTOMVERTEX[vertexCount];
	for (size_t i = 0; i < vertexCount; i++)
	{
		vertices[i].X = parent->vertices[i].x;
		vertices[i].Y = parent->vertices[i].y;
		vertices[i].Z = parent->vertices[i].z;
		vertices[i].NORMAL.x = parent->normals[i].x;
		vertices[i].NORMAL.y = parent->normals[i].y;
		vertices[i].NORMAL.z = parent->normals[i].z;
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
    memcpy(pVoid, vertices, vertexCount * sizeof(CUSTOMVERTEX));
    vertexBuffer->Unlock();

	delete[] vertices;
}

void D3d9ModelRenderer::Draw(Camera* camera)
{
	RDRMAT4& world = *parent->GetWorld();

	d3ddev->SetFVF(CUSTOMFVF);
	
    d3ddev->SetTransform(D3DTS_VIEW, camera->GetView());    // set the view transform
    d3ddev->SetTransform(D3DTS_PROJECTION, camera->GetProjection());    // set the projection
    d3ddev->SetTransform(D3DTS_WORLD, &world);    // set the world transform

	d3ddev->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
    //d3ddev->SetIndices(indexBuffer);

    d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertexCount);
}