#include "pch.h"
#include "Cube.h"
#include "Export_Utility.h"

Cube::Cube(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos, const wstring& szTexPath, const wstring& szOriginName)
	: Engine::GameObject(pGraphicDev), m_wsTexPath(szTexPath), m_vInitPos(vInitPos)
{
	m_szOriginName = szOriginName;
}

Cube::Cube(const Cube& rhs)
	: Engine::GameObject(rhs), m_wsTexPath(rhs.m_wsTexPath), m_vInitPos(rhs.m_vInitPos)
{
	m_szOriginName = rhs.m_szOriginName;
}

Cube::~Cube()
{
}

HRESULT Cube::Init_GameObject()
{
	HRESULT result = GameObject::Init_GameObject();
	m_pTransformCom->Set_Pos(m_vInitPos.x, m_vInitPos.y, m_vInitPos.z);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return result;
}

_int Cube::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = GameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void Cube::LateUpdate_GameObject()
{
	GameObject::LateUpdate_GameObject();
}

void Cube::Render_GameObject()
{
	/*	wrap: 0~1을 넘는 경우 다시 0부터 출력
	clamp : 0~1을 넘는 경우 그냥 잘라냄
	mirror : 0~1을 넘는 경우 반전하여 출력 */

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	
}

HRESULT Cube::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = CubeTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_CUBE, m_wsTexPath);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

	return S_OK;
}

Cube* Cube::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos, const wstring& szTexPath, const wstring& szOriginName)
{
	Cube* pInstance = new Cube(pGraphicDev, vInitPos, szTexPath, szOriginName);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Cube Create Failed");
		return nullptr;
	}

	return pInstance;

}

void Cube::Free()
{
	__super::Free();
}
