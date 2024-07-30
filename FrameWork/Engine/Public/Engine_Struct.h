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

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // �ؽ�ó�� UV ��ǥ ���� FLOAT�� 3���� ǥ���ϰڴٴ� ��ũ��(��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ��� UV���� �������� �� �� �ִµ� ���� 0��° ���� �����ϰڴٴ� �ǹ�)

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
	

	
	//Collider�� ������ ������ ���� ����ü
	typedef struct ColInfo
	{
		UINT		m_iColLayer{ 0 };				// �ڽ��� �浹 ���̾�
		D3DXVECTOR3	m_vInfo[INFO_END];				// �ڽ��� ��ġ�� ���� ����
		D3DXVECTOR3 m_vSize{ 1.0f,1.0f,1.0f };		// �ڽ��� ������
		float		m_fRadius{ 0.0f };				// �ڽ��� ������
		D3DXMATRIX	m_matWorld;						// �ڽ��� �������
		void*		m_pThis{ nullptr };				// �ڽ��� �����͸� ����
		UINT		m_iDamage{ 0 };
	}CINFO;

	// Particle ����ü
	typedef struct tagParticle
	{
		D3DXVECTOR3 vPos{ 0.0f,0.0f,0.0f };
		D3DCOLOR dwColor{0xFFFFFFFF};
	}PTC;
	const DWORD FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	// ��ƼŬ ���
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


	// MapTool ������ �Ľ̿� ����ü
	struct PREFABDAT // ���� ��ü ��
	{
		PREFABDAT& operator=(const PREFABDAT& rhs)
		{
			lstrcpy(szName, rhs.szName);
			strcpy_s(sName, rhs.sName);
			lstrcpy(szTexPath, rhs.szTexPath);
			eType = rhs.eType;
		}

		_tchar szName[128]; // Ŭ�� �� �̸�
		char sName[128]; // ���� �� �̸�
		_tchar szTexPath[128]; // �ؽ�ó ���
		PREFABTYPE eType; // RcTex, Cube Ÿ������ 
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

		_tchar szOriginName[128]; // ���� �̸�
		_tchar szLayerName[128]; // Scene�ȿ� �ִ� Layer Key �̸�
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

	// [��ä] : UI_Shop test�� ���� �ӽ� ����ü ���� ����!
	typedef struct tagItem
	{
		std::wstring strProtoKey;
		std::wstring strItemName;
		_int iItemPrice = NULL;
	}ITEM;

}




#endif // Engine_Struct_h__
