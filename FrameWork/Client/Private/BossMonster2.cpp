#include "stdafx.h"
#include "BossMonster2.h"
#include "ChargeEffect.h"
#include "Breath.h"
#include "Totem.h"
#include "DragonClow.h"
#include "Storm.h"
#include "GuGuBomb.h"
#include "ToTemBomb.h"
#include "Orbit.h"
#include "BossBackLight.h"
#include "BossEndingPortal.h"

#include "Export_System.h"
#include "Export_Utility.h"

BossMonster2::BossMonster2(LPDIRECT3DDEVICE9 pGraphicDev)
    :Creature(pGraphicDev)
{
}

BossMonster2::BossMonster2(const BossMonster2& rhs)
    :Creature(rhs)
{
}

BossMonster2::~BossMonster2()
{
}

HRESULT BossMonster2::Init_GameObject()
{
    __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetSize(1.0f, 2.0f, 1.0f);
    m_pColliderCom->SetColLayer(COL_MONSTER);
    m_pColliderCom->SetThisPointer(this);

	// 발판 정보 입력
	m_vecFloatPos.resize(8);
	{
		m_vecFloatPos[0] = { -118.f, 25.f, 168.f };
		m_vecFloatPos[1] = { -110.f, 24.f, 203.f };
		m_vecFloatPos[2] = { -89.f,  24.f, 181.f };
		m_vecFloatPos[3] = { -88.f,  24.f, 135.f };
		m_vecFloatPos[4] = { -130.f, 24.f, 137.f };
		m_vecFloatPos[5] = { -151.f, 23.f, 152.f };
		m_vecFloatPos[6] = { -150.f, 24.f, 175.f };
		m_vecFloatPos[7] = { -145.f, 25.f, 197.f };
	}

	// 패턴 테이블 넣기
	m_iTable1_Phase1.resize(4);
	{
		m_iTable1_Phase1[0] = 1;
		m_iTable1_Phase1[1] = 1;
		m_iTable1_Phase1[2] = 0;
		m_iTable1_Phase1[3] = 2;
	}
	m_iTable2_Phase1.resize(3);
	{
		m_iTable2_Phase1[0] = 1;
		m_iTable2_Phase1[1] = 2;
		m_iTable2_Phase1[2] = 1;
	}

	m_iTable1_Phase2.resize(3);
	{
		m_iTable1_Phase2[0] = 1;
		m_iTable1_Phase2[1] = 2;
		m_iTable1_Phase2[2] = 1;
	}
	m_iTable2_Phase2.resize(3);
	{
		m_iTable2_Phase2[0] = 2;
		m_iTable2_Phase2[1] = 1;
		m_iTable2_Phase2[2] = 2;
	}

    return S_OK;
}

_int BossMonster2::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bAlive) return -1;

	if (m_creatureStat->iHp < 100 && !m_bPhaseChangeMotionPlayed)
	{
		m_bPhaseChangeMotionPlayed = true;
		m_eState = BOSS_GROGGY;
		m_bChargeEnd = true;
		m_bLaserEnd = true;
		ChangeAnim(m_pInjured);
		m_pDynamicTex->SetFrameSpeed(2.f);
	}
	
	if (m_creatureStat->iHp <= 0 && m_eState != BOSS_DEAD)
	{
		m_pBackLight->Destroy();

		ChangeAnim(m_pInjured);
		m_pDynamicTex->SetFrameSpeed(2.f);
		m_eState = BOSS_DEAD;
	}


    _int	iExit = __super::Update_GameObject(fTimeDelta);
	
	if (!m_bOnTotemPattern)
		m_pTransformCom->Move_Forward(&_vec3(0.f, -1.f, 0.f), fTimeDelta, 3.f);

	// 빌보드
	#pragma region 빌보드
	_matrix matWorld, matView, matBill;
	matWorld = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 빌 * (스 * 자 * 이)

	_matrix newMatWorld = matBill * matWorld;

	m_pTransformCom->Set_WorldMatrix(&newMatWorld);
#pragma endregion 

	TargetDistUpdate();

	switch (m_ePhase)
	{
	case PHASE1:
		Phase1(fTimeDelta);
		break;
	case PHASE2:
		Phase2(fTimeDelta);
		break;
	}

	if(!m_bOnTotemPattern)
		Engine::AddCollider(m_pColliderCom);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void BossMonster2::LateUpdate_GameObject()
{
	OnCollided();

	__super::LateUpdate_GameObject();
}

