#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL Pipeline
{
private:
	explicit Pipeline();
	~Pipeline();

public:
	static void MakeTransformMatrix(_matrix* pMatrix, 
		const _vec3* pRight,
		const _vec3* pUp,
		const _vec3* pLook,
		const _vec3* pPos);

	static void Make_ViewMatrix(_matrix* pMatrix, 
		const _vec3* pEye,
		const _vec3* pAt, 
		const _vec3* pUp);

	static void	Make_ProjMatrix(_matrix* pMatrix,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

};

END