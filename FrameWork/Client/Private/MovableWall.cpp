#include "stdafx.h"
#include "MovableWall.h"
#include "Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

MovableWall::MovableWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

MovableWall::MovableWall(const MovableWall& rhs)
	: Super(rhs)
{
}

MovableWall::~MovableWall()
{
}

HRESULT MovableWall::Init_GameObject()
{
	Super::Init_GameObject();

	m_eIntState = INTSTATE_IDLE;

	return S_OK;
}

_int MovableWall::Update_GameObject(const _float& fTimeDelta)
{
	/*if (m_bIsOpened == true && m_eIntState == INTSTATE_DESTROY)
		return -1;*/

	_int	iExit = Super::Update_GameObject(fTimeDelta);

	if (m_bIsOpened == false)
	{
		m_fDeltaTime = fTimeDelta;

		AddCollider(m_pColliderCom);
		AddCollider(m_pColliderWallCom);

		Add_RenderGroup(RENDER_NONALPHA, this);
	}
	
	return iExit;
}

void MovableWall::LateUpdate_GameObject()
{
	if (m_bIsOpened == false)
	{
		switch (m_eIntState)
		{
		case INTSTATE_IDLE:
		{
			MovableWallIdle();
		}
		break;
		case INTSTATE_MOVE:
		{
			MovableWallMove();
		}
		break;
		}

		Super::LateUpdate_GameObject();
	}

	OnCollided();

	Super::LateUpdate_GameObject();
}

void MovableWall::Render_GameObject()
{
	if (m_bIsOpened == false)
	{
		if (m_pColliderCom->GetColSphere() & COL_PLAYER)
		{
			if (m_eWallType == WALLTYPE_WALL)
				Engine::Render_Font(L"Font_LogoText", L"E : 열기", &_vec4(0, 0, WINCX, WINCY), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		if (m_eWallType == WALLTYPE_LADDER)
		{
			m_pBufferCom->Set_UVScale(1, 6);
		}
		else if (m_eWallType == WALLTYPE_SEAL0 || m_eWallType == WALLTYPE_SEAL1 || m_eWallType == WALLTYPE_SEAL2)
		{
			m_pBufferCom->Set_UVScale(2, 1);
		}
		else
		{
			m_pBufferCom->Set_UVScale(2, 2);
		}

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pBufferCom->Set_UVScale(1, 1);
	}
}

void MovableWall::Set_DesiredPos(_vec3& desiredPos)
{
	m_vDesiredPos = Get_Transform()->Get_Info(INFO_POS) + desiredPos;
}

void MovableWall::Set_DesiredPos(InteractableDir intDir)
{
	switch (intDir)
	{
	case INTDIR_UP:
	{
		Set_DesiredPos(_vec3(0.f, 4.f, 0.f));
	}
	break;
	case INTDIR_RIGHT:
	{
		Set_DesiredPos(_vec3(4.f, 0.f, 0.f));
	}
	break;
	case INTDIR_DOWN:
	{
		Set_DesiredPos(_vec3(0.f, -4.f, 0.f));
	}
	break;
	case INTDIR_LEFT:
	{
		Set_DesiredPos(_vec3(-4.f, 0.f, 0.f));
	}
	break;
	}
}

MovableWall* MovableWall::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, InteractableDir intDir, InteractableType intType, MovableWallType wallType, TriggerType triType)
{
	MovableWall* pInstance = new MovableWall(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MovableWall Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MovableWall Create Failed");
		return nullptr;
	}

	pInstance->Set_TextureKey(textureKey);

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	pInstance->Get_WallCollider()->SetSize(colSize);
	pInstance->Get_WallCollider()->SetColLayer(COL_WALL);
	pInstance->Get_WallCollider()->SetThisPointer(pInstance);

	pInstance->Get_Transform()->Set_Pos(initPos);

	pInstance->Set_IntDir(intDir);
	pInstance->Set_IntType(intType);
	pInstance->Set_WallType(wallType);
	pInstance->Set_TriggerType(triType);

	pInstance->Set_DesiredPos(intDir);

	return pInstance;
}

void MovableWall::OnCollided()
{
	if (m_pColliderCom->GetColSphere() & COL_PLAYER)
	{
		list<pair<CINFO, _vec3>> playerList = m_pColliderCom->GetColSphere_Info(COL_PLAYER);
		for (auto& it : playerList)
		{
			Player* player = static_cast<Player*>(it.first.m_pThis);
			if (player)
			{
				pPlayer = player; // 아 이런거 지양해야하는데...
				if (m_eTriType & TRITYPE_BLOCK)
				{
					
				}
				else if (m_eTriType & TRITYPE_WALL)
				{
					//if (Engine::Key_Down('E'))
					//	m_eIntState = INTSTATE_MOVE;
					if (Engine::Get_DIKeyState(DIK_E) & 0x80)
					{
						m_eIntState = INTSTATE_MOVE;
						//SoundMgr::GetInstance()->Play_Sound(L"door_stone_open", SOUND_INTERACTION, 0.6f);

						SingleCamera::GetInstance()->PlayCameraShake(0.2f, 0.1f);

					}
				}
			}
		}
	}
}

void MovableWall::MovableWallIdle()
{
	
}

void MovableWall::MovableWallMove()
{
	_vec3 dir = m_vDesiredPos - Get_Transform()->Get_Info(INFO_POS);
	D3DXVec3Normalize(&dir, &dir);

	Get_Transform()->Move_Forward(&dir, m_fDeltaTime, 3.f);

	SoundMgr::GetInstance()->Play_Sound(L"door_stone_open", SOUND_INTERACTION, 1.f);

	if (fabs(D3DXVec3Length(&(m_vDesiredPos - Get_Transform()->Get_Info(INFO_POS)))) < 1.f)
	{
		Get_Transform()->Set_Pos(m_vDesiredPos);
		m_eIntState = INTSTATE_DESTROY;
		m_bIsOpened = true;
		if (m_eTriType & TRITYPE_BLOCK)
		{
			// 여기서 카운트 증가?
			pPlayer->Add_SealCount();
		}
	}
}

HRESULT MovableWall::Add_Component(const wstring& textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void MovableWall::Free()
{
	Super::Free();
}
