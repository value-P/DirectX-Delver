#include "pch.h"
#include "BillBoardObj.h"
#include "Export_Utility.h"

BillBoardObj::BillBoardObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vInitPos, _vec2 vInitScale, const wstring& szTexturePath, const wstring& szOriginName)
    :GameObject(pGraphicDev), m_szTexturePath(szTexturePath), m_vInitPos(vInitPos), m_vInitScale(vInitScale)
{
    m_szOriginName = szOriginName;
}

BillBoardObj::BillBoardObj(const BillBoardObj& rhs)
    :GameObject(rhs), m_szTexturePath(rhs.m_szTexturePath), m_vInitPos(rhs.m_vInitPos), m_vInitScale(rhs.m_vInitScale)
{
    m_szOriginName = rhs.m_szOriginName;
}

BillBoardObj::~BillBoardObj()
{
}

HRESULT BillBoardObj::Init_GameObject()
{
    HRESULT result = GameObject::Init_GameObject();
    m_pTransformCom->Set_Pos(m_vInitPos.x, m_vInitPos.y, m_vInitPos.z);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return result;
}

_int BillBoardObj::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit =  GameObject::Update_GameObject(fTimeDelta);

    _matrix matWorld, matView, matBill;
    matWorld = *m_pTransformCom->Get_WorldMatrix();

    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;
    // 카메라의 y회전값
    D3DXMatrixInverse(&matBill, 0,&matBill);

    _matrix newMatWorld = matBill * matWorld;

    m_pTransformCom->Set_WorldMatrix(&newMatWorld);

    return iExit;
}

void BillBoardObj::LateUpdate_GameObject()
{
    GameObject::LateUpdate_GameObject();
}

void BillBoardObj::Render_GameObject()
{
   	//// 알파 테스트
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT BillBoardObj::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, m_szTexturePath);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    return S_OK;
}

BillBoardObj* BillBoardObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vInitPos, _vec2 vInitScale, const wstring& szTexturePath, const wstring& szOriginName)
{
    BillBoardObj* pInstance = new BillBoardObj(pGraphicDev, vInitPos, vInitScale, szTexturePath, szOriginName);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("CloneRect Create Failed");
        return nullptr;
    }

    return pInstance;
}

void BillBoardObj::Free()
{
    __super::Free();
}
