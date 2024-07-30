#include "stdafx.h"
#include "MoveWall.h"
#include "Export_System.h"
#include "Export_Utility.h"

MoveWall::MoveWall(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: GameObject(_pGraphicDevice)
{
}

MoveWall::~MoveWall()
{
}

MoveWall* MoveWall::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, bool _bAuto, bool _bRepeat, const wstring _wstrTag, const D3DXVECTOR3& _vPosBegin, const D3DXVECTOR3& _vPosEnd, const D3DXVECTOR3& _vSize, const float& _fSpeed, const float& _fStopTime,const D3DXVECTOR3& _vRot)
{
	MoveWall* pInstance = new MoveWall(_pGraphicDevice);

	if (FAILED(pInstance->Init_MoveWall(_bAuto, _bRepeat, _wstrTag, _vPosBegin, _vPosEnd, _vSize, _fSpeed, _fStopTime, _vRot)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_MoveWall Failed", L"MoveWall::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT MoveWall::Init_MoveWall(bool _bAuto, bool _bRepeat, const wstring _wstrTag, const D3DXVECTOR3& _vPosBegin, const D3DXVECTOR3& _vPosEnd, const D3DXVECTOR3& _vSize, const float& _fSpeed, const float& _fStopTime, const D3DXVECTOR3& _vRot)
{
	m_bAuto = _bAuto;
	if (!m_bAuto)
	{
		m_bMove = false;
		memcpy_s(&m_fDeltaTime, sizeof(float), &_fStopTime, sizeof(float));
		
	}
	m_bRepeat = _bRepeat;

	if (FAILED(Add_Component(_wstrTag)))
	{
		return E_FAIL;
	}

	memcpy_s(&m_vBegin, sizeof(D3DXVECTOR3), &_vPosBegin, sizeof(D3DXVECTOR3));
	memcpy_s(&m_vEnd, sizeof(D3DXVECTOR3), _vPosEnd, sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&m_vDir, &D3DXVECTOR3{ _vPosEnd - _vPosBegin });
	memcpy_s(&m_fSpeed, sizeof(float), &_fSpeed, sizeof(float));
	memcpy_s(&m_fStopTime, sizeof(float), &_fStopTime, sizeof(float));
	m_pTransformCom->Set_Scale(_vSize);
	m_pCollider->SetSize(_vSize);

	D3DXMATRIX matWorld, matScale, matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], _vRot.x);
	D3DXMatrixRotationX(&matRot[ROT_Y], _vRot.y);
	D3DXMatrixRotationX(&matRot[ROT_Z], _vRot.z);

	D3DXMATRIX matR{ matRot[ROT_X] * matRot[ROT_Y] * matRot[ROT_Z] };
	memcpy_s(&matR.m[3][0], 12, &m_vBegin, 12);

	m_pTransformCom->Set_Info(INFO_RIGHT, *(D3DXVECTOR3*)(&matR.m[0][0]));
	m_pTransformCom->Set_Info(INFO_UP, *(D3DXVECTOR3*)(&matR.m[1][0]));
	m_pTransformCom->Set_Info(INFO_LOOK, *(D3DXVECTOR3*)(&matR.m[2][0]));
	m_pTransformCom->Set_Info(INFO_POS, m_vBegin);
	
	D3DXMatrixScaling(&matScale, _vSize.x, _vSize.y, _vSize.z);
	D3DXMatrixMultiply(&matWorld, &matScale, &matR);

	m_pTransformCom->Set_WorldMatrix(&matWorld);
	m_pCollider->SetColLayer(COL_WALL);
	m_pCollider->SetThisPointer(this);
	m_pCollider->SelfBuilding();

	return S_OK;
}

int MoveWall::Update_GameObject(const float& _fTimeDelta)
{
	if (m_bMove)
	{
		Move(_fTimeDelta);
	}
	if (0.0f < m_fDeltaTime)
	{
		if (m_bAuto)
		{
			m_fDeltaTime -= _fTimeDelta;
		}
		else
		{
			if (m_pCollider->GetColOBB() & COL_PLAYER)
			{
				m_fDeltaTime -= _fTimeDelta;
			}
			else
			{
				memcpy_s(&m_fDeltaTime, sizeof(float), &m_fStopTime, sizeof(float));
			}
		}
	}
	if (0.0f >= m_fDeltaTime && m_bOnce)
	{
		if (m_bRepeat)
		{
			m_bMove = true;
		}
		else
		{
			if (m_bOnce)
			{
				m_bMove = true;
				m_bOnce = false;
			}
		}
	}


	m_pCollider->ClearInfo();
	Engine::AddCollider(m_pCollider);

	return 0;
}

void MoveWall::LateUpdate_GameObject()
{
	if (m_pLinkedWall)
	{
		if (m_pLinkedWall->m_bMove)
		{
			if(m_bGoEnd == m_pLinkedWall->m_bGoEnd)
			{
				m_bMove = true;
			}
			else
			{
				memcpy_s(&m_fDeltaTime, sizeof(float), &m_fStopTime, sizeof(float));
			}
		}
	}
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void MoveWall::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

void MoveWall::SetMove()
{
	m_fDeltaTime = 0.0f;
}

void MoveWall::SetLinkedWall(MoveWall* _pMoveWall)
{
	if (_pMoveWall)
	{
		m_pLinkedWall = _pMoveWall;
	}
}

HRESULT MoveWall::Add_Component(const wstring _wstrTag)
{
	m_pTransformCom = dynamic_cast<Transform*>(Engine::Clone_Proto(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Transform", m_pTransformCom });

	m_pBufferCom = dynamic_cast<CubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	if (nullptr == m_pBufferCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex",m_pBufferCom });

	m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(_wstrTag));
	if (nullptr == m_pTextureCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ _wstrTag,m_pTextureCom });

	m_pCollider = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	if (nullptr == m_pCollider) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pCollider });
	m_pCollider->SetTransform(m_pTransformCom);

	return S_OK;
}

void MoveWall::Move(const float& _fTimeDelta)
{
	D3DXVECTOR3 vPos{ m_pTransformCom->Get_Info(INFO_POS) };

	m_pTransformCom->Move_Pos(D3DXVECTOR3{ m_vDir * m_fSpeed * _fTimeDelta });
	if (m_pCollider->GetColOBB() & COL_PLAYER)
	{
		if (m_pCollider->GetColOBB_Info(COL_PLAYER).front().first.m_vInfo[INFO_POS].y > m_pTransformCom->Get_Info(INFO_POS).y + m_pTransformCom->Get_Scale().y)
		{
			static_cast<GameObject*>(m_pCollider->GetColOBB_Info(COL_PLAYER).front().first.m_pThis)->Get_Transform()->Move_Pos(m_vDir * m_fSpeed * _fTimeDelta);
		}
	}
	if (m_bGoEnd)
	{
		if (0.2f > D3DXVec3Length(&D3DXVECTOR3{ m_vEnd - vPos }))
		{
			m_pTransformCom->Set_Pos(m_vEnd);
			if (m_bRepeat)
			{
				memcpy_s(&m_fDeltaTime, sizeof(float), &m_fStopTime, sizeof(float));
				m_vDir *= -1.0f;
				m_bGoEnd = !m_bGoEnd;
			}
			else
			{
				m_bOnce = false;
			}
			m_bMove = false;
		}
	}
	else
	{
		if (0.2f > D3DXVec3Length(&D3DXVECTOR3{ m_vBegin - vPos }))
		{
			m_pTransformCom->Set_Pos(m_vBegin);
			memcpy_s(&m_fDeltaTime, sizeof(float), &m_fStopTime, sizeof(float));
			m_vDir *= -1.0f;
			m_bGoEnd = !m_bGoEnd;
			m_bMove = false;
		}
	}

	D3DXMATRIX matWorld;
	D3DXVECTOR3 vScale{ m_pTransformCom->Get_Scale() };
	D3DXMatrixIdentity(&matWorld);
	memcpy_s(&matWorld.m[0][0], 12, &D3DXVECTOR3{ m_pTransformCom->Get_Info(INFO_RIGHT) * vScale.x }, 12);
	memcpy_s(&matWorld.m[1][0], 12, &D3DXVECTOR3{ m_pTransformCom->Get_Info(INFO_UP) * vScale.y }, 12);
	memcpy_s(&matWorld.m[2][0], 12, &D3DXVECTOR3{ m_pTransformCom->Get_Info(INFO_LOOK) * vScale.z }, 12);
	memcpy_s(&matWorld.m[3][0], 12, &m_pTransformCom->Get_Info(INFO_POS), 12);
	m_pTransformCom->Set_WorldMatrix(&matWorld);
	m_pCollider->SelfBuilding();
}

void MoveWall::Free()
{
	GameObject::Free();
}
