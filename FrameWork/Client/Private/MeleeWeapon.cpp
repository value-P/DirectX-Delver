#include "stdafx.h"
#include "MeleeWeapon.h"
#include "MeleeAttack.h"

#include "Export_System.h"
#include "Export_Utility.h"

MeleeWeapon::MeleeWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

MeleeWeapon::MeleeWeapon(const MeleeWeapon& rhs)
	: Super(rhs)
{
}

MeleeWeapon::~MeleeWeapon()
{
}

HRESULT MeleeWeapon::Init_GameObject()
{
	Super::Init_GameObject();

	return S_OK;
}

_int MeleeWeapon::Update_GameObject(const _float& fTimeDelta)
{
	m_fDeltaTime = fTimeDelta;

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
	else if (m_pColliderCom->GetColLayer() & COL_DROPITEM)
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

void MeleeWeapon::LateUpdate_GameObject()
{
	if (m_pColliderCom->GetColOBB() & COL_WALL)
	{
		PlaneCollision();
	}

	Super::LateUpdate_GameObject();
}

void MeleeWeapon::Render_GameObject()
{
	if (m_pColliderCom->GetColLayer() & COL_PLAYERATTACK)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Set_Texture(0);
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

void MeleeWeapon::StateIdle()
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
	m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
	m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
	m_vInfo[INFO_POS] = { 0.3f,-0.3f,0.8f };

	_matrix		matRot[ROT_END];

	//m_vAngle.x = D3DX_PI * 0.05f;
	m_vAngle.y = D3DX_PI * 0.35f;
	//m_vAngle.z = D3DX_PI * 0.15f;

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

void MeleeWeapon::StateMove()
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

void MeleeWeapon::StateWeak()
{
	if (m_bIsAttacking == false)
	{
		SoundMgr::GetInstance()->Stop_Sound(SOUND_ATK_SWORD);
		m_bIsAttacking = true;
	}

	m_fMeleeTime += m_fDeltaTime;
	if (m_fMeleeTime <= 0.3f)
	{
		m_vInfo[INFO_POS] += _vec3{ 0.f, 0.0f, 0.03f };

		for (_uint i = 0; i < INFO_END; ++i)
			memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

	if (m_fMeleeTime > 0.3f)
	{
		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"attack_03", SOUND_ATK_SWORD, 0.6f);

		Transform* transform = dynamic_cast<Transform*>(Engine::Get_Component(L"GameObject", L"Player", TAG_TRANSFORM, ID_DYNAMIC));
		D3DXVECTOR3 Pos = transform->Get_Info(INFO_POS);
		D3DXVECTOR3 Dir = transform->Get_Info(INFO_LOOK);

		MeleeAttack* meleeAttack = MeleeAttack::Create(m_pGraphicDev, Pos + Dir, Dir, 0.4f, m_ItemStat->iAttack);
		meleeAttack->SetColType(COL_PLAYERATTACK);

		Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
			Add_GameObject(L"MeleeAttack", meleeAttack);

		m_eState = WEAPON_IDLE;
		m_fMeleeTime = 0.f;
		m_bIsAttacking = false;

		D3DXMatrixIdentity(&m_matWorld);

		m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
		m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
		m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
		m_vInfo[INFO_POS] = { 0.3f,-0.3f, 0.8f };

		m_vAngle.x = 0.f;
		m_vAngle.z = 0.f;
	}
}

void MeleeWeapon::StateStrong()
{
	if (m_bIsAttacking == false)
	{
		m_bIsAttacking = true;
		SoundMgr::GetInstance()->Stop_Sound(SOUND_ATK_SWORD);
	}

	m_fMeleeTime += m_fDeltaTime;

	if (m_fMeleeTime <= 0.5f)
	{
		m_vInfo[INFO_POS] += _vec3{ 0.015f, 0.015f, -0.008f };

		_matrix		matRot[ROT_END];

		m_vAngle.x = D3DXToRadian(-3.5f);
		m_vAngle.y = 0.0f; //D3DXToRadian(2.f);
		m_vAngle.z = D3DXToRadian(-1.f);

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
	
	else if (m_fMeleeTime >= 0.5f && m_fMeleeTime < 0.7f)
	{
		m_vInfo[INFO_POS] += _vec3{ -0.03f, -0.03f, 0.04f };

		_matrix		matRot[ROT_END];

		m_vAngle.x = D3DXToRadian(12.f);
		m_vAngle.y = D3DXToRadian(-2.f);
		m_vAngle.z = D3DXToRadian(-3.f);

		D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
		D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
		D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

		SoundMgr::GetInstance()->Play_Sound(L"wep_swing_heavy_01", SOUND_ATK_SWORD, 0.6f);

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
	
	else if (m_fMeleeTime >= 0.7f && m_fMeleeTime < 0.9f)
	{
		m_vInfo[INFO_POS] += _vec3{ -0.02f, 0.f, -0.03f };

		_matrix		matRot[ROT_END];

		m_vAngle.x = D3DXToRadian(-2.5f);
		m_vAngle.y = D3DXToRadian(-0.8f);
		m_vAngle.z = D3DXToRadian(-2.8f);

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

	else if (m_fMeleeTime >= 0.9f && m_fMeleeTime < 1.1f)
	{
		m_vInfo[INFO_POS] += _vec3{ -0.03f, 0.f, -0.03f };

		_matrix		matRot[ROT_END];

		m_vAngle.x = 0.f;
		m_vAngle.y = D3DXToRadian(-6.f);
		m_vAngle.z = 0.f;

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

	else if (m_fMeleeTime >= 1.1f)
	{
		m_eState = WEAPON_IDLE;
		m_fMeleeTime = 0.f;
		m_bIsAttacking = false;

		D3DXMatrixIdentity(&m_matWorld);

		m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
		m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
		m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
		m_vInfo[INFO_POS] = { 0.3f,-0.3f,0.8f };

		m_vAngle.x = 0.f;
		m_vAngle.z = 0.f;
	}
}

void MeleeWeapon::OnCollided()
{

}

HRESULT MeleeWeapon::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void MeleeWeapon::Refresh()
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
	m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
	m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
	m_vInfo[INFO_POS] = { 0.3f,-0.3f,0.8f };

	_matrix		matRot[ROT_END];

	m_vAngle.x = 0.f;
	m_vAngle.y = D3DX_PI * 0.35f;
	m_vAngle.z = 0.f;

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

MeleeWeapon* MeleeWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType,
	_uint itemPrice, _uint itemCount, _bool isStackable, _bool isActivated, _uint attack, _uint defense)
{
	MeleeWeapon* pInstance = new MeleeWeapon(pGraphicDev);
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

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	ItemStat itemStat = ItemStat(itemName, itemID, itemType, itemPrice, itemCount, isStackable, isActivated, attack, defense);
	pInstance->Set_ItemStat(&itemStat);

	return pInstance;
}

ItemBase* MeleeWeapon::Clone()
{
	MeleeWeapon* outItem = MeleeWeapon::Create(m_pGraphicDev, m_textureKey, m_pColliderCom->GetSize(), static_cast<COLLAYER>(m_pColliderCom->GetColLayer()),
		m_ItemStat->wsItemName, m_ItemStat->iItemID, m_ItemStat->eItemType, m_ItemStat->iItemPrice, m_ItemStat->iItemCount,
		m_ItemStat->bIsStackable, m_ItemStat->bIsActivated, m_ItemStat->iAttack, m_ItemStat->iDefense);

	return outItem;
}

void MeleeWeapon::Free()
{
	Super::Free();
}
