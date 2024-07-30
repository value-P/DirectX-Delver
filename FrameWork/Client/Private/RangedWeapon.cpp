#include "stdafx.h"
#include "RangedWeapon.h"
#include "Arrow.h"

#include "Export_Utility.h"
#include "Export_System.h"

RangedWeapon::RangedWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

RangedWeapon::RangedWeapon(const RangedWeapon& rhs)
	: Super(rhs)
{
}

RangedWeapon::~RangedWeapon()
{
}

HRESULT RangedWeapon::Init_GameObject()
{
	Super::Init_GameObject();

	m_pColliderCom->SetThisPointer(this);

	return S_OK;
}

_int RangedWeapon::Update_GameObject(const _float& fTimeDelta)
{
	m_fDeltaTime = fTimeDelta;

	if (m_pColliderCom->GetColLayer() & COL_DROPITEM)
	{
		_int iExit = Super::Update_GameObject(fTimeDelta);

		if (m_bIsDeleted)
		{
			return -1;
		}

		Engine::Add_RenderGroup(RENDER_NONALPHA, this);

		AddCollider(m_pColliderCom);
	}

	return 0;
}

void RangedWeapon::LateUpdate_GameObject()
{
	if (m_pColliderCom->GetColLayer() & COL_PLAYERATTACK)
	{
		switch (m_eState)
		{
		case WEAPON_IDLE:
		{
			StateIdle();
		}
		break;
		case WEAPON_MOVE:
		{
			StateMove();
		}
		break;
		case WEAPON_WEAK:
		{
			StateWeak();
		}
		break;
		case WEAPON_STRONG:
		{
			StateStrong();
		}
		break;
		}
	}

	else if (m_pColliderCom->GetColOBB() & COL_WALL)
	{
		PlaneCollision();
	}

	Super::LateUpdate_GameObject();
}

void RangedWeapon::Render_GameObject()
{
	if (m_pColliderCom->GetColLayer() & COL_PLAYERATTACK)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Set_Texture(static_cast<_uint>(m_fFrame));
		m_pBufferCom->Render_Buffer();
	}
	else if (m_pColliderCom->GetColLayer() & COL_DROPITEM)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

}

void RangedWeapon::StateIdle()
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
	m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
	m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
	m_vInfo[INFO_POS] = { 0.2f,-0.4f,0.7f };

	_matrix		matRot[ROT_END];

	m_vAngle.x = D3DX_PI * 0.09f;
	m_vAngle.y = (D3DX_PI * 0.4f);
	m_vAngle.z = D3DX_PI * 0.03f;

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
}

void RangedWeapon::StateMove()
{
	if (m_bIsDirUp)
	{
		m_fShakingTime += m_fDeltaTime;
		if (m_fShakingTime >= 0.4f)
		{
			m_fShakingTime = 0.f;
			m_bIsDirUp = false;
		}
		m_vInfo[INFO_POS] += { 0.f, 0.002f, 0.f };
	}
	else
	{
		m_fShakingTime += m_fDeltaTime;
		if (m_fShakingTime >= 0.4f)
		{
			m_fShakingTime = 0.f;
			m_bIsDirUp = true;
		}
		m_vInfo[INFO_POS] += { 0.f, -0.002f, 0.f };
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
}

void RangedWeapon::StateWeak()
{
	if (m_bArrowReady == true)
	{
		Transform* playerPos = dynamic_cast<Transform*>(Engine::Get_Component(L"GameObject", L"Player", TAG_TRANSFORM, ID_DYNAMIC));
		if (playerPos)
		{
			Arrow* arrow = Arrow::Create(m_pGraphicDev, playerPos->Get_Info(INFO_POS) + playerPos->Get_Info(INFO_LOOK), playerPos->Get_Info(INFO_LOOK), /*속도*/45.f, /*데미지*/m_ItemStat->iAttack);
			arrow->SetEffectType(EFFECT_ARROW);
			arrow->SetColType(COL_PLAYER_PROJ);

			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"Arrow", arrow);

			SingleCamera::GetInstance()->PlayCameraShake(0.1f, 0.03f);

			m_eState = WEAPON_IDLE;
			m_fRangedTime = 0.f;
			m_fFrame = 0.f;

			Refresh();
			m_bArrowReady = false;
			m_bIsAttacking = false;
		}
	}
	else
	{
		return;
	}
}

