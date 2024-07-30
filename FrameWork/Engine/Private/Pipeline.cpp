#include "..\..\Header\Pipeline.h"



Pipeline::Pipeline()
{
}


Pipeline::~Pipeline()
{
}

void Pipeline::MakeTransformMatrix(_matrix * pMatrix, const _vec3 * pRight, const _vec3 * pUp, const _vec3 * pLook, const _vec3 * pPos)
{
	memcpy(&pMatrix->m[0][0], pRight, sizeof(_vec3));
	memcpy(&pMatrix->m[1][0], pUp, sizeof(_vec3));
	memcpy(&pMatrix->m[2][0], pLook, sizeof(_vec3));
	memcpy(&pMatrix->m[3][0], pPos, sizeof(_vec3));

}

void Pipeline::Make_ViewMatrix(_matrix * pMatrix,
	const _vec3 * pEye, 
	const _vec3 * pAt, 
	const _vec3 * pUp)
{
	D3DXMatrixIdentity(pMatrix);

	_vec3	vRight	{ 1.f, 0.f, 0.f };
	_vec3	vUp		{ 0.f, 1.f, 0.f };
	_vec3	vLook	{ 0.f, 0.f, 1.f };
	_vec3	vPos	{ 0.f, 0.f, 0.f };

	// look
	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	// right
	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	// up
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	// pos
	vPos = *pEye;
	
	MakeTransformMatrix(pMatrix, &vRight, &vUp, &vLook, &vPos);

	D3DXMatrixInverse(pMatrix, 0, pMatrix);

}

void Pipeline::Make_ProjMatrix(_matrix * pMatrix, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	D3DXMatrixIdentity(pMatrix);

	pMatrix->_11 = (1.f / tan(fFov / 2.f)) / fAspect;
	pMatrix->_22 = 1.f / tan(fFov / 2.f);
	pMatrix->_33 = fFar / (fFar - fNear);
	pMatrix->_34 = 1.f;
	pMatrix->_43 = (-fNear * fFar) / (fFar - fNear);
	pMatrix->_44 = 0.f;

}
