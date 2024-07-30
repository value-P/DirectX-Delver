#include "stdafx.h"
#include "Shield.h"

#include "Export_Utility.h"

Shield::Shield(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

Shield::Shield(const Shield& rhs)
	: Super(rhs)
{
}

Shield::~Shield()
{
}

HRESULT Shield::Init_GameObject()
{
	Super::Init_GameObject();

	m_ItemState = ItemState::ITEMSTATE_IDLE;

	return S_OK;
}

_int Shield::Update_GameObject(const _float& fTimeDelta)
{
	m_fDeltaTime = fTimeDelta;

	// Late Update 후부터 다음 프레임의 Update_GameObject 전까지 충돌 된거에 대한 정보를 가지고 있다

	if (m_pColliderCom->GetColLayer() & COL_DROPITEM)
	{
		_int iExit = Super::Update_GameObject(fTimeDelta);

		if (m_bIsDeleted)
		{
			return -1;
		}

		Engine::Add_RenderGroup(RENDER_NONALPHA, this);

		AddCollider(m_pColliderCom);

		return iExit;
	}

	return 0;
}

void Shield::LateUpdate_GameObject()
{
	switch (m_ItemState)
	{
	case ItemState::ITEMSTATE_IDLE:
	{
		StateIdle();
	}
	break;
	case ItemState::ITEMSTATE_MOVE:
	{
		StateMove();
	}
	break;
	case ItemState::ITEMSTATE_WEAK:
	{
		StateWeak();
	}
	break;
	case ItemState::ITEMSTATE_STRONG:
	{
		StateStrong();
	}
	break;
	}

	Super::LateUpdate_GameObject();
}

void Shield::Render_GameObject()
{
	if (m_pColliderCom->GetColLayer() & COL_PLAYERATTACK)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Set_Texture(1);
		m_pBufferCom->Render_Buffer();
	}
	else if (m_pColliderCom->GetColLayer() & COL_DROPITEM)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();
	}
}

Shield* Shield::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType,
	_uint itemPrice, _uint itemCount, _bool isStackable, _bool isActivated, _uint attack, _uint defense)
{
	Shield* pInstance = new Shield(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Shield Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Shield Create Failed");
		return nullptr;
	}

	pInstance->Set_ItemTextureKey(textureKey);

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);

	ItemStat itemStat = ItemStat(itemName, itemID, itemType, itemPrice, itemCount, isStackable, isActivated, attack, defense);
	pInstance->Set_ItemStat(&itemStat);

	return pInstance;
}

ItemBase* Shield::Clone()
{
	Shield* outItem = Shield::Create(m_pGraphicDev, m_textureKey, m_pColliderCom->GetSize(), static_cast<COLLAYER>(m_pColliderCom->GetColLayer()),
		m_ItemStat->wsItemName, m_ItemStat->iItemID, m_ItemStat->eItemType, m_ItemStat->iItemPrice, m_ItemStat->iItemCount,
		m_ItemStat->bIsStackable, m_ItemStat->bIsActivated, m_ItemStat->iAttack, m_ItemStat->iDefense);

	return outItem;
}

void Shield::StateIdle()
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vInfo[INFO_RIGHT] = { 0.3f,0.0f,0.0f };
	m_vInfo[INFO_UP] = { 0.0f,0.3f,0.0f };
	m_vInfo[INFO_LOOK] = { 0.0f,0.0f,0.3f };
	m_vInfo[INFO_POS] = { -0.3f, -0.5f, 0.8f };

	_matrix		matRot[ROT_END];

	//m_vAngle.x = D3DX_PI * 0.09f;
	m_vAngle.y = D3DXToRadian(-30.f);
	//m_vAngle.z = D3DX_PI * 0.03f;

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

void Shield::StateMove()
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

void Shield::StateWeak()
{
}

void Shield::StateStrong()
{
}

HRESULT Shield::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void Shield::OnCollided()
{
}

void Shield::Free()
{
	Super::Free();
}