void BossMonster2::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 250);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 이미지 띄울려고
	D3DXMATRIX matWorld = *m_pTransformCom->Get_WorldMatrix();
	matWorld.m[3][1] += 0.8f;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDynamicTex->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT BossMonster2::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pIdle = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Idle/Idle_%d.png", 2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"IdleTex", pComponent });

	pComponent = m_pSleep = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Sleep/Sleep_%d.png", 2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"SleepTex", pComponent });

	pComponent = m_pWalk = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Walk/Walk_%d.png", 4);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"WalkTex", pComponent });
	
	pComponent = m_pScratch = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Scratch/Scratch_%d.png", 8);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"ScratchTex", pComponent });
	
	pComponent = m_pRoar = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Roar/Roar_%d.png", 11);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"RoarTex", pComponent });
	
	pComponent = m_pCharge = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Charge/Charge_%d.png", 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"ChargeTex", pComponent });
	
	pComponent = m_pShoot = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Shoot/Shoot_%d.png", 12);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"ShootTex", pComponent });
	
	pComponent = m_pLaser = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/SpecialAttack0/SpecialAttack0_%d.png", 19);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LaserTex", pComponent });
	
	pComponent = m_pInjured = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Hurt/Injured_%d.png", 2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"InjuredTex", pComponent });
	
	pComponent = m_pDie = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Dialga/Die/Die_%d.png", 27);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"DieTex", pComponent });

	m_pDynamicTex = m_pSleep;
	m_mapComponent[ID_DYNAMIC].insert({ L"DynamicTex",m_pDynamicTex });

	m_pDynamicTex->SetFrameSpeed(2.0f);

	return S_OK;
}

void BossMonster2::OnCollided()
{
	if (m_pColliderCom->GetColOBB() & COL_WALL)
	{
		PlaneCollision(&m_bOnTerrain);
	}

	// 피격
	if (m_pColliderCom->GetColOBB() & COL_PLAYERATTACK)
	{
		m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_PLAYERATTACK).front().first.m_iDamage;

		// 파티클 제조
		PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
		if (nullptr == pPSystem) { return; }
		PSMgr::GetInstance()->AddParticles(pPSystem);
		for (int i = 0; i < 16; i++)
		{
			pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 0, 20, 211));
		}

	}

	if (m_pColliderCom->GetColOBB() & COL_PLAYER_PROJ)
	{
		m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_PLAYER_PROJ).front().first.m_iDamage;
		
		// 파티클 제조
		PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
		if (nullptr == pPSystem) { return; }
		PSMgr::GetInstance()->AddParticles(pPSystem);
		for (int i = 0; i < 16; i++)
		{
			pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 0, 20, 211));
		}

	}

	if (m_pColliderCom->GetColOBB() & COL_EXPLOSION)
	{
		m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_EXPLOSION).front().first.m_iDamage;
	}
}

void BossMonster2::OnDamaged()
{
}

void BossMonster2::ChangeAnim(AnimationTexture* pAnim)
{
	m_pDynamicTex->SetFrame(0.f);
	m_mapComponent[ID_DYNAMIC][L"DynamicTex"] = pAnim;
	m_pDynamicTex = pAnim;
}

int BossMonster2::GetPattern()
{
	if (m_pCurTable == nullptr || m_bPahseChanged || curPattern == m_pCurTable->end())
	{
		if (m_iPatternStack >= 2)
		{
			m_iPatternStack = 0;
			return 0;
		}
		else if (m_ePhase == PHASE2 && curPattern == m_pCurTable->end())
			m_iPatternStack++;

		m_bPahseChanged = false;

		int rand = Engine::Rand_Num(0, 1);
		if (rand == 0)
		{
			if (m_ePhase == PHASE1)
			{
				m_pCurTable = &m_iTable1_Phase1;
				curPattern = m_pCurTable->begin();
			}
			else
			{
				m_pCurTable = &m_iTable1_Phase2;
				curPattern = m_pCurTable->begin();
			}
		}
		else
		{
			if (m_ePhase == PHASE1)
			{
				m_pCurTable = &m_iTable2_Phase1;
				curPattern = m_pCurTable->begin();
			}
			else
			{
				m_pCurTable = &m_iTable2_Phase2;
				curPattern = m_pCurTable->begin();
			}
		}

		int iResult = *curPattern;
		curPattern++;

		return iResult;
	}

	int iResult = *curPattern;
	curPattern++;

	return iResult;
}

