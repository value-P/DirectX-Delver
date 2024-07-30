#pragma once
#include "Component.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL Transform :	public Component
{
private:
	explicit Transform();
	explicit Transform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Transform(const Transform& rhs);
	virtual ~Transform();

public:		// Get Function
	const D3DXVECTOR3& Get_Info(INFO _eType) { return m_vInfo[_eType]; }
	const D3DXVECTOR3& Get_Scale() { return m_vScale; }
	void			Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}
	const _matrix* Get_WorldMatrix() { return &m_matWorld; }
	const D3DXVECTOR3& GetAngle() { return m_vAngle; }

public:		// Set Function
	void	Set_Info(INFO _eType, const D3DXVECTOR3& _vInfo) { m_vInfo[_eType] = _vInfo; }
	void			Set_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(&m_matWorld.m[eType][0], pInfo, sizeof(_vec3));
	}
	void			Set_Rotation(_vec3* pInfo)
	{
		memcpy(&m_vAngle, pInfo, sizeof(_vec3));
	}
	void			Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eType) += fAngle;
	}
	void			Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
	{
		m_vScale = { fX, fY, fZ };
	}
	void			Set_Scale(const D3DXVECTOR3& _vScale) { m_vScale = _vScale; }
	void			Set_Pos(const D3DXVECTOR3& _vPos) { m_vInfo[INFO_POS] = _vPos; }
	void			Set_Pos(_float fX, _float fY, _float fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y = fY;
		m_vInfo[INFO_POS].z = fZ;
	}
	void			Set_PosX(const float& _fX);
	void			Set_PosY(const float& _fY);
	void			Set_PosZ(const float& _fZ);
	void			Move_PosX(const float& _fX);
	void			Move_PosY(const float& _fY);
	void			Move_PosZ(const float& _fZ);
	void			Move_Pos(const D3DXVECTOR3& _vDir);
	void			Move_Pos(const float& _fX, const float& _fY, const float& _fZ);
	void			Set_WorldMatrix(const _matrix* pWorld) { m_matWorld = *pWorld; }
	
public:		// Class Function
	void			Move_Forward(const _vec3* pDir, const _float& fTimeDelta, const _float& fSpeed)
	{
		m_vInfo[INFO_POS] += *pDir  * fTimeDelta * fSpeed;
	}
	void			Chase_Target(const _vec3* pTargetPos, const _float& fTimeDelta, const _float& fSpeed);
	const _matrix*	Compute_Lookattarget(const _vec3* pTargetPos);
	void SettingMatrix();

public:
	HRESULT			Init_Transform();
	virtual			_int	Update_Component(const _float& fTimeDelta);
	virtual			void	LateUpdate_Component();

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

public:
	static	Transform*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	Component*	Clone();

private:
	virtual void		Free();
};

END