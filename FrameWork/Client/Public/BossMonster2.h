#pragma once
#include "Creature.h"

namespace Engine
{
	class AnimationTexture;
}

class Totem;
class ToTemBomb;
class BossBackLight;

class BossMonster2 :
    public Creature
{
public:
	explicit BossMonster2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit BossMonster2(const BossMonster2& rhs);
	virtual ~BossMonster2();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Set_Target(GameObject* pTarget) { m_pTarget = pTarget; }

	_bool	IsChargeEnd()	{ return m_bChargeEnd; }
	_bool	IsLaserEnd()	{ return m_bLaserEnd; }

protected:
	HRESULT	Add_Component();

protected:
	virtual void OnCollided() override;
	virtual void OnDamaged() override;

private:
	// 애니메이션 변경 함수
	void	ChangeAnim(AnimationTexture* pAnim);

	// 패턴 테이블 함수
	int		GetPattern();

	// 페이즈 함수
	void	Phase1(const _float& fTimeDelta);
	void	Phase2(const _float& fTimeDelta);

	// 타겟과의 거리를 업데이트 하는 함수
	void	TargetDistUpdate();

	// 상태별 사용 함수
	void	DetectTarget(const _float& fTimeDelta);
	void	OnIdle(const _float& fTimeDelta);
	void	OnAwake(const _float& fTimieDelta);
	void	PatternChange(const _float& fTimeDelta);
	void	OnGroggy(const _float& fTimeDelta);
	void	OnDead(const _float& fTimeDelta);

private:
	void	RoarOfTime(const _float& fTimeDelta);
	void	Teleport(const _float& fTimeDelta);		// 플레이어에게 가장 가까운 발판으로 이동하는 기술
	void	RangeStorm(const _float& fTimeDelta);
	void	SpawnTotem(const _float& fTimeDelta);
	void	SpawnGuGu(const _float& fTimeDelta);
	void	SpawnOrbit(const _float& fTimeDelta);
	void	ThrowDragonClow(const _float& fTimeDelta);

private:
	BossPhase	m_ePhase			= PHASE1;
	BossState	m_eState			= BOSS_SLEEP;

	GameObject* m_pTarget			= nullptr;
	_float		m_fTargetDist		= 100.f;

	_float		m_fPatternInterval	= 3.f;

	_bool		m_bOnTerrain		= false;
	_bool		m_bChargeEnd		= false;
	_bool		m_bChargeStarted	= false;
	_bool		m_bLaserStarted		= false;
	_bool		m_bLaserEnd			= false;
	_bool		m_bTelePorted		= false;
	_bool		m_bRoared			= false;
	_bool		m_bOnTotemPattern	= false;
	_bool		m_bPahseChanged		= false;
	_bool		m_bPhaseChangeMotionPlayed = false;
	_bool		m_bCheck = false;

	_bool		m_bAlive = true;

	int			m_iPatternStack = 0;
	int			m_iGuGuCnt = 0;

private:
	vector<_vec3>	m_vecFloatPos; // 장판 위치 배열
	ToTemBomb*		m_pTotemBomb = nullptr;
	list<Totem*>	m_pTotemList;

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
	AnimationTexture* m_pLaser		= nullptr;
	AnimationTexture* m_pDie		= nullptr;

	vector<int>			m_iTable1_Phase1;
	vector<int>			m_iTable2_Phase1;

	vector<int>			m_iTable1_Phase2;
	vector<int>			m_iTable2_Phase2;

	vector<int>* m_pCurTable = nullptr;
	vector<int>::iterator	curPattern;

	BossBackLight* m_pBackLight = nullptr;

public:
	static BossMonster2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free();

};