void BossMonster2::Phase1(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case BOSS_SLEEP:
		DetectTarget(fTimeDelta);
		break;
	case BOSS_AWAKE:
		OnAwake(fTimeDelta);
		break;
	case BOSS_IDLE:
		OnIdle(fTimeDelta);
		break;
	case BOSS_PATTERN1:
		RoarOfTime(fTimeDelta);
		break;
	case BOSS_PATTERN2:
		ThrowDragonClow(fTimeDelta);
		break;
	case BOSS_PATTERN3:
		SpawnGuGu(fTimeDelta);
		break;
	case BOSS_GROGGY:
		PatternChange(fTimeDelta);
		break;
	case BOSS_HIT:
		break;
	default:
		break;
	}
}

void BossMonster2::Phase2(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case BOSS_IDLE:
		OnIdle(fTimeDelta);
		break;
	case BOSS_PATTERN1:
		SpawnTotem(fTimeDelta);
		break;
	case BOSS_PATTERN2:
		RangeStorm(fTimeDelta);
		break;
	case BOSS_PATTERN3:
		SpawnOrbit(fTimeDelta);
		break;
	case BOSS_GROGGY:
		OnGroggy(fTimeDelta);
		break;
	case BOSS_HIT:
		break;
	case BOSS_DEAD:
		OnDead(fTimeDelta);
		break;
	default:
		break;
	}
}

void BossMonster2::TargetDistUpdate()
{
	if (m_pTarget)
	{
		_vec3 targetPos, myPos;
		m_pTarget->Get_Transform()->Get_Info(INFO_POS, &targetPos);
		m_pTransformCom->Get_Info(INFO_POS, &myPos);
		_vec3 Dist = targetPos - myPos;
		m_fTargetDist = D3DXVec3Length(&Dist);
	}
}

void BossMonster2::DetectTarget(const _float& fTimeDelta)
{
	if (m_pTarget)
	{
		if (m_fTargetDist < 30.f)
		{
			Engine::Play_Sound(L"palkia", SOUND_EFFECT_1, 0.6f);
			ChangeAnim(m_pRoar);
			m_pDynamicTex->SetFrameSpeed(7.f);
			m_eState = BOSS_AWAKE;
		}
	}
}

void BossMonster2::OnIdle(const _float& fTimeDelta)
{
	m_fPatternInterval += fTimeDelta;

	//일정 시간 이후마다 패턴 실행
	if (m_fPatternInterval >= 2.5f)
	{
		m_fPatternInterval = 0.f;

		int iRandNum = GetPattern();
		//UINT iRandNum = 0;

		switch (iRandNum)
		{
		case 0:
			m_eState = BOSS_PATTERN1;
			ChangeAnim(m_pCharge);
			m_pDynamicTex->SetFrameSpeed(8.f);
			break;
		case 1:
			m_eState = BOSS_PATTERN2;
			break;
		case 2:
			if (m_ePhase == PHASE1)
			{
				ChangeAnim(m_pRoar);
				m_pDynamicTex->SetFrameSpeed(8.f);
			}
			else
			{
				ChangeAnim(m_pShoot);
				m_pDynamicTex->SetFrameSpeed(8.f);
			}
			m_eState = BOSS_PATTERN3;
			break;
		}
	}
}

void BossMonster2::OnAwake(const _float& fTimieDelta)
{
	UINT lastIdx = m_pDynamicTex->GetLastFrameIndex();
	_float curIdx = m_pDynamicTex->GetFrame();
	if ((UINT)curIdx >= lastIdx)
	{
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
		m_eState = BOSS_IDLE;
	}
}

void BossMonster2::PatternChange(const _float& fTimeDelta)
{

	m_fPatternInterval += fTimeDelta;
	// 3초간 그로기 되고 2페이즈로 전환
	if (m_fPatternInterval > 5.f)
	{
		if (!m_bRoared)
		{
			Engine::Play_Sound_NoInterval(L"dialga", SOUND_EFFECT_2, 0.8f);
			m_bRoared = true;
			ChangeAnim(m_pRoar);
			m_pDynamicTex->SetFrameSpeed(7.f);
		}
		else
		{
			UINT lastIdx = m_pDynamicTex->GetLastFrameIndex();
			_float curIdx = m_pDynamicTex->GetFrame();
			if ((UINT)curIdx >= lastIdx)
			{
				m_fPatternInterval = 0.f;
				m_bRoared = false;
				m_ePhase = PHASE2;
				m_bPahseChanged = true;

				ChangeAnim(m_pIdle);
				m_pDynamicTex->SetFrameSpeed(2.f);
				m_eState = BOSS_IDLE;

				m_pBackLight = BossBackLight::Create(m_pGraphicDev, m_pTarget->Get_Transform(), m_pTransformCom, BOSS2);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
					Add_GameObject(L"BossBackLight", m_pBackLight);
			}
		}
	}
}

