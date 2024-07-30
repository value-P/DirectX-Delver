#pragma once

#include "Creature.h"

namespace Engine
{
	class Texture;
}

class Player;

class Monster : public Creature
{
	using Super = Creature;

public:
	explicit Monster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Monster(const Monster& rhs);
	virtual ~Monster();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component(wstring textureKey);

protected:
	virtual void OnCollided() override;
	virtual void OnDamaged() override;
	void CreateParticles(D3DCOLOR _dwColor);
	void CreateHitEffect(const D3DXVECTOR3& _vPos);

public:
	virtual void Set_BehaviorStat(_float _fSight_Range, _float _fAttackAble_Range, _float _fCoolTime_NormalAttack, _float _fSpeed);
	
protected:
	virtual void NormalAttack(_vec3 _vDir_Player);

public:
	static Monster* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey);

protected:
	virtual void	Free();

protected:
	MONSTER_ID m_eMonsterID = MI_END;

	Player* pPlayer = nullptr;
	Transform* m_pPlayerTransformCom = nullptr;

	Texture* m_pTextureCom = nullptr;
	Texture* m_pIdle_TextureCom = nullptr;
	Texture* m_pAttack_TextureCom = nullptr;
	Texture* m_pHit_TextureCom = nullptr;
	Texture* m_pDeath_TextureCom = nullptr;

	float m_fFrame = 0.f; // Creature�� �÷��� �ɵ�?
	
	_float m_fSight_Range = 0.f;
	_float m_fAttackAble_Range = 0.f;
	_float m_fDistance_Player = 0.f;
	
	bool m_bSelf_destruct = false;
	bool Lock_Tracking = true;
	
	bool m_bCollided_Player = false;

	
	// ���� �ǰ� ���� ��������
	bool m_bIs_HitStunState = false;
	_float	m_fDelayTime_HitStun = 0.f;
	_float m_fCoolTime_HitStun = 0.5f;

	// ���� �⺻ ���� ��Ÿ�� ����.
	bool m_bCoolDown_NormalAttack = false; // ���� �⺻���� ��ٿ� ����
	_float	m_fDelayTime_NormalAttack = 0.f; // �⺻������ ��ٿ� ���� ���� ��� �ð� ����.
	_float m_fCoolTime_NormalAttack = 0.f; // �ش� ������ �⺻���� ��Ÿ��.

	
	//UINT m_iCurRepeatCnt = 0;
	// �ð� ���� ����
	_float	m_fDelayTime_Death = 0.f;


	bool m_bIsDeathStart = true;

	bool m_bIsNormalAttackStart = true;
	bool m_bIsNormalAttackEnd = false;

	bool m_bIsStunStart = true;

	_float m_fSaveSpeed = 0.f;
};
