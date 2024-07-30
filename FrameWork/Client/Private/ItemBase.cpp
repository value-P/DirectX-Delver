#include "stdafx.h"
#include "ItemBase.h"

#include "Export_System.h"
#include "Export_Utility.h"

ItemBase::ItemBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
}

ItemBase::ItemBase(const ItemBase& rhs)
	: Super(rhs)
{
	
}

ItemBase::~ItemBase()
{

}

HRESULT ItemBase::Init_GameObject()
{
	Super::Init_GameObject();

	m_ItemStat = new ItemStat();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int ItemBase::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = Super::Update_GameObject(fTimeDelta);

	if (!m_ItemStat->bIsActivated)
	{
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
	}

	return iExit;
}

void ItemBase::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void ItemBase::Render_GameObject()
{
	Super::Render_GameObject();
}

void ItemBase::Set_ItemStat(ItemStat* itemStat)
{
	m_ItemStat->wsItemName = itemStat->wsItemName;
	m_ItemStat->iItemID = itemStat->iItemID;
	m_ItemStat->eItemType = itemStat->eItemType;
	m_ItemStat->iItemPrice = itemStat->iItemPrice;
	m_ItemStat->iItemCount = itemStat->iItemCount;
	m_ItemStat->bIsStackable = itemStat->bIsStackable;
	m_ItemStat->bIsActivated = itemStat->bIsActivated;

	m_ItemStat->iAttack = itemStat->iAttack;
	m_ItemStat->iDefense = itemStat->iDefense;
	m_ItemStat->iHealAmount = itemStat->iHealAmount;
}

bool ItemBase::Increase_ItemCount()
{
	if (m_ItemStat->bIsStackable == false)
		return false;

	m_ItemStat->iItemCount++;
	/*if (m_ItemStat->iItemCount > 99)
	{
		m_ItemStat->iItemCount = 99;
		return false;
	}*/

	return true;
}

bool ItemBase::Decrease_ItemCount()
{
	m_ItemStat->iItemCount--;
	if (m_ItemStat->iItemCount < 1)
	{
		m_ItemStat->iItemCount = 0;
		return false;
	}

	return true;
}

HRESULT ItemBase::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Collider",pComponent });
	m_pColliderCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void ItemBase::PlaneCollision()
{
	// OBB와 구 충돌만 적용
	// 구와 가장 가까운 점이 VECTOR3 형식으로 들어가 있다~
	list<pair<CINFO, D3DXVECTOR3>>::const_iterator iter;

	for (iter = m_pColliderCom->GetColOBB_Info(COL_WALL).begin(); iter != m_pColliderCom->GetColOBB_Info(COL_WALL).end(); iter++)
	{
		D3DXVECTOR3 Vec{};
		D3DXVec3Normalize(&Vec, &(m_pTransformCom->Get_Info(INFO_POS) - (*iter).second));
		m_pTransformCom->Set_Pos(((*iter).second + Vec * m_pColliderCom->GetRadius()));

		//D3DXVECTOR3 PlayerPos = m_pTransformCom->Get_Info(INFO_POS);
		//if (D3DXVec3LengthSq(&(PlayerPos - (*iter).second)) < m_pColliderCom->GetRadius()* m_pColliderCom->GetRadius())
		//{
		//	D3DXVECTOR3 normal{};
		//	D3DXVec3Normalize(&normal, &((*iter).second - PlayerPos));
		//	float fRadian = D3DXVec3Dot(&normal, &D3DXVECTOR3{ 0.0f,-1.0f,0.0f });
		//	if (fRadian >= cos(D3DX_PI * 0.25f))
		//	{
		//		m_pTransformCom->Set_Pos(PlayerPos.x, (*iter).second.y + m_pColliderCom->GetRadius(), PlayerPos.z);
		//	}
		//	else if (fRadian > cos(D3DX_PI * 0.25f) * -1.0f && fRadian < cos(D3DX_PI * 0.25f))
		//	{
		//		m_pTransformCom->Set_Pos((*iter).second.x + normal.x * -1.0f * m_pColliderCom->GetRadius(),
		//			PlayerPos.y,
		//			(*iter).second.z + normal.z * -1.0f * m_pColliderCom->GetRadius());
		//		/*m_pTransformCom->Set_Pos((*iter).second.x + normal.x * -1.0f * m_pColliderCom->GetRadius(),
		//			(*iter).second.y + m_pColliderCom->GetRadius(),
		//			(*iter).second.z + normal.z * -1.0f * m_pColliderCom->GetRadius());*/
		//	}
		//}
	}
}

void ItemBase::TrackingPlayer()
{
	if (m_ItemStat->bIsActivated)
	{
		Transform* pTransform = dynamic_cast<Transform*>(Management::GetInstance()->Get_Component(L"GameObject", L"Player", TAG_TRANSFORM, ID_DYNAMIC));

		m_pTransformCom->Set_Pos(pTransform->Get_Info(INFO_POS));
	}
}

void ItemBase::Free()
{
	if (m_ItemStat)
	{
		delete m_ItemStat;
		m_ItemStat = nullptr;
	}

	Super::Free();
}