void BossMonster2::OnGroggy(const _float& fTimeDelta)
{
	m_fPatternInterval += fTimeDelta;

	if (m_fPatternInterval > 6.f)
	{
		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
		m_fPatternInterval = 0.f;
	}
}

void BossMonster2::OnDead(const _float& fTimeDelta)
{
	m_fPatternInterval += fTimeDelta;

	if (m_fPatternInterval > 3.f && !m_bCheck)
	{
		m_bCheck = true;
		ChangeAnim(m_pDie);
		m_pDynamicTex->SetFrameSpeed(5.f);
	}
	
	if(m_bCheck)
	{
		UINT lastIdx = m_pDynamicTex->GetLastFrameIndex();
		_float curIdx = m_pDynamicTex->GetFrame();
		if ((UINT)curIdx >= lastIdx)
		{
			BossEndingPortal* pPortal = BossEndingPortal::Create(m_pGraphicDev, &_vec3(-118.f, 27.f, 168.f));
			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"EndingPortal", pPortal);

			m_bAlive = false;
		}
	}
}

void BossMonster2::RoarOfTime(const _float& fTimeDelta)
{
	_float curFrame = m_pDynamicTex->GetFrame();

	// 차지
	if (!m_bChargeEnd)
	{
		m_fPatternInterval += fTimeDelta;

		if (!m_bChargeStarted)
		{
			Engine::Play_Sound_NoInterval(L"ChargeEnergy", SOUND_EFFECT_1, 0.5f);
			ChargeEffect* m_pEffect = ChargeEffect::Create(m_pGraphicDev, this, m_pTarget->Get_Transform());
			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"ChargeEffect", m_pEffect);

			m_bChargeStarted = true;
		}

		// 1.5초 차지 후 발사
		if (m_fPatternInterval > 3.f)
		{
			Engine::Play_Sound_NoInterval(L"Breath", SOUND_EFFECT_2, 0.5f);
			m_bChargeStarted = false;
			m_bChargeEnd = true;
			m_fPatternInterval = 0.f;
			ChangeAnim(m_pLaser);
			m_pDynamicTex->SetFrameSpeed(8.f);
		}

	}
	// 차지 가 끝났다면 레이저 발사
	else
	{
		m_fPatternInterval += fTimeDelta;

		if (!m_bLaserStarted)
		{
			m_bLaserStarted = true;
			m_bLaserEnd = false;

			Breath* m_pEffect = Breath::Create(m_pGraphicDev, this, m_pTarget->Get_Transform(), 40.f, 1);
			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"Breath", m_pEffect);
		}

		if (m_fPatternInterval > 6.f)
		{
			m_bLaserEnd = true;
			m_bLaserStarted = false;
			m_bChargeEnd = false;
			m_fPatternInterval = 0.f;
			m_eState = BOSS_IDLE;
			ChangeAnim(m_pIdle);
			m_pDynamicTex->SetFrameSpeed(2.f);
		}
	}
}

void BossMonster2::Teleport(const _float& fTimeDelta)
{
	// 플레이어와 가장 가까운 발판으로 이동
	_float nearestLength = 1000.f;
	_vec3 nearestPos;

	for (auto& iter : m_vecFloatPos)
	{
		_vec3 TargetPos = m_pTarget->Get_Transform()->Get_Info(INFO_POS);
		_vec3 temp = iter - TargetPos;
		_float length = D3DXVec3Length(&temp);

		if (length < nearestLength)
		{
			nearestLength = length;
			nearestPos = iter;
		}
	}

	_vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
	_vec3 dist = myPos - nearestPos;
	_float tempLeng = D3DXVec3Length(&dist);

	if (tempLeng > 4.f)
	{
		Engine::Play_Sound_NoInterval(L"Teleport", SOUND_EFFECT_3, 0.5f);
	}

	nearestPos.y += 1.2f;
	m_pTransformCom->Set_Pos(nearestPos);
}

