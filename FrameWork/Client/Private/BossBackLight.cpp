#include "stdafx.h"
#include "BossBackLight.h"

#include "Export_Utility.h"

BossBackLight::BossBackLight(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTargetTransform, Transform* pOwnerTransform, BOSSTYPE eBossType)
    :BossSkillObject(pGraphicDev),m_pTargetTransform(pTargetTransform), m_pOwnerTransform(pOwnerTransform), m_eBossType(eBossType)
{
}

BossBackLight::~BossBackLight()
{
}

HRESULT BossBackLight::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(3.f, 3.f, 3.f);

    return iResult;
}

_int BossBackLight::Update_GameObject(const _float& fTimeDelta)
{
	// 몬스터 뒤통수 따라다니기
	_vec3 vPos = m_pOwnerTransform->Get_Info(INFO_POS);
	_vec3 vTargetPos = m_pTargetTransform->Get_Info(INFO_POS);
	_vec3 vLook = vTargetPos - vPos;
	D3DXVec3Normalize(&vLook, &vLook);

	vPos.y += 1.f;
	vPos -= vLook * 0.4f;

	m_pTransformCom->Set_Pos(vPos);

	_int iResult = __super::Update_GameObject(fTimeDelta);

#pragma region 빌보드
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
#pragma endregion 

	Compute_ViewZ(&m_pTransformCom->Get_Info(INFO_POS));
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void BossBackLight::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void BossBackLight::Render_GameObject()
{
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 205);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT BossBackLight::Add_Component()
{
	Component* pComponent = nullptr;

	if (m_eBossType == BOSS1)
	{
		pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev,TEX_NORMAL,
			L"../../Resource/Texture/Effect/Boss/BackLight1.png");
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE,pComponent });
	}
	else
	{
		pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL,
			L"../../Resource/Texture/Effect/Boss/BackLight2.png");
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE,pComponent });
	}

	pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	return S_OK;
}

BossBackLight* BossBackLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTargetTransform, Transform* pOwnerTransform, BOSSTYPE eBossType)
{
	BossBackLight* pInstance = new BossBackLight(pGraphicDev, pTargetTransform, pOwnerTransform, eBossType);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossBackLight Create Failed");
		return nullptr;
	}

	return pInstance;
}

void BossBackLight::Free()
{
	__super::Free();
}
