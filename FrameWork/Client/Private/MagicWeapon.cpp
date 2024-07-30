#include "stdafx.h"
#include "MagicWeapon.h"
#include "Magic.h"

#include "Export_Utility.h"
#include "Export_System.h"

MagicWeapon::MagicWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

MagicWeapon::MagicWeapon(const MagicWeapon& rhs)
	: Super(rhs)
{
}

MagicWeapon::~MagicWeapon()
{
}

HRESULT MagicWeapon::Init_GameObject()
{
	Super::Init_GameObject();

	return S_OK;
}

_int MagicWeapon::Update_GameObject(const _float& fTimeDelta)
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

void MagicWeapon::LateUpdate_GameObject()
{
	if (m_pColliderCom->GetColOBB() & COL_WALL)
	{
		PlaneCollision();
	}

	Super::LateUpdate_GameObject();
}

void MagicWeapon::Render_GameObject()
{
	if (m_pColliderCom->GetColLayer() & COL_PLAYERATTACK)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		if (m_eType == MWT_MAGIC_1)
		{
			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();
		}

		else if (m_eType == MWT_MAGIC_0)
		{
			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();
		}
	}
	else if (m_pColliderCom->GetColLayer() & COL_DROPITEM)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		if (m_eType == MWT_MAGIC_1)
		{
			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();
		}

		else if (m_eType == MWT_MAGIC_0)
		{
			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();
		}

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

void MagicWeapon::StateIdle()
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
	m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
	m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
	m_vInfo[INFO_POS] = { 0.3f,-0.35f,0.7f };

	_matrix		matRot[ROT_END];

	//m_vAngle.y = D3DX_PI * 0.35f;

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

void MagicWeapon::StateMove()
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

void MagicWeapon::StateWeak()
{
	if (m_bIsAttacking == false)
		m_bIsAttacking = true;

	m_vInfo[INFO_POS] += _vec3{ 0.f, 0.f, -0.005f };

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	m_fMagicTime += m_fDeltaTime;
	if (m_fMagicTime >= 0.5f)
	{
		Transform* transform = dynamic_cast<Transform*>(Engine::Get_Component(L"GameObject", L"Player", TAG_TRANSFORM, ID_DYNAMIC));

		if (m_eType == MWT_MAGIC_0)
		{
			Magic* magic = Magic::Create(m_pGraphicDev, MAGIC_0, MAGIC_EX_0, transform->Get_Info(INFO_POS) + transform->Get_Info(INFO_LOOK), transform->Get_Info(INFO_LOOK), 20.f, m_ItemStat->iAttack, D3DXCOLOR{ 0.f, 0.f, 1.f, 1.f });
			magic->SetEffectType(EFFECT_MAGIC);
			magic->SetColType(COL_PLAYER_PROJ);

			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"Magic", magic);

			SoundMgr::GetInstance()->Play_Sound_NoInterval(L"mg_ice_shoot_04", SOUND_ATK_MAGIC, 0.6f);
		}

		else if (m_eType == MWT_MAGIC_1)
		{
			Magic* magic = Magic::Create(m_pGraphicDev, MAGIC_1, MAGIC_EX_2, transform->Get_Info(INFO_POS) + transform->Get_Info(INFO_LOOK), transform->Get_Info(INFO_LOOK), 20.0f, m_ItemStat->iAttack, D3DXCOLOR{ 1.0f,0.0f,0.0f,1.0f });
			magic->SetEffectType(EFFECT_MAGIC);
			magic->SetColType(COL_PLAYER_PROJ);

			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"Magic", magic);

			SoundMgr::GetInstance()->Play_Sound_NoInterval(L"mg_fire_shoot_01", SOUND_ATK_MAGIC, 0.6f);
		}

		m_eState = WEAPON_IDLE;
		m_fMagicTime = 0.f;
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

void MagicWeapon::StateStrong()
{
}

HRESULT MagicWeapon::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void MagicWeapon::OnCollided()
{

}

MagicWeapon* MagicWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID,
	ITEMTYPE itemType, _uint itemPrice, MagicWeaponType magicType, _uint itemCount, _bool isStackable, _bool isActivated, _uint attack, _uint defense)
{
	MagicWeapon* pInstance = new MagicWeapon(pGraphicDev);
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

	pInstance->SetMagicWeaponType(magicType);

	ItemStat itemStat = ItemStat(itemName, itemID, itemType, itemPrice, itemCount, isStackable, isActivated, attack, defense);
	pInstance->Set_ItemStat(&itemStat);

	return pInstance;
}

ItemBase* MagicWeapon::Clone()
{
	MagicWeapon* outItem = MagicWeapon::Create(m_pGraphicDev, m_textureKey, m_pColliderCom->GetSize(), static_cast<COLLAYER>(m_pColliderCom->GetColLayer()),
		m_ItemStat->wsItemName, m_ItemStat->iItemID, m_ItemStat->eItemType, m_ItemStat->iItemPrice, m_eType, m_ItemStat->iItemCount,
		m_ItemStat->bIsStackable, m_ItemStat->bIsActivated, m_ItemStat->iAttack, m_ItemStat->iDefense);

	return outItem;
}

void MagicWeapon::Free()
{
	Super::Free();
}