void BossMonster2::RangeStorm(const _float& fTimeDelta)
{
	if (!m_bTelePorted)
	{
		m_bTelePorted = true;
		Teleport(fTimeDelta);
	}
	else
	{
		m_fPatternInterval += fTimeDelta;
		// 텔포 2초 후 스킬 시전
		if (m_fPatternInterval > 2.f && !m_bRoared)
		{
			m_bRoared = true;
			Engine::Play_Sound(L"dialga", SOUND_EFFECT_3, 0.5f);
			ChangeAnim(m_pRoar);
			m_pDynamicTex->SetFrameSpeed(8.f);
		}
	}

	_float curFrame = m_pDynamicTex->GetFrame();
	if (m_bRoared && curFrame > 9.f)
	{
		Storm* pStorm = Storm::Create(m_pGraphicDev, 8.f, 1);
		_vec3 pos = m_pTransformCom->Get_Info(INFO_POS);
		pos.y -= 1.8f;
		pStorm->Get_Transform()->Set_Pos(pos);
		Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
			Add_GameObject(L"Storm", pStorm);

		m_fPatternInterval = 0.f;
		m_bTelePorted = false;
		m_bRoared = false;
		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
	}

}

void BossMonster2::SpawnTotem(const _float& fTimeDelta)
{
	_float curFrame = m_pDynamicTex->GetFrame();
	if (curFrame > 9.f)
	{
		if (!m_bOnTotemPattern)
		{
			Engine::Play_Sound_NoInterval(L"SpawnTotem", SOUND_EFFECT_2, 1.f);

			_vec3 Pos = m_vecFloatPos[0];
			Pos.y += 1.2f;

			m_pTransformCom->Set_Pos(Pos);

			m_bOnTotemPattern = true;

			m_pDynamicTex->SetFrameSpeed(0.f);

			// totembomb 소환
			m_pTotemBomb = ToTemBomb::Create(m_pGraphicDev, &_vec3(-118.f, 60.f, 168.f));
			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"TotemBomb", m_pTotemBomb);

			// 토템 개수
			int iCnt = Engine::Rand_Num(3, 6);

			// 발판 위치 섞기
			std::shuffle(m_vecFloatPos.begin() + 1, m_vecFloatPos.end(), std::default_random_engine());

			Totem* pTotem = nullptr;
			for (int i = 1; i <= iCnt; ++i)
			{
				_vec3 Pos = m_vecFloatPos[i];
				Pos.y += 1.f;
				pTotem = Totem::Create(m_pGraphicDev, &Pos, m_pTotemBomb->Get_Transform());
				m_pTotemList.push_back(pTotem);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
					Add_GameObject(L"Totem", pTotem);
			}

		}
		// 토템이 다 소환된 상태, 시간초 재기
		else
		{
			m_fPatternInterval += fTimeDelta;

			// 20초 후 토템을 다 비활성화 시켰는지에 따라 성공 실패 갈림
			if (m_fPatternInterval > 40.f)
			{
				int iActiveCnt = 0;
				for (auto& iter : m_pTotemList)
				{
					// 하나라도 활성화되어있다면 실패
					if (iter->IsActivated())
					{
						iActiveCnt++;
					}
				}

				// 실패
				if (iActiveCnt > 0)
				{
					Engine::Play_Sound_NoInterval(L"TotemFail", SOUND_EFFECT_3, 1.f);

					m_pTotemBomb->Bomb();
					Creature* pPlayer = dynamic_cast<Creature*>(m_pTarget);
					pPlayer->Set_CreatureHp(pPlayer->Get_CreatureHp() - 10.f);
					
					m_fPatternInterval = 0.f;
					m_bOnTotemPattern = false;
					m_eState = BOSS_IDLE;
					ChangeAnim(m_pIdle);
					m_pDynamicTex->SetFrameSpeed(2.f);
				}
				// 성공
				else
				{
					Engine::Play_Sound_NoInterval(L"TotemSuccess", SOUND_EFFECT_3, 1.f);
					
					m_pTotemBomb->Destroy_Bomb();

					m_fPatternInterval = 0.f;
					m_bOnTotemPattern = false;
					m_eState = BOSS_GROGGY;
					ChangeAnim(m_pInjured);
					m_pDynamicTex->SetFrameSpeed(2.f);
				}

				// 토템 파괴
				for (auto& iter : m_pTotemList)
					iter->Destroy();

				m_pTotemList.clear();
			}
		}
	}
}

