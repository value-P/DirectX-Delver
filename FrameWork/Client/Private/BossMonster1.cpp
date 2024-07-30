#include "stdafx.h"
#include "BossMonster1.h"
#include "EnergyBall.h"
#include "SpacialRend.h"
#include "JumpCrack.h"
#include "ShootingStar.h"
#include "MeteorLine.h"
#include "BossBackLight.h"
#include "BossPortal.h"

#include "Export_Utility.h"
#include "Export_System.h"

BossMonster1::BossMonster1(LPDIRECT3DDEVICE9 pGraphicDev)
	:Creature(pGraphicDev)
{
}

BossMonster1::BossMonster1(const BossMonster1& rhs)
	:Creature(rhs)
{
}

BossMonster1::~BossMonster1()
{
}

HRESULT BossMonster1::Init_GameObject()
{
	__super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetSize(1.0f, 2.0f, 1.0f);
	m_pColliderCom->SetColLayer(COL_MONSTER);
	m_pColliderCom->SetThisPointer(this);

	// 랜덤 텔포 위치 (y는 노상관)
	m_vecPos.resize(6);
	{
		m_vecPos[0] = { 118.f,27.f,168.f };
		m_vecPos[1] = { 100.f,27.f,145.f };
		m_vecPos[2] = { 98.f,27.f,170.f };
		m_vecPos[3] = { 120.f,27.f,186.f };
		m_vecPos[4] = { 136.f,27.f,171.f };
		m_vecPos[5] = { 127.f,27.f,145.f };
	}

	// 패턴 테이블 넣기
	m_iTable1_Phase1.resize(5);
	{
		m_iTable1_Phase1[0] = 2;
		m_iTable1_Phase1[1] = 2;
		m_iTable1_Phase1[2] = 0;
		m_iTable1_Phase1[3] = 2;
		m_iTable1_Phase1[4] = 1;
	}
	m_iTable2_Phase1.resize(3);
	{
		m_iTable2_Phase1[0] = 0;
		m_iTable2_Phase1[1] = 2;
		m_iTable2_Phase1[2] = 1;
	}

	m_iTable1_Phase2.resize(3);
	{
		m_iTable1_Phase2[0] = 0;
		m_iTable1_Phase2[1] = 1;
		m_iTable1_Phase2[2] = 2;
	}
	m_iTable2_Phase2.resize(4);
	{
		m_iTable2_Phase2[0] = 2;
		m_iTable2_Phase2[1] = 0;
		m_iTable2_Phase2[2] = 2;
		m_iTable2_Phase2[3] = 1;
	}

	return S_OK;
}

_int BossMonster1::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bAlive) return -1;

	// 체력 400 미만이면 2페이즈
	if (m_creatureStat->iHp < 100 && !m_bPhaseChangeMotionPlayed)
	{
		m_bPhaseChangeMotionPlayed = true;
		m_eState = BOSS_GROGGY;
		ChangeAnim(m_pInjured);
		m_pDynamicTex->SetFrameSpeed(2.f);
		for (auto& iter : m_pShootingStarList)
			iter->Destroy();
		
		m_pShootingStarList.clear();
	}

	if (m_creatureStat->iHp <= 0 && m_eState != BOSS_DEAD)
	{
		m_pBackLight->Destroy();

		ChangeAnim(m_pInjured);
		m_pDynamicTex->SetFrameSpeed(2.f);
		m_ePhase = PHASE1;
		m_eState = BOSS_DEAD;
	}

	_int	iExit = __super::Update_GameObject(fTimeDelta);

	// 2페때 공중에서 날리기
	if (m_ePhase == PHASE2)
	{
		m_bOnAir = true;

		m_fFlyTime += fTimeDelta;

		if (m_fFlyTime < 2.f)
		{
			m_pTransformCom->Move_Forward(&_vec3(0.f, 1.f, 0.f), fTimeDelta, 4.f);
		}
	}
	else
	{
		m_pTransformCom->Move_Forward(&_vec3(0.f, -1.f, 0.f), fTimeDelta, 3.f);
	}


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

	Engine::AddCollider(m_pColliderCom);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void BossMonster1::LateUpdate_GameObject()
{
	OnCollided();

	__super::LateUpdate_GameObject();
}

