#include "stdafx.h"
#include "Consumable.h"

#include "Export_Utility.h"

Consumable::Consumable(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

Consumable::Consumable(const Consumable& rhs)
	: Super(rhs)
{
}

Consumable::~Consumable()
{
}

HRESULT Consumable::Init_GameObject()
{
	Super::Init_GameObject();

	return S_OK;
}

_int Consumable::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Super::Update_GameObject(fTimeDelta);

	// Late Update 후부터 다음 프레임의 Update_GameObject 전까지 충돌 된거에 대한 정보를 가지고 있다

	_matrix matWorld, matView, matBill;
	matWorld = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 빌 * (스 * 자 * 이)

	_matrix newMatWorld = matBill * matWorld;

	m_pTransformCom->Set_WorldMatrix(&newMatWorld);

	Engine::AddCollider(m_pColliderCom);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void Consumable::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void Consumable::Render_GameObject()
{
}

Consumable* Consumable::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType,
	_uint itemPrice, _uint itemCount, _bool isStackable, _bool isActivated, _uint attack, _uint defense, _uint healAmount)
{
	Consumable* pInstance = new Consumable(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Consumable Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Consumable Create Failed");
		return nullptr;
	}

	pInstance->Set_ItemTextureKey(textureKey);

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	ItemStat itemStat = ItemStat(itemName, itemID, itemType, itemPrice, itemCount, isStackable, isActivated, attack, defense, healAmount);
	pInstance->Set_ItemStat(&itemStat);

	return pInstance;
}

ItemBase* Consumable::Clone()
{
	ItemBase* outItem = Consumable::Create(m_pGraphicDev, m_textureKey, m_pColliderCom->GetSize(), static_cast<COLLAYER>(m_pColliderCom->GetColLayer()),
		m_ItemStat->wsItemName, m_ItemStat->iItemID, m_ItemStat->eItemType, m_ItemStat->iItemPrice, m_ItemStat->iItemCount,
		m_ItemStat->bIsStackable, m_ItemStat->bIsActivated, m_ItemStat->iAttack, m_ItemStat->iDefense);

	return outItem;
}

HRESULT Consumable::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	return S_OK;
}

void Consumable::Free()
{
	Super::Free();
}
