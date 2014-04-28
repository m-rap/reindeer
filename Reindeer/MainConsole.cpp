#include <stdio.h>
#include "RdrHelper.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

void printMatrix(D3DMATRIX* matrix)
{
	printf("%7.3f %7.3f %7.3f %7.3f\n", matrix->_11, matrix->_12, matrix->_13, matrix->_14);
	printf("%7.3f %7.3f %7.3f %7.3f\n", matrix->_21, matrix->_22, matrix->_23, matrix->_24);
	printf("%7.3f %7.3f %7.3f %7.3f\n", matrix->_31, matrix->_32, matrix->_33, matrix->_34);
	printf("%7.3f %7.3f %7.3f %7.3f\n", matrix->_41, matrix->_42, matrix->_43, matrix->_44);
}

void toq1(D3DXQUATERNION& q, D3DXVECTOR3& ypr)
{
	float s1 = sin(ypr.x/2), s2 = sin(ypr.y/2), s3 = sin(ypr.z/2),
		  c1 = cos(ypr.x/2), c2 = cos(ypr.y/2), c3 = cos(ypr.z/2);
	q.w = c2 * c1 * c3 - s2 * s1 * s3;
	q.z = s2 * s1 * c3 + c2 * c1 * s3;
	q.y = s2 * c1 * c3 + c2 * s1 * s3;
	q.x = c2 * s1 * c3 - s2 * c1 * s3;
}

void toq2(D3DXQUATERNION& q, D3DXVECTOR3& ypr)
{
	float S1 = sin(ypr.x), S2 = sin(ypr.y), S3 = sin(ypr.z),
		  C1 = cos(ypr.x), C2 = cos(ypr.y), C3 = cos(ypr.z);

	q.w = sqrt(1.0 + C2 * C1 + C2*C3 - S2 * S1 * S3 + C1*C3) / 2;
	q.z = (C1 * S3 + C2 * S3 + S2 * S1 * C3) / (4.0 * q.w);
	q.y = (S2 * C1 + S2 * C3 + C2 * S1 * S3) / (4.0 * q.w);
	q.x = (-S2 * S3 + C2 * S1 * C3 + S1)     / (4.0 * q.w);
}

int main()
{
	D3DXQUATERNION q, tempQ;
	D3DXQuaternionIdentity(&q);

	D3DXVECTOR3 ypr(D3DXToRadian(30), D3DXToRadian(45), D3DXToRadian(60));
	D3DXQuaternionRotationYawPitchRoll(&q, ypr.y, ypr.x, ypr.z);

	RdrHelper::QuaternionToEuler(ypr, q);
	printf("%f %f %f\n", ypr.x, ypr.y, ypr.z);
	RdrHelper::Vec3ToDegree(ypr, ypr);
	printf("%f %f %f\n", ypr.x, ypr.y, ypr.z);

	getchar();

	return 0;
}