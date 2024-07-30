#include "stdafx.h"
#include "VillageSkyBox.h"

#include "Export_Utility.h"
#include "Export_System.h"

VillageSkyBox::VillageSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

VillageSkyBox::VillageSkyBox(const VillageSkyBox& rhs)
	: Super(rhs)
{
}

VillageSkyBox::~VillageSkyBox()
{
}

HRESULT VillageSkyBox::Init_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(40.f, 40.f, 40.f);

	return S_OK;
}

_int VillageSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	_int	iExit = GameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void VillageSkyBox::LateUpdate_GameObject()
{
	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 + 5.f, matCamWorld._43);
}

void VillageSkyBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT VillageSkyBox::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Transform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_SkyBox_Village_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_SkyBox_Village_Texture", pComponent });

	return S_OK;
}

VillageSkyBox* VillageSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	VillageSkyBox* pInstance = new VillageSkyBox(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkyBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void VillageSkyBox::Free()
{
	Super::Free();
}