void BossMonster1::Render_GameObject()
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

HRESULT BossMonster1::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pIdle = AnimationTexture::Create(m_pGraphicDev, 
		L"../../Resource/Texture/Boss/Palkia/Idle/Idle_%d.png", 2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"IdleTex", pComponent });

	pComponent = m_pSleep = AnimationTexture::Create(m_pGraphicDev, 
		L"../../Resource/Texture/Boss/Palkia/Sleep/Sleep_%d.png", 2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"SleepTex", pComponent });
	
	pComponent = m_pWalk = AnimationTexture::Create(m_pGraphicDev, 
		L"../../Resource/Texture/Boss/Palkia/Walk/Walk_%d.png", 4);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"WalkTex", pComponent });
	
	pComponent = m_pScratch = AnimationTexture::Create(m_pGraphicDev, 
		L"../../Resource/Texture/Boss/Palkia/Scratch/Scratch_%d.png", 15);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"ScratchTex", pComponent });
		
	pComponent = m_pRoar = AnimationTexture::Create(m_pGraphicDev, 
		L"../../Resource/Texture/Boss/Palkia/Roar/Roar_%d.png", 14);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"RoarTex", pComponent });
		
	pComponent = m_pInjured = AnimationTexture::Create(m_pGraphicDev, 
		L"../../Resource/Texture/Boss/Palkia/Injured/Injured_%d.png", 2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"InjuredTex", pComponent });
		
	pComponent = m_pCharge = AnimationTexture::Create(m_pGraphicDev, 
		L"../../Resource/Texture/Boss/Palkia/Charge/Charge_%d.png", 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"ChargeTex", pComponent });

	pComponent = m_pShoot = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Palkia/Shoot/Shoot_%d.png", 15);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"ShootTex", pComponent });

	pComponent = m_pJumpAttack = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Boss/Palkia/JumpAttack/JumpAttack_%d.png", 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"JumpAttackTex", pComponent });

	m_pDynamicTex = m_pSleep;
	m_mapComponent[ID_DYNAMIC].insert({ L"DynamicTex",m_pDynamicTex });

	m_pDynamicTex->SetFrameSpeed(2.0f);

	return S_OK;
}

void BossMonster1::OnCollided()
{
	// 지형, 벽 충돌
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
			pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 211, 60, 0));
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
			pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 211, 60, 0));
		}

	}

	if (m_pColliderCom->GetColOBB() & COL_EXPLOSION)
	{
		m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_EXPLOSION).front().first.m_iDamage;
	}

}

void BossMonster1::OnDamaged()
{

}

void BossMonster1::ChangeAnim(AnimationTexture* pAnim)
{
	m_pDynamicTex->SetFrame(0.f);
	m_mapComponent[ID_DYNAMIC][L"DynamicTex"] = pAnim;
	m_pDynamicTex = pAnim;
}

int BossMonster1::GetPattern()
{
	if (m_pCurTable == nullptr || m_bPahseChanged || curPattern == m_pCurTable->end())
	{
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

// 페이즈
void BossMonster1::Phase1(const _float& fTimeDelta)
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
		ThrowCube(fTimeDelta);
		break;
	case BOSS_PATTERN2:
		//Clow(fTimeDelta);
		ShootStar(fTimeDelta);
		break;
	case BOSS_PATTERN3:
		JumpAttack(fTimeDelta);
		break;
	case BOSS_GROGGY:
		PatternChange(fTimeDelta);
		break;
	case BOSS_DEAD:
		OnDead(fTimeDelta);
		break;
	default:
		break;
	}
}

void BossMonster1::Phase2(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case BOSS_IDLE:
		OnIdle(fTimeDelta);
		break;
	case BOSS_PATTERN1:
		ThrowCube(fTimeDelta);
		break;
	case BOSS_PATTERN2:
		ShootStar(fTimeDelta);
		break;
	case BOSS_PATTERN3:
		DropMeteor(fTimeDelta);
		break;
	case BOSS_GROGGY:
		break;
	case BOSS_HIT:
		break;
	case BOSS_DEAD:
		break;
	default:
		break;
	}
}

