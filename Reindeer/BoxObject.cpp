#include "BoxObject.h"


BoxObject::BoxObject(LPDIRECT3DDEVICE9 graphicsDevice)
	: BaseObject()
{
	min.x = min.y = min.z = 0;
	max.x = max.y = max.z = 0;

	this->graphicsDevice = graphicsDevice;
	vertexBuffer = NULL;
	indexBuffer = NULL;

	BuildWidthHeightLength();
}


BoxObject::~BoxObject()
{
	if (vertexBuffer != NULL)
		vertexBuffer->Release();
	if (indexBuffer != NULL)
		indexBuffer->Release();
}


void BoxObject::SetGraphicsDevice(const LPDIRECT3DDEVICE9& graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;
	BuildBuffers();
}


LPDIRECT3DDEVICE9 BoxObject::GetGraphicsDevice()
{
	return graphicsDevice;
}


void BoxObject::SetMin(const D3DVECTOR& value)
{
	this->min = value;
	BuildWidthHeightLength();
}


D3DVECTOR BoxObject::GetMin()
{
	return min;
}


void BoxObject::SetMax(const D3DVECTOR& value)
{
	this->max = value;
	BuildWidthHeightLength();
}


D3DVECTOR BoxObject::GetMax()
{
	return max;
}


void BoxObject::SetMinMax(const D3DVECTOR& min, const D3DVECTOR& max)
{
	this->min = min;
	this->max = max;
	BuildWidthHeightLength();
}


void BoxObject::Draw(Camera& camera)
{
	if (graphicsDevice == NULL)
		return;

	graphicsDevice->SetFVF(CUSTOMFVF);
	
    graphicsDevice->SetTransform(D3DTS_VIEW, &camera.GetView());    // set the view transform
    graphicsDevice->SetTransform(D3DTS_PROJECTION, &camera.GetProjection());    // set the projection
    graphicsDevice->SetTransform(D3DTS_WORLD, &world);    // set the world transform

	// select the vertex and index buffers to use
    graphicsDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
    graphicsDevice->SetIndices(indexBuffer);

    // draw the cube
    graphicsDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}


void BoxObject::BuildWidthHeightLength()
{
	width  = max.x - min.x;
	height = max.y - min.y;
	length = max.z - min.z;

	BuildBuffers();
}


void BoxObject::BuildBuffers()
{
	if (graphicsDevice == NULL)
		return;

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
		graphicsDevice->CreateVertexBuffer(24*sizeof(CUSTOMVERTEX),
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
		graphicsDevice->CreateIndexBuffer(36*sizeof(short),
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