#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3		vPosition;		
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;
	
	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	
	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;
	

	
	//Collider가 가지는 정보에 대한 구조체
	typedef struct ColInfo
	{
		UINT		m_iColLayer{ 0 };				// 자신의 충돌 레이어
		D3DXVECTOR3	m_vInfo[INFO_END];				// 자신의 위치와 방향 정보
		D3DXVECTOR3 m_vSize{ 1.0f,1.0f,1.0f };		// 자신의 사이즈
		float		m_fRadius{ 0.0f };				// 자신의 반지름
		D3DXMATRIX	m_matWorld;						// 자신의 월드행렬
		void*		m_pThis{ nullptr };				// 자신의 포인터를 저장
		UINT		m_iDamage{ 0 };
	}CINFO;

	// Particle 구조체
	typedef struct tagParticle
	{
		D3DXVECTOR3 vPos{ 0.0f,0.0f,0.0f };
		D3DCOLOR dwColor{0xFFFFFFFF};
	}PTC;
	const DWORD FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	// 파티클 요소
	struct Attribute
	{
		D3DXVECTOR3 vPos{ 0.0f,0.0f,0.0f };
		D3DXVECTOR3 vVelocity{ 0.0f,0.0f,0.0f };
		D3DXVECTOR3 vAccel{ 0.0f,0.0f,0.0f };
		float fLifeTime{ 0.0f };
		float fAge{ 0.0f };
		D3DXCOLOR dwColor{ 0.0f,0.0f,0.0f,0.0f };
		D3DXCOLOR dwColorFade{ 0.0f,0.0f,0.0f,0.0f };
		float fSize{ 0.0f };
		bool bAlive{ false };
		D3DXVECTOR3 vPosLinear{ 0.0f,0.0f,0.0f };
	};

	struct BoundingBox
	{
		bool IsPointInside(const D3DXVECTOR3& _vPoint)
		{
			if (_vMin.x <= _vPoint.x && _vMax.x >= _vPoint.x && _vMin.y <= _vPoint.y && _vMax.y >= _vPoint.y && _vMin.z <= _vPoint.z && _vMax.z >= _vPoint.z)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		D3DXVECTOR3 _vMin{ -1.0f,-1.0f,-1.0f };
		D3DXVECTOR3 _vMax{ 1.0f,1.0f,1.0f };
	};

	struct BoundingSphere
	{
		D3DXVECTOR3 _vCenter{ 0.0f,0.0f,0.0f };
		float		_fRadius{ 0.0f };
	};


	// MapTool 데이터 파싱용 구조체
	struct PREFABDAT // 원본 객체 용
	{
		PREFABDAT& operator=(const PREFABDAT& rhs)
		{
			lstrcpy(szName, rhs.szName);
			strcpy_s(sName, rhs.sName);
			lstrcpy(szTexPath, rhs.szTexPath);
			eType = rhs.eType;
		}

		_tchar szName[128]; // 클라 용 이름
		char sName[128]; // 맵툴 용 이름
		_tchar szTexPath[128]; // 텍스처 경로
		PREFABTYPE eType; // RcTex, Cube 타입인지 
		//COLLIDERDAT tColData;
	};

	struct CLONEDAT
	{
		CLONEDAT& operator=(const CLONEDAT& rhs)
		{
			lstrcpy(szOriginName, rhs.szOriginName);
			lstrcpy(szLayerName, rhs.szLayerName);
			memcpy(&vScale, rhs.vScale, sizeof(vScale));
			memcpy(&vRotation, rhs.vRotation, sizeof(vRotation));
			memcpy(&vTransform, rhs.vTransform, sizeof(vTransform));
		}

		_tchar szOriginName[128]; // 원본 이름
		_tchar szLayerName[128]; // Scene안에 있는 Layer Key 이름
		_vec3 vScale;
		_vec3 vRotation;
		_vec3 vTransform;
	};

	struct COLBOXDAT
	{
		size_t m_iColLayer; // Collider Layer
		_vec3 vScale;
		_vec3 vPos;
	};

	// [우채] : UI_Shop test를 위한 임시 구조체 추후 삭제!
	typedef struct tagItem
	{
		std::wstring strProtoKey;
		std::wstring strItemName;
		_int iItemPrice = NULL;
	}ITEM;

}




#endif // Engine_Struct_h__
