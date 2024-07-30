#include "stdafx.h"
#include "Rope.h"
#include "Export_Utility.h"

Rope::Rope(LPDIRECT3DDEVICE9 pGraphicDev)
    :GameObject(pGraphicDev)
{
}

Rope::Rope(const Rope& rhs)
    :GameObject(rhs)
{
}

Rope::~Rope()
{
}

HRESULT Rope::Init_GameObject()
{
	HRESULT iResult = __super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(),E_FAIL);

	m_tVertices[0].vTexUV = { 0.f,0.f };
	m_tVertices[1].vTexUV = { 1.f,0.f };
	m_tVertices[2].vTexUV = { 1.f,1.f };

	m_tVertices[3].vTexUV = { 0.f,0.f };
	m_tVertices[4].vTexUV = { 1.f,1.f };
	m_tVertices[5].vTexUV = { 0.f,1.f };

    return iResult;
}

_int Rope::Update_GameObject(const _float& fTimeDelta)
{
	return __super::Update_GameObject(fTimeDelta);
}

void Rope::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void Rope::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF,250);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pGraphicDev->SetFVF(FVF_TEX);
	m_pTextureCom->Set_Texture(0);
	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, m_tVertices, sizeof(VTXTEX));
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void Rope::Rope_Update(_vec3* pStart, _vec3* pEnd, _float fThickness)
{
	m_vStartPos = *pStart;
	m_vEndPos = *pEnd;
	m_fThickness = fThickness;

	_vec3 Dir = m_vEndPos - m_vStartPos;
	_vec3 Normal = { Dir.z, 0.f, -Dir.x }; // y축과 수직으로 평평하게
	D3DXVec3Normalize(&Normal, &Normal);

	m_vPoints[0] = m_vStartPos + m_fThickness * 0.5f * Normal;
	m_vPoints[1] = m_vEndPos   + m_fThickness * 0.5f * Normal;
	m_vPoints[2] = m_vStartPos - m_fThickness * 0.5f * Normal;
	m_vPoints[3] = m_vEndPos   - m_fThickness * 0.5f * Normal;

	m_tVertices[0].vPosition = m_vPoints[3];
	m_tVertices[1].vPosition = m_vPoints[1];
	m_tVertices[2].vPosition = m_vPoints[0];

	m_tVertices[3].vPosition = m_vPoints[3];
	m_tVertices[4].vPosition = m_vPoints[0];
	m_tVertices[5].vPosition = m_vPoints[2];

	_float length = D3DXVec3Length(&Dir); 

	m_tVertices[0].vTexUV = { 0.f,0.f };
	m_tVertices[1].vTexUV = { 1.f,0.f };
	m_tVertices[2].vTexUV = { 1.f,length };

	m_tVertices[3].vTexUV = { 0.f,0.f };
	m_tVertices[4].vTexUV = { 1.f,length };
	m_tVertices[5].vTexUV = { 0.f,length };

}

HRESULT Rope::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev,TEX_NORMAL,L"../../Resource/Texture/Effect/Rope.png");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });
	
	pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	return S_OK;
}

Rope* Rope::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Rope* pInstance = new Rope(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Rope Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Rope::Free()
{
	__super::Free();
}