void BossMonster1::TargetDistUpdate()
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

void BossMonster1::DetectTarget(const _float& fTimeDelta)
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

void BossMonster1::OnIdle(const _float& fTimeDelta)
{
	m_fPatternInterval += fTimeDelta;

	// 일정 시간 이후마다 패턴 실행
	if (m_fPatternInterval >= 2.5f)
	{
		if(m_ePhase == PHASE2)
			TelePort(fTimeDelta);
		
		m_fPatternInterval = 0.f;

		int iRandNum = GetPattern();
		//UINT iRandNum = 1;

		switch (iRandNum)
		{
		case 0:
			m_eState = BOSS_PATTERN1;
			ChangeAnim(m_pShoot);
			m_pDynamicTex->SetFrameSpeed(8.f);
			break;
		case 1:
			m_eState = BOSS_PATTERN2;
			ChangeAnim(m_pScratch);
			m_pDynamicTex->SetFrameSpeed(14.f);
			break;
		case 2:
			if (m_ePhase == PHASE1)
			{
				ChangeAnim(m_pJumpAttack);
				m_pDynamicTex->SetFrameSpeed(8.f);
			}
			else
			{
				ChangeAnim(m_pRoar);
				Engine::Play_Sound(L"palkia", SOUND_EFFECT_3, 1.f);
				m_pDynamicTex->SetFrameSpeed(8.f);
			}

			m_eState = BOSS_PATTERN3;
			break;
		}
	}
}

void BossMonster1::OnAwake(const _float& fTimieDelta)
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

void BossMonster1::PatternChange(const _float& fTimeDelta)
{
	m_fPatternInterval += fTimeDelta;
	// 3초간 그로기 되고 2페이즈로 전환
	if (m_fPatternInterval > 3.f)
	{
		if (!m_bRoared)
		{
			Engine::Play_Sound_NoInterval(L"palkia", SOUND_EFFECT_3, 0.8f);

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

				m_pBackLight = BossBackLight::Create(m_pGraphicDev,m_pTarget->Get_Transform(), m_pTransformCom, BOSS1);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
					Add_GameObject(L"BossBackLight", m_pBackLight);
			}
		}
	}
}

void BossMonster1::OnDead(const _float& fTimeDelta)
{
	for (auto& iter : m_pShootingStarList)
	{
		if(iter != nullptr)
			iter->Destroy();
	}

	m_pShootingStarList.clear();

	m_fPatternInterval += fTimeDelta;

	if (m_fPatternInterval > 6.f)
	{
		m_bAlive = false;

		// 포탈 생성
		BossPortal* pPortal = BossPortal::Create(m_pGraphicDev, m_pTarget->Get_Transform(), &_vec3(118.f,30.f,168.f));
		Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
			Add_GameObject(L"Portal", pPortal);
	}
}

