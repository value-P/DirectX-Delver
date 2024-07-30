#include "stdafx.h"
#include "Creature.h"
#include "ItemBase.h"

#include "Export_System.h"
#include "Export_Utility.h"

Creature::Creature(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{

}

Creature::Creature(const Creature& rhs)
	: Super(rhs)
{

}

Creature::~Creature()
{

}

HRESULT Creature::Init_GameObject()
{
	Super::Init_GameObject();

	FAILED_CHECK_RETURN(Creature::Add_Component(), E_FAIL);

	m_creatureStat = new CreatureStat();

	return S_OK;
}

Engine::_int Creature::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = Super::Update_GameObject(fTimeDelta);


	return iExit;
}

void Creature::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();

}

void Creature::Render_GameObject()
{
	Super::Render_GameObject(); // 딱히 없어도 무방

}

void Creature::Set_CreatureStat(CreatureStat* creatureStat)
{
	*m_creatureStat = *creatureStat;
}

HRESULT Creature::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Collider",pComponent });
	m_pColliderCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void Creature::PlaneCollision(bool* _bOnTerrain)
{
	// OBB와 구 충돌만 적용
	// 구와 가장 가까운 점이 VECTOR3 형식으로 들어가 있다~
	list<pair<CINFO, D3DXVECTOR3>>::const_iterator iter;

	for (iter = m_pColliderCom->GetColOBB_Info(COL_WALL).begin(); iter != m_pColliderCom->GetColOBB_Info(COL_WALL).end(); iter++)
	{
		D3DXVECTOR3 PlayerPos{ m_pTransformCom->Get_Info(INFO_POS) };
		float fRadius{ m_pColliderCom->GetRadius() };
		D3DXVECTOR3 Dir{ PlayerPos - (*iter).second };
		float LengthSq = D3DXVec3LengthSq(&Dir);
		if (LengthSq < fRadius * fRadius)
		{
			D3DXVECTOR3 vNormalDir{};
			D3DXVec3Normalize(&vNormalDir, &Dir);
			float fRadian{ D3DXVec3Dot(&vNormalDir, &D3DXVECTOR3{ 0.0f,1.0f,0.0f }) };
			D3DXVECTOR3 fSize{ m_pColliderCom->GetSize() };
			float hypotenuse = sqrt(fSize.y * fSize.y + fSize.x * fSize.x);
			if (fRadian * hypotenuse >= fSize.y)
			{
				if ((*iter).second.y >= PlayerPos.y - fSize.y)
				{
					m_pTransformCom->Move_PosY(fSize.y - Dir.y);
					if (_bOnTerrain)
					{
						*_bOnTerrain = true;
					}
				}
			}
			else if (fRadian * hypotenuse < fSize.y && fRadian * hypotenuse >= -fSize.y)
			{
				float fr{ fSize.x * fSize.x};
				if (LengthSq < fr)
				{
					vNormalDir.y = 0.0f;
					D3DXVec3Normalize(&vNormalDir, &vNormalDir);
					m_pTransformCom->Move_Pos(vNormalDir * (sqrt(fr) - sqrt(LengthSq)));
				}
			}
			else
			{
				if ((*iter).second.y <= PlayerPos.y + fSize.y)
				{
					m_pTransformCom->Move_PosY(-(fSize.y + Dir.y));
				}
			}
		}
	}
}

void Creature::Free()
{
	if (m_creatureStat)
	{
		delete m_creatureStat;
		m_creatureStat = nullptr;
	}

	Super::Free();
}