void RangedWeapon::StateStrong()
{
	if (m_bIsAttacking == false)
	{
		m_bIsAttacking = true;
		SoundMgr::GetInstance()->Stop_Sound(SOUND_ATK_BOW);
	}

	m_fRangedTime += m_fDeltaTime;

	if (m_fRangedTime < 0.2f)
	{
		m_vInfo[INFO_POS] += _vec3{ 0.f, 0.003f, 0.f };

		_matrix		matRot[ROT_END];

		m_vAngle.x = D3DXToRadian(2.4f);
		m_vAngle.y = D3DXToRadian(1.1f);
		m_vAngle.z = D3DXToRadian(1.1f);

		D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
		D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
		D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

		for (_uint i = 0; i < INFO_POS; ++i)
		{
			for (_uint j = 0; j < ROT_END; ++j)
			{
				D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
			}
		}

		for (_uint i = 0; i < INFO_END; ++i)
			memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

	if (m_fRangedTime >= 0.2f && m_fRangedTime < 0.5f)
	{
		m_vInfo[INFO_POS] += _vec3{ 0.f, 0.003f, 0.f };

		SoundMgr::GetInstance()->Play_Sound(L"wep_swing_bow_load_04", SOUND_ATK_BOW, 0.6f);

		for (_uint i = 0; i < INFO_END; ++i)
			memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

	if (m_fRangedTime >= 0.5f)
	{
		m_fFrame += 0.2f;
		if (m_fFrame >= 3)
		{
			m_fFrame = 3;
			m_bArrowReady = true;
		}
	}
}

void RangedWeapon::Refresh()
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
	m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
	m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
	m_vInfo[INFO_POS] = { 0.2f,-0.4f,0.7f };

	_matrix		matRot[ROT_END];

	m_vAngle.x = D3DX_PI * 0.09f;
	m_vAngle.y = (D3DX_PI * 0.4f);
	m_vAngle.z = D3DX_PI * 0.03f;

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
}

HRESULT RangedWeapon::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void RangedWeapon::OnCollided()
{
}

RangedWeapon* RangedWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID,
	ITEMTYPE itemType, _uint itemPrice, _uint itemCount, _bool isStackable, _bool isActivated, _uint attack, _uint defense)
{
	RangedWeapon* pInstance = new RangedWeapon(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MeleeWeapon Create Failed");

		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MeleeWeapon Create Failed");

		return nullptr;
	}

	pInstance->Set_ItemTextureKey(textureKey);

	pInstance->Set_ItemTextureKey(textureKey);

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	ItemStat itemStat = ItemStat(itemName, itemID, itemType, itemPrice, itemCount, isStackable, isActivated, attack, defense);
	pInstance->Set_ItemStat(&itemStat);

	return pInstance;
}

ItemBase* RangedWeapon::Clone()
{
	RangedWeapon* outItem = RangedWeapon::Create(m_pGraphicDev, m_textureKey, m_pColliderCom->GetSize(), static_cast<COLLAYER>(m_pColliderCom->GetColLayer()),
		m_ItemStat->wsItemName, m_ItemStat->iItemID, m_ItemStat->eItemType, m_ItemStat->iItemPrice, m_ItemStat->iItemCount,
		m_ItemStat->bIsStackable, m_ItemStat->bIsActivated, m_ItemStat->iAttack, m_ItemStat->iDefense);

	return outItem;
}

void RangedWeapon::Free()
{
	Super::Free();
}