void BossMonster1::ThrowCube(const _float& fTimeDelta)
{
	UINT endFrame = m_pDynamicTex->GetLastFrameIndex();
	UINT repeatCnt = m_pDynamicTex->GetRepeatCnt();

	if (m_pDynamicTex->GetFrame() >= endFrame - 6 && m_iRepeatCnt != repeatCnt)
	{
		Engine::Play_Sound_NoInterval(L"energyBall", SOUND_EFFECT_3, 1.f);

		_vec3 initPos;
		m_pTransformCom->Get_Info(INFO_POS, &initPos);
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		initPos += vLook * 0.5f;
		
		// 패턴 강화
		if (m_ePhase == PHASE2)
		{
			int cnt = (360 / 60);
			_float angle = 360.f / 6.f;
			_vec3 vInitDir = { 0.f,1.f,0.f };

			for (int i = 0; i < cnt; ++i)
			{
				_vec3 Dir, Pos;
				EnergyBall* Obj = EnergyBall::Create(m_pGraphicDev, &vInitDir, m_pTarget->Get_Transform(), 1, 5.f);
				Obj->Get_Transform()->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
				Obj->Get_Transform()->Set_Pos(initPos);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
					Add_GameObject(L"EnergyBall", Obj);

				_matrix rotAxis;
				D3DXQUATERNION qRot;
				D3DXQuaternionRotationAxis(&qRot, &vLook, D3DXToRadian(angle));
				D3DXMatrixRotationQuaternion(&rotAxis, &qRot);
				D3DXVec3TransformNormal(&vInitDir, &vInitDir, &rotAxis);
			}
		}
		else
		{
			_vec3 vInitDir;
			m_pTransformCom->Get_Info(INFO_RIGHT, &vInitDir);
			D3DXVec3Normalize(&vInitDir, &vInitDir);

			for (int i = 0; i < 4; ++i)
			{
				_vec3 Dir, Pos;
				EnergyBall* Obj = EnergyBall::Create(m_pGraphicDev, &vInitDir, m_pTarget->Get_Transform(), 1, 5.f);
				Obj->Get_Transform()->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
				Obj->Get_Transform()->Set_Pos(initPos);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
					Add_GameObject(L"EnergyBall", Obj);

				_matrix rotAxis;
				D3DXQUATERNION qRot;
				D3DXQuaternionRotationAxis(&qRot, &vLook, D3DXToRadian(60.f));
				D3DXMatrixRotationQuaternion(&rotAxis, &qRot);
				D3DXVec3TransformNormal(&vInitDir, &vInitDir, &rotAxis);
			}
		}

		m_iRepeatCnt = repeatCnt;
		m_iShootCnt++;
	}

	// 4번 쏘면 탈출
	if (m_iShootCnt >= 4 && m_pDynamicTex->GetFrame() >= endFrame)
	{
		m_iShootCnt = 0;
		m_iRepeatCnt = -1;

		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
	}
}

void BossMonster1::Clow(const _float& fTimeDelta)
{
	UINT endFrame = m_pDynamicTex->GetLastFrameIndex();
	UINT repeatCnt = m_pDynamicTex->GetRepeatCnt();

	_vec3 initPos,MyPos;
	m_pTransformCom->Get_Info(INFO_POS, &initPos);
	MyPos = initPos;
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	initPos -= vLook * 0.5f;

	if (m_pDynamicTex->GetFrame() >= endFrame - 4 && m_iRepeatCnt != repeatCnt)
	{
		Engine::Play_Sound_NoInterval(L"boss_attacks_01", SOUND_EFFECT_2, 1.f);

		// 패턴 강화
		if (m_ePhase == PHASE2)
		{

		}
		else
		{
			_vec3 Dir, Pos;
			m_pTarget->Get_Transform()->Get_Info(INFO_POS, &Pos);
			Dir = Pos - MyPos;
			D3DXVec3Normalize(&Dir, &Dir);

			UINT iRandNum = Engine::Rand_Num(0, 1);

			if (iRandNum == 0)
			{
				_vec3 Normal = { Dir.z,0.f,-Dir.x };
				D3DXVec3Normalize(&Normal, &Normal);
				initPos -= Normal * 4.5f;
				_vec3 tempPos;

				for (int i = 0; i < 4; ++i)
				{
					tempPos = initPos + Normal * (3.f * i);

					SpacialRend* Obj = SpacialRend::Create(m_pGraphicDev, &Dir, 25.f,false);
					Obj->Get_Transform()->Set_Scale(_vec3(0.5f, 5.f, 1.f));
					Obj->Get_Transform()->Set_Pos(tempPos);
					Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
						Add_GameObject(L"SpacialRend", Obj);
				}
			}
			else
			{
				_vec3 Normal = { 0.f,-Dir.z,Dir.y };
				D3DXVec3Normalize(&Normal, &Normal);
				initPos -= Normal * 1.2f;
				_vec3 tempPos;

				for (int i = 0; i < 2; ++i)
				{
					tempPos = initPos + Normal * (5.f * i);

					SpacialRend* Obj = SpacialRend::Create(m_pGraphicDev, &Dir, 25.f, true);
					Obj->Get_Transform()->Set_Scale(_vec3(5.f, 0.5f, 5.f));
					Obj->Get_Transform()->Set_Pos(tempPos);
					Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
						Add_GameObject(L"SpacialRend", Obj);
				}
			}
		}

		m_iRepeatCnt = repeatCnt;
		m_iShootCnt++;
	}

	// 4번 쏘면 탈출
	if (m_iShootCnt >= 4 && m_pDynamicTex->GetFrame() >= endFrame)
	{
		m_iShootCnt = 0;
		m_iRepeatCnt = -1;

		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
	}

}