void BossMonster2::SpawnGuGu(const _float& fTimeDelta)
{
	_float curFrame = m_pDynamicTex->GetFrame();

	if (curFrame > 6.f)
	{
		m_pDynamicTex->SetFrameSpeed(0.f);

		m_fPatternInterval += fTimeDelta;
		if (m_fPatternInterval > 1.5f)
		{
			m_fPatternInterval = 0.f;

			_vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
			_vec3 pos = myPos + _vec3(0.f, 2.f, 0.f);

			GuGuBomb* pGuGu = GuGuBomb::Create(m_pGraphicDev, m_pTarget->Get_Transform(), 5.f, 1);

			pGuGu->Get_Transform()->Set_Pos(pos);

			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"GuGuBomb", pGuGu);

			++m_iGuGuCnt;
		}
	}

	if (m_iGuGuCnt > 4)
	{
		m_fPatternInterval = 0.f;
		m_iGuGuCnt = 0;
		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
	}
}

void BossMonster2::SpawnOrbit(const _float& fTimeDelta)
{
	_float curFrame = m_pDynamicTex->GetFrame();
	if (curFrame > 10.f && !m_bRoared)
	{
		m_bRoared = true;
		
		_vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
		_vec3 vLook = m_pTransformCom->Get_Info(INFO_LOOK);
		_vec3 vRight;
		D3DXVec3Cross(&vRight, &vLook, &_vec3(0.f, 1.f, 0.f));
		_matrix matAxis;

		Engine::Play_Sound_NoInterval(L"OpenOrbit", SOUND_EFFECT_1, 1.f);
		for (int i = 0; i < 3; ++i)
		{
			_vec3 Dir;
			D3DXMatrixRotationAxis(&matAxis, &vLook, D3DXToRadian(-90.f * i));
			D3DXVec3TransformNormal(&Dir, &vRight, &matAxis);
			_vec3 pos = myPos;
			pos += Dir;

			Orbit* pOrbit = Orbit::Create(m_pGraphicDev, m_pTarget->Get_Transform(),&pos,8.f,1);
			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"Orbit", pOrbit);
		}

		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
	}

	m_fPatternInterval += fTimeDelta;
	if (m_fPatternInterval > 4.f)
	{
		m_fPatternInterval = 0.f;
		m_bTelePorted = false;
		m_bRoared = false;
		m_eState = BOSS_IDLE;
	}
}

void BossMonster2::ThrowDragonClow(const _float& fTimeDelta)
{
	if (!m_bTelePorted)
	{
		m_bTelePorted = true;
		Teleport(fTimeDelta);
	}
	else
	{
		m_fPatternInterval += fTimeDelta;
		// 텔포 2초 후 스킬 시전
		if (m_fPatternInterval > 2.f && !m_bRoared)
		{
			m_bRoared = true;
			Engine::Play_Sound(L"dialga", SOUND_EFFECT_2, 0.5f);
			ChangeAnim(m_pScratch);
			m_pDynamicTex->SetFrameSpeed(8.f);
		}
	}

	_float curFrame = m_pDynamicTex->GetFrame();

	if (curFrame > 6.f)
	{
		_vec3 targetPos = m_pTarget->Get_Transform()->Get_Info(INFO_POS);
		_vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
		_vec3 Dir = targetPos - myPos;
		D3DXVec3Normalize(&Dir, &Dir);
		_vec3 pos = myPos + Dir;

		DragonClow* pClow = DragonClow::Create(m_pGraphicDev, &Dir, 14.f, 1);

		pClow->Get_Transform()->Set_Pos(pos);

		Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
			Add_GameObject(L"DragonClow", pClow);

		Engine::Play_Sound_NoInterval(L"mg_ice_impact_04", SOUND_EFFECT_1, 0.5f);
		m_fPatternInterval = 0.f;
		m_bTelePorted = false;
		m_bRoared = false;
		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
	}
}

BossMonster2* BossMonster2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	BossMonster2* pInstance = new BossMonster2(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossMonster2 Create Failed");
		return nullptr;
	}

	return pInstance;
}

void BossMonster2::Free()
{
	m_mapComponent[ID_DYNAMIC].erase(L"DynamicTex");
	__super::Free();
}
