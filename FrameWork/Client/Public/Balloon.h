#pragma once

#include "EnvironmentBase.h"

class Balloon : public EnvironmentBase
{
private:
	explicit Balloon(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Balloon();

public:
	static Balloon* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const D3DCOLOR& _dwColor, const D3DXVECTOR3& _vMinDot, const D3DXVECTOR3& _vMaxDot, GameObject* _pObject = nullptr);
	
public:
	HRESULT Init_Balloon(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const D3DCOLOR& _dwColor, const D3DXVECTOR3& _vMinDot, const D3DXVECTOR3& _vMaxDot, GameObject* _pObject);

public:
	virtual _int Update_GameObject(const float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT	Add_Component();

private:
	D3DXVECTOR3 m_vDir{ 0.0f,0.0f,0.0f };
	float m_fSpeed{ 0.0f };
	D3DCOLOR m_dwColor{ 0xFFFFFFFF };
	bool m_bAlive{ true };
	D3DXVECTOR3 m_vMinDot{ 0.0f,0.0f,0.0f };
	D3DXVECTOR3 m_vMaxDot{ 0.0f,0.0f,0.0f };

	GameObject* m_pGenerateObj{ nullptr };

private:
	virtual void Free();
};

