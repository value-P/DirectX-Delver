#include "stdafx.h"
#include "Gold.h"

#include "Export_Utility.h"
#include "Export_System.h"

Gold::Gold(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

Gold::Gold(const Gold& rhs)
	: Super(rhs)
{
}

Gold::~Gold()
{
}

HRESULT Gold::Init_GameObject()
{
	Super::Init_GameObject();

	return S_OK;
}

_int Gold::Update_GameObject(const _float& fTimeDelta)
{
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

void Gold::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void Gold::Render_GameObject()
{
	if (m_pColliderCom->GetColLayer() & COL_DROPITEM)
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

Gold* Gold::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice, _uint itemCount, _bool isStackable, _bool isActivated, _uint attack, _uint defense, _uint healAmount)
{
	Gold* pInstance = new Gold(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Gold Create Failed");

		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Gold Create Failed");

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

ItemBase* Gold::Clone()
{
	Gold* outItem = Gold::Create(m_pGraphicDev, m_textureKey, m_pColliderCom->GetSize(), static_cast<COLLAYER>(m_pColliderCom->GetColLayer()),
		m_ItemStat->wsItemName, m_ItemStat->iItemID, m_ItemStat->eItemType, m_ItemStat->iItemPrice, m_ItemStat->iItemCount,
		m_ItemStat->bIsStackable, m_ItemStat->bIsActivated, m_ItemStat->iAttack, m_ItemStat->iDefense);

	return outItem;
}

HRESULT Gold::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void Gold::Free()
{
	Super::Free();
}
