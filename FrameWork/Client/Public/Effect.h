#pragma once

#include "GameObject.h" 

// 정말 정말 임시 By Gwan
enum EffectType
{
	EFFECT_ARROW,
	EFFECT_MAGIC,

	EFFECTTYPE_END,
};

class Effect : public GameObject
{
protected:
	explicit Effect(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Effect();

public:
	HRESULT Init_Effect();

public:
	virtual int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() PURE;

public:		// Get Function
	float GetAliveTime() { return m_fAliveTime; }
	virtual D3DCOLOR GetColor() { return 0; }

	// Temp
	EffectType GetEffectType() { return m_eEffectType; }
	void SetEffectType(EffectType effectType) { m_eEffectType = effectType; }

public:		// Set Function
	void SetPos(const D3DXVECTOR3& _vPos);
	virtual void SetColType(COLLAYER _eType) {};	// 생성하고 타입 넣어줘야 함

protected:
	void ComputeBillBoard();	// SetTransform 들어있어서 렌더 해주기 전에 해주면됨

private:
	HRESULT	Add_Component();

protected:
	float m_fAliveTime{ 6.0f };
	bool m_bAlive{ true };
	EffectType m_eEffectType { EFFECTTYPE_END };

protected:
	virtual void	Free();
};