void BossMonster1::JumpAttack(const _float& fTimeDelta)
{
	_vec3 TargetDir, targetPos, myPos;
	_float length;
	m_pTarget->Get_Transform()->Get_Info(INFO_POS, &targetPos);
	m_pTransformCom->Get_Info(INFO_POS, &myPos);
	TargetDir = targetPos - myPos;
	TargetDir.y = 0.f;
	// 직선거리
	length = D3DXVec3Length(&TargetDir);
	D3DXVec3Normalize(&TargetDir, &TargetDir);

	_float curFrame = m_pDynamicTex->GetFrame();

	// 7프레임까지 점프
	if ((UINT)curFrame < (UINT)7)
	{
		//  기본 추락속도 3을 상쇄 하려고
		m_pTransformCom->Move_Forward(&_vec3(0.f, 1.f, 0.f), fTimeDelta, 20);
		//m_pTransformCom->Move_Forward(&TargetDir, fTimeDelta, length * 0.5f);
		m_bOnTerrain = false;
	}
	// 9프레임 고정하여 내려찍기
	else if ((UINT)curFrame >= (UINT)9)
	{
		m_pDynamicTex->SetFrameSpeed(0.f);
		m_pTransformCom->Move_Forward(&_vec3(0.f,-1.f,0.f), fTimeDelta, 15.f);
		m_pTransformCom->Move_Forward(&TargetDir, fTimeDelta, length * 2.f);
	}

	// 내려 찍었으면 끝
	if ((UINT)curFrame >= (UINT)9 && m_bOnTerrain)
	{
		SingleCamera::GetInstance()->PlayCameraShake(0.3f, 0.2f);
		Engine::Play_Sound_NoInterval(L"boss_attacks_02", SOUND_EFFECT_3, 1.f);

		_vec3 TempPos;
		m_pTransformCom->Get_Info(INFO_POS, &TempPos);
		TempPos.y -= 1.9f;
		JumpCrack* Obj = JumpCrack::Create(m_pGraphicDev, &TempPos,1);
		Obj->Get_Transform()->Set_Scale(_vec3(6.f, 6.f, 1.f));
		Obj->Get_Transform()->Rotation(ROT_X,D3DXToRadian(90.f));
		Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
			Add_GameObject(L"JumpCrack", Obj);

		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
	}

}

