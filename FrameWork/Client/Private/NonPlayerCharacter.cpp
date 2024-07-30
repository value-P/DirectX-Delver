#include "stdafx.h"
#include "NonPlayerCharacter.h"
#include "TradeInv.h"
#include "InventoryBase.h"
#include "ItemBase.h"
#include "UI_ShopBG.h"
#include "UI_DialogBG.h"
#include "MyHUD.h"

#include "Export_Utility.h"

NonPlayerCharacter::NonPlayerCharacter(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

NonPlayerCharacter::NonPlayerCharacter(const NonPlayerCharacter& rhs)
	: Super(rhs)
{
}

NonPlayerCharacter::~NonPlayerCharacter()
{
}

HRESULT NonPlayerCharacter::Init_GameObject()
{
	Super::Init_GameObject();

	m_pColliderCom->SetSize(1.0f, 1.0f, 1.0f);
	m_pColliderCom->SetColLayer(COL_NPC);
	m_pColliderCom->SetThisPointer(this);

	return S_OK;
}

_int NonPlayerCharacter::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pMyHUD == nullptr)
		m_pMyHUD = dynamic_cast<MyHUD*>(UIManager::GetInstance()->GetHUD());

	m_fFrame += (fTimeDelta*5);

	if (m_fIdx_Frame < m_fFrame)
	{
		m_fFrame = 0.0f;
	}

	if (m_bIs_DialogClosed && m_eNpcType == NPC_TRADE)
	{
		
		m_bIs_DialogClosed = false;
		UIManager::GetInstance()->Init_UI(L"UI_ShopBG", UI_ShopBG::Create(m_pGraphicDev, m_pTradeInv, 0.f, 0.f, 1.f, 1.f));
		UIManager::GetInstance()->Activate_UI(L"UI_ShopBG");
	}

	if (m_bIs_DialogClosed && m_eNpcType == NPC_DIALOG && m_bQuest_NPC == true)
	{
		m_bQuest_NPC = false;
		m_bFontON_TechniqueTransfer = true;
		vector<vector<wstring>> pNPCDialogVec;
		pNPCDialogVec.push_back(
			{
				L"행운을 빕니다.모험가님!"
			});
		m_pNPCDialogVec = pNPCDialogVec;
	}
	_int	iExit = Super::Update_GameObject(fTimeDelta);


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

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void NonPlayerCharacter::LateUpdate_GameObject()
{
	_vec3 vPos{};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	Super::LateUpdate_GameObject();

	OnCollided();
}

void NonPlayerCharacter::Render_GameObject()
{

	if (m_bFontON_TechniqueTransfer)
	{
		Engine::Render_Font(L"Font_RoundShape_Get_Technique", L"로프를 획득하였습니다.", &_vec4(0, 200, 800, 250), DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, (m_fAlpha_Font / 255.f)));
		m_fAlpha_Font -= 2.5f;
		if (m_fAlpha_Font < 0)
		{
			m_bFontON_TechniqueTransfer = false;
			m_bFontON_TechniqueExplain = true;
			m_fAlpha_Font = 255.f;
		}
	}

	if (m_bFontON_TechniqueExplain)
	{
		Engine::Render_Font(L"Font_RoundShape_Get_Technique", L"로프는 R 버튼을 이용하여 사용할 수 있습니다.", &_vec4(0, 200, 800, 250), DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, (m_fAlpha_Font / 255.f)));
		m_fAlpha_Font -= 2.5f;
		if (m_fAlpha_Font < 0)
		{
			m_bFontON_TechniqueTransfer = false;
			m_bFontON_TechniqueExplain = false;
			m_fAlpha_Font = 255.f;
		}
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture(static_cast<_uint>(m_fFrame));
	m_pBufferCom->Render_Buffer();
	if (m_bIs_ColPlayer && (UIManager::GetInstance()->GetCurUI() == UIManager::GetInstance()->GetHUD()))
	{
		m_pMyHUD->Set_Close_CrossCursor(true);

		switch (m_eNpcType)
		{
		case Engine::NPC_TRADE:
			Engine::Render_Font(L"Font_RoundShape_NPC", L"F : 상점", &_vec4(0, 0, WINCX, WINCY), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			break;
		case Engine::NPC_DIALOG:
			Engine::Render_Font(L"Font_RoundShape_NPC", L"F : 대화하기", &_vec4(0, 0, WINCX, WINCY), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			break;
		}
		return;
	}
	m_pMyHUD->Set_Close_CrossCursor(false);

	
}

void NonPlayerCharacter::Talknpc()
{
	UIManager::GetInstance()->Init_UI(L"UI_DialogBG", UI_DialogBG::Create(m_pGraphicDev, this, m_pNPCDialogVec, 0.f, 0.f, 1.f, 1.f));
	UIManager::GetInstance()->Activate_UI(L"UI_DialogBG"); // NPC가 이 부분 호출해야함(수정)
}

NonPlayerCharacter* NonPlayerCharacter::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _float _fIdx_Frame)
{
	NonPlayerCharacter* pInstance = new NonPlayerCharacter(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NonPlayerCharacter Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("NonPlayerCharacter Create Failed");
		return nullptr;
	}

	pInstance->Set_FrameIndax(_fIdx_Frame);
	return pInstance;
}

void NonPlayerCharacter::OnCollided()
{
	if (m_pColliderCom->GetColSphere() & COL_PLAYER)
	{
		m_bIs_ColPlayer = true;
	}
	else
	{
		m_bIs_ColPlayer = false;
	}
}

HRESULT NonPlayerCharacter::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	// 상점 인벤
	pComponent = m_pTradeInv = dynamic_cast<TradeInv*>(Engine::Clone_Proto(L"Proto_TradeInv"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TradeInv", pComponent });

	return S_OK;
}

void NonPlayerCharacter::Free()
{
	Super::Free();
}
