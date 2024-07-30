#pragma once
#include "BossSkillObject.h"

enum BOSSTYPE
{
	BOSS1,
	BOSS2
};

class BossBackLight :
    public BossSkillObject
{
private:
	explicit BossBackLight(LPDIRECT3DDEVICE9 pGraphicDev,Transform* pTargetTransform, Transform* pOwnerTransform, BOSSTYPE eBossType);
	virtual ~BossBackLight();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Destroy() { m_bAlive = false; }

protected:
	HRESULT	Add_Component();

private:
	Transform* m_pOwnerTransform;
	Transform* m_pTargetTransform;
	BOSSTYPE   m_eBossType;

public:
	static BossBackLight* Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTargetTransform, Transform* pOwnerTransform, BOSSTYPE eBossType);

private:
	virtual void	Free();
};

