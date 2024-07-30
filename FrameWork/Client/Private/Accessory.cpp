#include "stdafx.h"
#include "Accessory.h"

#include "Export_Utility.h"

Accessory::Accessory(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

Accessory::Accessory(const Accessory& rhs)
	: Super(rhs)
{
}

Accessory::~Accessory()
{
}

HRESULT Accessory::Init_GameObject()
{
	Super::Init_GameObject();

	return S_OK;
}

_int Accessory::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Super::Update_GameObject(fTimeDelta);

	if (m_bIsDeleted)
	{
		return -1;
	}

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void Accessory::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void Accessory::Render_GameObject()
{
	
}

Accessory* Accessory::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType,
	_uint itemPrice, _uint itemCount, _bool isStackable, _bool isActivated, _uint attack, _uint defense)
{
	Accessory* pInstance = new Accessory(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Accessory Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Accessory Create Failed");
		return nullptr;
	}

	pInstance->Set_ItemTextureKey(textureKey);

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);

	ItemStat itemStat = ItemStat(itemName, itemID, itemType, itemPrice, itemCount, isStackable, isActivated, attack, defense);
	pInstance->Set_ItemStat(&itemStat);

	return pInstance;
}

ItemBase* Accessory::Clone()
{
	Accessory* outAcc = Accessory::Create(m_pGraphicDev, m_textureKey, m_pColliderCom->GetSize(), static_cast<COLLAYER>(m_pColliderCom->GetColLayer()),
		m_ItemStat->wsItemName, m_ItemStat->iItemID, m_ItemStat->eItemType, m_ItemStat->iItemPrice, m_ItemStat->iItemCount,
		m_ItemStat->bIsStackable, m_ItemStat->bIsActivated, m_ItemStat->iAttack, m_ItemStat->iDefense);

	return outAcc;
}

HRESULT Accessory::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void Accessory::OnCollided()
{
}

void Accessory::Free()
{
	Super::Free();
}
