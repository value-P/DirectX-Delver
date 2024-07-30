#include "stdafx.h"
#include "AutoTrack.h"
#include "Player.h"

#include "Export_Utility.h"

AutoTrack::AutoTrack(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{

}

AutoTrack::AutoTrack(const AutoTrack& rhs)
	: Super(rhs)
{

}

AutoTrack::~AutoTrack()
{
	
}

HRESULT AutoTrack::Init_GameObject()
{
	Super::Init_GameObject();

	m_eIntState = INTSTATE_IDLE;

	return S_OK;
}

Engine::_int AutoTrack::Update_GameObject(const _float& fTimeDelta)
{
	m_fDeltaTime = fTimeDelta;

	_int	iExit = Super::Update_GameObject(fTimeDelta);

	AddCollider(m_pColliderCom);

	return iExit;
}

void AutoTrack::LateUpdate_GameObject()
{
	switch (m_eIntState)
	{
	case INTSTATE_IDLE:
	{
		AutoTrackIdle();
	}
	break;
	case INTSTATE_MOVE:
	{
		AutoTrackMove();
	}
	break;
	case INTSTATE_AUTOMOVE:
	{
		AutoTrackAutoMove();
	}
	break;
	}

	OnCollided();

	Super::LateUpdate_GameObject();
}

void AutoTrack::Render_GameObject()
{
	
}

void AutoTrack::Set_DesiredPos(_vec3& desiredPos)
{
	m_vDesiredPos = Get_Transform()->Get_Info(INFO_POS) + desiredPos;
}

void AutoTrack::Set_DesiredPos(InteractableDir intDir)
{
	switch (intDir)
	{
	case INTDIR_FRONT:
	{
		Set_DesiredPos(_vec3(0.f, 0.f, 1.f));
	}
	break;
	case INTDIR_RIGHT:
	{
		Set_DesiredPos(_vec3(1.f, 0.f, 0.f));
	}
	break;
	case INTDIR_BACK:
	{
		Set_DesiredPos(_vec3(0.f, 0.f, -1.f));
	}
	break;
	case INTDIR_LEFT:
	{
		Set_DesiredPos(_vec3(-1.f, 0.f, 0.f));
	}
	break;
	case INTDIR_UP:
	{
		Set_DesiredPos(_vec3(0.f, 1.f, 0.f));
	}
	break;
	case INTDIR_DOWN:
	{
		Set_DesiredPos(_vec3(0.f, -1.f, 0.f));
	}
	break;
	}
}

AutoTrack* AutoTrack::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, InteractableDir intDir, InteractableType intType, _vec3& startPos, bool bIsLastTrack)
{
	AutoTrack* pInstance = new AutoTrack(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("AutoTrack Create Failed");
		return nullptr;
	}

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	pInstance->Get_Transform()->Set_Pos(initPos);
	pInstance->Set_IntDir(intDir);
	pInstance->Set_IntType(intType);

	pInstance->Set_DesiredPos(intDir);

	pInstance->SetIsLastTrack(bIsLastTrack);

	pInstance->Set_StartPos(startPos);

	return pInstance;
}

void AutoTrack::OnCollided()
{
	if (m_pColliderCom->GetColSphere() & COL_PLAYER)
	{
		list<pair<CINFO, _vec3>> playerList = m_pColliderCom->GetColSphere_Info(COL_PLAYER);
		for (auto& it : playerList)
		{
			Player* player = static_cast<Player*>(it.first.m_pThis);
			if (player)
			{
				_vec3 vDir = player->Get_Transform()->Get_Info(INFO_POS) - m_vStartPos;
				float curDist = D3DXVec3Length(&vDir);
				if (fabs(curDist) < 2.f)
				{
					if (m_bIsLastTrack == false)
					{
						player->SetIsOnTrack(true);
						if (m_bIsOnTrack == false)
						{
							m_eIntState = INTSTATE_MOVE;
						}
					}
					else
					{
						player->SetIsOnTrack(false);
					}
				}
			}
		}
	}
	else
	{
		m_bIsOnTrack = false;
	}
}

void AutoTrack::AutoTrackIdle()
{
	
}

void AutoTrack::AutoTrackMove()
{
	m_bIsOnTrack = true;

	Transform* playerTrans = dynamic_cast<Transform*>(Engine::Get_Component(L"GameObject", L"Player", TAG_TRANSFORM, ID_DYNAMIC));
	if (playerTrans)
	{
		_vec3 dir = m_vStartPos - playerTrans->Get_Info(INFO_POS);
		D3DXVec3Normalize(&dir, &dir);

		playerTrans->Move_Forward(&dir, m_fDeltaTime, 5.f);

		if (fabs(D3DXVec3Length(&(m_vStartPos - playerTrans->Get_Info(INFO_POS)))) < 1.f)
		{
			playerTrans->Set_Pos(m_vStartPos);
			m_eIntState = INTSTATE_AUTOMOVE;
		}
	}
}

void AutoTrack::AutoTrackAutoMove()
{
	if (m_bIsOnTrack)
	{
		_vec3 dir = m_vDesiredPos - Get_Transform()->Get_Info(INFO_POS);
		D3DXVec3Normalize(&dir, &dir);

		Transform* playerTrans = dynamic_cast<Transform*>(Engine::Get_Component(L"GameObject", L"Player", TAG_TRANSFORM, ID_DYNAMIC));
		if (playerTrans)
		{
			playerTrans->Move_Forward(&dir, m_fDeltaTime, 5.f);
		}
	}

	//m_fResetTime += m_fDeltaTime;
	//if (m_fResetTime >= 60.f)
	//{
	//	m_eIntState = INTSTATE_IDLE;
	//	m_fResetTime = 0.f;
	//	m_bIsOnTrack = false;
	//}
}

HRESULT AutoTrack::Add_Component(const wstring& textureKey)
{

	return S_OK;
}

void AutoTrack::Free()
{
	Super::Free();
}
