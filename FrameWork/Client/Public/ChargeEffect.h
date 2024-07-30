#pragma once
#include "BossSkillObject.h"
class BossMonster2;

class ChargeEffect :
    public BossSkillObject
{
private:
	explicit ChargeEffect(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform);
	explicit ChargeEffect(const ChargeEffect& rhs);
	virtual ~ChargeEffect();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	Transform*			m_pTargetTransform;
	BossMonster2*		m_pOwner;;
public:
	static ChargeEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform);

private:
	virtual void	Free();
};

