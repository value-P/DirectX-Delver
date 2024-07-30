#pragma once
#include "Creature.h"

namespace Engine
{
	class AnimationTexture;

}

class ShootingStar;
class BossBackLight;

class BossMonster1 : public Creature
{
public:
	explicit BossMonster1(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit BossMonster1(const BossMonster1& rhs);
	virtual ~BossMonster1();
	
public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Set_Target(GameObject* pTarget) { m_pTarget = pTarget; }

protected:
	HRESULT	Add_Component();

protected:
	virtual void OnCollided() override;
	virtual void OnDamaged() override;

private:
	// �ִϸ��̼� ���� �Լ�
	void	ChangeAnim(AnimationTexture* pAnim);

	// ���� ���̺� �Լ�
	int		GetPattern();

	// ������ �Լ�
	void	Phase1(const _float& fTimeDelta);
	void	Phase2(const _float& fTimeDelta);

	// Ÿ�ٰ��� �Ÿ��� ������Ʈ �ϴ� �Լ�
	void	TargetDistUpdate();

	// ���º� ��� �Լ�
	void	DetectTarget(const _float& fTimeDelta);
	void	OnIdle(const _float& fTimeDelta);
	void	OnAwake(const _float& fTimieDelta);
	void	PatternChange(const _float& fTimeDelta);
	void	OnDead(const _float& fTimeDelta);

	// ��ų �Լ�
	void	ThrowCube(const _float& fTimeDelta);		// �ĵ�ź
	void	Clow(const _float& fTimeDelta);		// ���� ����
	void	JumpAttack(const _float& fTimeDelta);	// �������
	void	ShootStar(const _float& fTimeDelta);	// ������
	void	DropMeteor(const _float& fTimeDelta);		// �������� �������� ������ �����ְ� ���׿� ����

	// 2�� ����
	void	TelePort(const _float& fTimeDelta);

private:
	BossPhase	m_ePhase		= PHASE1;
	BossState	m_eState		= BOSS_SLEEP;

	GameObject* m_pTarget		= nullptr;	
	_float		m_fTargetDist	= 100.f;
	
	// ���ǿ� ����
	_int		m_iShootCnt		= 0;
	_int		m_iRepeatCnt	= -1;
	_int		m_iMeteorRayCnt = 0;

	// �ð� üũ�� ����
	_float		m_fDeltaTime		= 0.f;
	_float		m_fPatternInterval	= 3.f;
	_float		m_fFlyTime			= 0.f;


	_bool		m_bOnAir		= false;
	_bool		m_bThrowed		= false;

	_bool		m_bOnJump		= false;
	_bool		m_bOnTerrain	= false;
	_bool		m_bPahseChanged = false;
	_bool		m_bPhaseChangeMotionPlayed = false;
	_bool		m_bRoared		= false;
	_bool		m_bCheck		= false;

	_bool		m_bAlive		= true;

private:
	AnimationTexture* m_pDynamicTex = nullptr;
	AnimationTexture* m_pSleep		= nullptr;
	AnimationTexture* m_pIdle		= nullptr;
	AnimationTexture* m_pWalk		= nullptr;
	AnimationTexture* m_pScratch	= nullptr;
	AnimationTexture* m_pRoar		= nullptr;
	AnimationTexture* m_pInjured	= nullptr;
	AnimationTexture* m_pCharge		= nullptr;
	AnimationTexture* m_pShoot		= nullptr;
	AnimationTexture* m_pJumpAttack = nullptr;

	list<ShootingStar*>	m_pShootingStarList;
	
	vector<int>			m_iTable1_Phase1;
	vector<int>			m_iTable2_Phase1;

	vector<int>			m_iTable1_Phase2;
	vector<int>			m_iTable2_Phase2;

	vector<int>*		m_pCurTable = nullptr;
	vector<int>::iterator	curPattern;
	
	BossBackLight*		m_pBackLight = nullptr;

	vector<_vec3>		m_vecPos;

public:
	static BossMonster1* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free();

};

