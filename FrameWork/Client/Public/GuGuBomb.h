#pragma once
#include "BossSkillObject.h"
class GuGuBomb :
    public BossSkillObject
{
private:
	explicit GuGuBomb(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage);
	virtual ~GuGuBomb();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	Transform*		m_pTarget;
	
	_float			m_fModifyTime = 0.f; // Ç×·Î ¼öÁ¤ ÅÒ

public:
	static GuGuBomb* Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage);

private:
	virtual void	Free();
};