void BossMonster1::ShootStar(const _float& fTimeDelta)
{
	_float curFrame = m_pDynamicTex->GetFrame();
	_vec3 myPos, myRight;
	m_pTransformCom->Get_Info(INFO_POS, &myPos);
	m_pTransformCom->Get_Info(INFO_RIGHT, &myRight);

	// 8프레임부터 에너지 볼 생성
	if ((UINT)curFrame < (UINT)8)
	{
		Engine::Play_Sound(L"en_beholder_idle_03", SOUND_EFFECT_1, 0.5f);
		m_pDynamicTex->SetFrameSpeed(0.f);

		m_fPatternInterval += fTimeDelta;
		if (m_fPatternInterval > 0.1f)
		{
			m_fPatternInterval = 0.f;

			ShootingStar* pStar = ShootingStar::Create(m_pGraphicDev, m_pTarget->Get_Transform(), 50.f,1);
			int iOffset = Engine::Rand_Num(5,10);
			_vec3 pos = myPos;
			pos.y += float(iOffset);
			
			iOffset = Engine::Rand_Num(-5, 5);
			pos += myRight * float(iOffset);

			pStar->Get_Transform()->Set_Pos(pos);
			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"ShootingStar", pStar);

			// 쏘기 위해 집어넣기
			m_pShootingStarList.push_back(pStar);
		}

		// 20개 생성했으면 애니메이션 진행
		if (m_ePhase == PHASE2)
		{
			if (m_pShootingStarList.size() > 20)
				m_pDynamicTex->SetFrameSpeed(14.f);
		}
		else
		{
			if (m_pShootingStarList.size() > 10)
				m_pDynamicTex->SetFrameSpeed(14.f);
		}
	}
	// 9프레임 고정하여 내려찍기
	else if ((UINT)curFrame >= (UINT)12 && (UINT)curFrame < (UINT)13)
	{
		Engine::Play_Sound(L"en_beholder_idle_02", SOUND_EFFECT_3, 1.f);

		m_pDynamicTex->SetFrameSpeed(0.f);

		m_fPatternInterval += fTimeDelta;
		if (m_fPatternInterval > 0.1f)
		{
			m_fPatternInterval = 0.f;

			if (m_pShootingStarList.empty())
			{
				m_pDynamicTex->SetFrameSpeed(14.f);
			}
			else
			{
				(*m_pShootingStarList.begin())->Shoot();
				m_pShootingStarList.pop_front();
			}
		}
	}
	// 내려 찍었으면 끝
	else if ((UINT)curFrame >= (UINT)13)
	{
		m_fPatternInterval = 0.f;
		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
		m_pShootingStarList.clear();
	}

}

void BossMonster1::DropMeteor(const _float& fTimeDelta)
{
	_float curFrame = m_pDynamicTex->GetFrame();
	UINT   endFrame = m_pDynamicTex->GetLastFrameIndex();

	// 광선 기준점 -> 랜덤하게 생성
	_vec3 InitPos = { 118.f,50.f,168.f };

	if ((UINT)curFrame >= (UINT)9 && (UINT)curFrame < (UINT)11)
	{
		m_pDynamicTex->SetFrameSpeed(0.f);

		if (m_iMeteorRayCnt > 10)
		{
			m_pDynamicTex->SetFrameSpeed(8.f);

		}

		m_fPatternInterval += fTimeDelta;
		if (m_fPatternInterval > 0.1f && m_iMeteorRayCnt <= 10)
		{
			m_fPatternInterval = 0.f;

			int randX = Engine::Rand_Num(-30, 30);
			int randZ = Engine::Rand_Num(-30, 30);

			_vec3 pos = InitPos;
			pos.x += (_float)randX;
			pos.z += (_float)randZ;

			MeteorLine* Obj = MeteorLine::Create(m_pGraphicDev, &pos, 7.5f, 1);
			Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
				Add_GameObject(L"MeteorLine", Obj);

			m_iMeteorRayCnt++;
		}
	}
	// 내려 찍었으면 끝
	else if (curFrame >= endFrame)
	{
		m_iMeteorRayCnt = 0;
		m_fPatternInterval = 0.f;
		m_eState = BOSS_IDLE;
		ChangeAnim(m_pIdle);
		m_pDynamicTex->SetFrameSpeed(2.f);
	}

}

void BossMonster1::TelePort(const _float& fTimeDelta)
{
	int iRand = Engine::Rand_Num(0, 3);

	// 1/4확률로 텔포
	if (iRand == 0)
	{
		Engine::Play_Sound_NoInterval(L"Teleport", SOUND_EFFECT_3, 0.5f);

		// 위치 섞기
		std::shuffle(m_vecPos.begin(), m_vecPos.end(), std::default_random_engine());

		_vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
		myPos.x = m_vecPos[0].x;
		myPos.z = m_vecPos[0].z;

		m_pTransformCom->Set_Pos(myPos);
	}
}

BossMonster1* BossMonster1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	BossMonster1* pInstance = new BossMonster1(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossMonster1 Create Failed");
		return nullptr;
	}

	return pInstance;
}

void BossMonster1::Free()
{
	m_mapComponent[ID_DYNAMIC].erase(L"DynamicTex");
	__super::Free();
}
