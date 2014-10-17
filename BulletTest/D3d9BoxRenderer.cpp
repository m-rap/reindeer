#include "D3d9BoxRenderer.h"
#include "BoxObject.h"

D3d9BoxRenderer::D3d9BoxRenderer(BaseObject* parent) : BoxRenderer(parent)
{
	vertexBuffer = NULL;
	indexBuffer = NULL;
}


D3d9BoxRenderer::~D3d9BoxRenderer(void)
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

void D3d9BoxRenderer::BuildBuffers()
{	
	BoxObject* parent = (BoxObject*)this->parent;

	float width  = parent->GetWidth(),
		  height = parent->GetHeight(),
		  length = parent->GetLength();

	VOID* pVoid;    // a void pointer

	#pragma region build vertex buffer
	// create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] =
    {
        { -width / 2, -height / 2, length / 2,  0.0f, 0.0f, 1.0f, },    // side 1
        {  width / 2, -height / 2, length / 2,  0.0f, 0.0f, 1.0f, },
        { -width / 2,  height / 2, length / 2,  0.0f, 0.0f, 1.0f, },
        {  width / 2,  height / 2, length / 2,  0.0f, 0.0f, 1.0f, },
		
        { -width / 2, -height / 2, -length / 2,  0.0f, 0.0f, -1.0f, },    // side 2
        { -width / 2,  height / 2, -length / 2,  0.0f, 0.0f, -1.0f, },
        {  width / 2, -height / 2, -length / 2,  0.0f, 0.0f, -1.0f, },
        {  width / 2,  height / 2, -length / 2,  0.0f, 0.0f, -1.0f, },
		
        { -width / 2, height / 2, -length / 2,  0.0f, 1.0f, 0.0f, },    // side 3
        { -width / 2, height / 2,  length / 2,  0.0f, 1.0f, 0.0f, },
        {  width / 2, height / 2, -length / 2,  0.0f, 1.0f, 0.0f, },
        {  width / 2, height / 2,  length / 2,  0.0f, 1.0f, 0.0f, },
		
        { -width / 2, -height / 2, -length / 2,  0.0f, -1.0f, 0.0f, },    // side 4
        {  width / 2, -height / 2, -length / 2,  0.0f, -1.0f, 0.0f, },
        { -width / 2, -height / 2,  length / 2,  0.0f, -1.0f, 0.0f, },
        {  width / 2, -height / 2,  length / 2,  0.0f, -1.0f, 0.0f, },
		
        { width / 2, -height / 2, -length / 2,  1.0f, 0.0f, 0.0f, },    // side 5
        { width / 2,  height / 2, -length / 2,  1.0f, 0.0f, 0.0f, },
        { width / 2, -height / 2,  length / 2,  1.0f, 0.0f, 0.0f, },
        { width / 2,  height / 2,  length / 2,  1.0f, 0.0f, 0.0f, },
		
        { -width / 2, -height / 2, -length / 2,  -1.0f, 0.0f, 0.0f, },    // side 6
        { -width / 2, -height / 2,  length / 2,  -1.0f, 0.0f, 0.0f, },
        { -width / 2,  height / 2, -length / 2,  -1.0f, 0.0f, 0.0f, },
        { -width / 2,  height / 2,  length / 2,  -1.0f, 0.0f, 0.0f, },
    };

	if (vertexBuffer == NULL)
	{
		// create a vertex buffer interface called v_buffer
		d3ddev->CreateVertexBuffer(24*sizeof(CUSTOMVERTEX),
			0,
			CUSTOMFVF,
			D3DPOOL_MANAGED,
			&vertexBuffer,
			NULL);
	}

    // lock v_buffer and load the vertices into it
    vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    vertexBuffer->Unlock();
	#pragma endregion
}

void D3d9BoxRenderer::BuildNormalsIndices()
{
	VOID* pVoid;
	#pragma region build index buffer
    // create the indices using an int array
    short indices[] =
    {
        0, 1, 2,    // side 1
        2, 1, 3,
        4, 5, 6,    // side 2
        6, 5, 7,
        8, 9, 10,    // side 3
        10, 9, 11,
        12, 13, 14,    // side 4
        14, 13, 15,
        16, 17, 18,    // side 5
        18, 17, 19,
        20, 21, 22,    // side 6
        22, 21, 23,
    };

	if (indexBuffer == NULL)
	{
		// create an index buffer interface called i_buffer
		d3ddev->CreateIndexBuffer(36*sizeof(short),
								  0,
								  D3DFMT_INDEX16,
								  D3DPOOL_MANAGED,
								  &indexBuffer,
								  NULL);
	}

    // lock i_buffer and load the indices into it
    indexBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, indices, sizeof(indices));
    indexBuffer->Unlock();
	#pragma endregion
}

void D3d9BoxRenderer::Draw(Camera* camera)
{
	RDRMAT4& world = *parent->GetWorld();

	d3ddev->SetFVF(CUSTOMFVF);
	
    d3ddev->SetTransform(D3DTS_VIEW, camera->GetView());    // set the view transform
    d3ddev->SetTransform(D3DTS_PROJECTION, camera->GetProjection());    // set the projection
    d3ddev->SetTransform(D3DTS_WORLD, &world);    // set the world transform

	// select the vertex and index buffers to use
    d3ddev->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
    d3ddev->SetIndices(indexBuffer);

    // draw the cube
    d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}