#include "stdafx.h"
#include "FloatingFloor.h"
#include "Player.h"

#include "Export_Utility.h"

FloatingFloor::FloatingFloor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{

}

FloatingFloor::FloatingFloor(const FloatingFloor& rhs)
	: Super(rhs)
{

}

FloatingFloor::~FloatingFloor()
{

}

HRESULT FloatingFloor::Init_GameObject()
{
	Super::Init_GameObject();

	m_eFloatState = FLOATING_IDLE;

	return S_OK;
}

Engine::_int FloatingFloor::Update_GameObject(const _float& fTimeDelta)
{
	m_fDeltaTime = fTimeDelta;

	_int	iExit = Super::Update_GameObject(fTimeDelta);

	//m_fDisTime += fTimeDelta;
	//if (m_fDisTime >= 1.f)
	//{
	//	AddCollider(m_pColliderCom);
	//	AddCollider(m_pColliderWallCom);
	//
	//	Add_RenderGroup(RENDER_NONALPHA, this);
	//	if (m_fDisTime >= 6.f)
	//	{
	//		m_fDisTime = 0.f;
	//	}
	//}

	AddCollider(m_pColliderCom);
	AddCollider(m_pColliderWallCom);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void FloatingFloor::LateUpdate_GameObject()
{
	switch (m_eFloatState)
	{
	case FLOATING_IDLE:
	{
		FFIdle();
	}
	break;
	case FLOATING_MOVEUP:
	{
		FFMoveUp();
	}
	break;
	case FLOATING_MOVEDOWN:
	{
		FFMoveDown();
	}
	break;
	case FLOATING_DESTROY:
	{
		FFDestroy();
	}
	break;
	}

	OnCollided();

	Super::LateUpdate_GameObject();
}

void FloatingFloor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

FloatingFloor* FloatingFloor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& textureKey, _vec3 colSize, COLLAYER colLayer, _vec3& startPos, _vec3& endPos, InteractableType intType, FloatingState floatingState)
{
	FloatingFloor* pInstance = new FloatingFloor(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FloatingFloor Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("FloatingFloor Create Failed");
		return nullptr;
	}

	pInstance->Set_TextureKey(textureKey);

	pInstance->Get_Collider()->SetSize(_vec3(1.f, 1.f, 1.f));
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	pInstance->Get_WallCollider()->SetSize(colSize);
	pInstance->Get_WallCollider()->SetColLayer(COL_WALL);
	pInstance->Get_WallCollider()->SetThisPointer(pInstance);

	pInstance->Get_Transform()->Set_Pos(startPos);
	pInstance->Set_StartPos(startPos);
	pInstance->Set_EndPos(endPos);

	pInstance->Set_IntType(intType);
	pInstance->Set_State(floatingState);
	
	return pInstance;
}

void FloatingFloor::FFIdle()
{
	
}

void FloatingFloor::FFMoveUp()
{
	//  || fabs(D3DXVec3Length(&(m_vEndPos - Get_Transform()->Get_Info(INFO_POS)))) < 1.f
	if (Get_Transform()->Get_Info(INFO_POS).y >= m_vEndPos.y)
	{
		Get_Transform()->Set_Pos(m_vEndPos);
		m_eFloatState = FLOATING_DESTROY;

		return;
	}

	_vec3 dir = m_vEndPos - m_vStartPos;
	D3DXVec3Normalize(&dir, &dir);

	Get_Transform()->Move_Forward(&dir, m_fDeltaTime, 5.f);
}

void FloatingFloor::FFMoveDown()
{
	// || fabs(D3DXVec3Length(&(Get_Transform()->Get_Info(INFO_POS) - m_vStartPos))) < 1.f
	if (Get_Transform()->Get_Info(INFO_POS).y <= m_vStartPos.y)
	{
		Get_Transform()->Set_Pos(m_vStartPos);
		m_eFloatState = FLOATING_MOVEUP;

		return;
	}

	_vec3 dir = m_vStartPos - m_vEndPos;
	D3DXVec3Normalize(&dir, &dir);

	Get_Transform()->Move_Forward(&dir, m_fDeltaTime, 5.f);
}

void FloatingFloor::FFDestroy()
{
	
}

void FloatingFloor::OnCollided()
{
	if (m_pColliderCom->GetColSphere() & COL_PLAYER)
	{
		list<pair<CINFO, _vec3>> playerList = m_pColliderCom->GetColSphere_Info(COL_PLAYER);
		for (auto& it : playerList)
		{
			Player* player = static_cast<Player*>(it.first.m_pThis);
			if (player)
			{
				if (m_bIsOnFloor == false)
				{
					_vec3 dir1 = player->Get_Transform()->Get_Info(INFO_POS) - m_vStartPos;
					_vec3 dir2 = player->Get_Transform()->Get_Info(INFO_POS) - m_vEndPos;
					float distance1 = fabs(D3DXVec3Length(&dir1));
					float distance2 = fabs(D3DXVec3Length(&dir2));

					if (distance1 < 1.f)
					{
						m_eFloatState = FLOATING_MOVEUP;
					}

					else if (distance2 < 1.f)
					{
						m_eFloatState = FLOATING_MOVEDOWN;
					}

					else if (distance1 < distance2)
					{
						m_eFloatState = FLOATING_MOVEUP;
					}

					else if (distance1 > distance2)
					{
						m_eFloatState = FLOATING_MOVEDOWN;
					}

					m_bIsOnFloor = true;
				}
			}
		}
	}
	else
	{
		m_eFloatState = FLOATING_IDLE;
		m_bIsOnFloor = false;
	}
}

HRESULT FloatingFloor::Add_Component(const wstring& textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void FloatingFloor::Free()
{
	Super::Free();
}
