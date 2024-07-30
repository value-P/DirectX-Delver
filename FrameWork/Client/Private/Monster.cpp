#include "stdafx.h"
#include "Monster.h"

#include "Export_Utility.h"
#include "Explosion.h"
#include "Fragment.h"
#include "MagicEx.h"
#include "ItemBase.h"
#include "Effect_Bundle.h"
#include "Magic.h"
#include "Player.h"
#include "Effect_Bundle.h"
#include "SoundMgr.h"
#include "Gold.h"

Monster::Monster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{

}

Monster::Monster(const Monster& rhs)
	: Super(rhs)
{

}

Monster::~Monster()
{
}

HRESULT Monster::Init_GameObject()
{
	Super::Init_GameObject();
	// 몬스터 젠 랜덤 생성.
	// _vec3 randomPos = { static_cast<float>(rand() % 120), 0.f, static_cast<float>(rand() % 120) };
	// m_pTransformCom->Set_Info(INFO_POS, &randomPos);

	m_pColliderCom->SetSize(1.0f, 1.0f, 1.0f);
	m_pColliderCom->SetColLayer(COL_MONSTER);

	return S_OK;
}

_int Monster::Update_GameObject(const _float& fTimeDelta)
{
	// Init에 추가하지 못해서 여기에 추가!
	if (!pPlayer) 
		pPlayer = dynamic_cast<Player*>((*(Engine::Get_GameObject(L"GameObject", L"Player"))).front());

	if (!m_pPlayerTransformCom)
	{
		// [우채] : Init 단계에서 수행이 안되는 관계로 일단 여기서 정리한다.
		Engine::Transform* pPlayerTransformCom = dynamic_cast<Transform*>(Engine::Get_Component(L"GameObject", L"Player", L"Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransformCom, -1);
		m_pPlayerTransformCom = pPlayerTransformCom;
	}

	// 몬스터->플레이어 추적 활성화 조건
	_vec3 vDir_Player = m_pPlayerTransformCom->Get_Info(INFO_POS) - m_pTransformCom->Get_Info(INFO_POS);
	m_fDistance_Player = D3DXVec3Length(&vDir_Player);
	

	// 몬스터 피격 스턴 지연.
	if (m_bIs_HitStunState)
	{
		if(m_creatureStat->iHp > 0)
		{
			if (m_bIsStunStart)
			{
				switch (m_eMonsterID)
				{
				case Engine::MI_SLIME:
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Slime_Hit", SOUND_SLIME, 0.6f);
					break;
				case Engine::MI_BAT:
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Bat_Hit", SOUND_BAT, 0.6f);
					break;
				case Engine::MI_WARRIOR:
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Warrior_Hit", SOUND_WARRIOR, 0.6f);
					break;
				case Engine::MI_WIZARD:
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Wizard_Hit", SOUND_WIZARD, 0.6f);
					break;
				case Engine::MI_ZOMBI:
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Zombi_Hit", SOUND_ZOMBI, 0.6f);
					break;
				case Engine::MI_DRUID:
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Druid_Hit", SOUND_DRUID, 0.6f);
					break;
				case Engine::MI_MONK:
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Monk_Hit", SOUND_MONK, 0.6f);
					break;
				}

				m_bIsStunStart = false;
				m_pTextureCom = m_pHit_TextureCom;
				dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.f);
				dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrame(0);
			}
			if (m_fDelayTime_HitStun < m_fCoolTime_HitStun)
			{
				m_fDelayTime_HitStun += fTimeDelta;
				Lock_Tracking = true;
			}
			else
			{
				m_bIsStunStart = true;
				m_bIs_HitStunState = false;
				m_fDelayTime_HitStun = 0.f;

				m_pTextureCom = m_pIdle_TextureCom;
				dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.f);
				dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrame(0);
			}
		}
	}

	// 범위 체크!
	if(m_bIs_HitStunState == false)
	{
		// 시야 범위 안에 있고, 공격 가능 거리에 도달하지 못했고, 피격당한 상태가 아니라면 움직여라
		if (m_creatureStat->iHp > 0 && m_fDistance_Player <= m_fSight_Range && m_fDistance_Player > m_fAttackAble_Range)
			Lock_Tracking = false;
		// 시야 범위 안에 있고, 공격 가능 거리에 도달했고
		else if (m_creatureStat->iHp > 0 && m_fDistance_Player <= m_fSight_Range && m_fDistance_Player <= m_fAttackAble_Range)
		{
			Lock_Tracking = true; // 이동을 멈추고
			NormalAttack(vDir_Player); // 공격해라
		}
		else
		{
			Lock_Tracking = true;
			if (m_creatureStat->iHp > 0)
			{
				m_pTextureCom = m_pIdle_TextureCom;
				dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.f); // 공격 애니매이션 프레임 속도는 1초에 8장
			}
		}
	}
	// 몬스터 기본 공격 쿨타임.
	if (m_bCoolDown_NormalAttack) // 기본값 false; 몬스터의 일반공격
	{
		if (m_fDelayTime_NormalAttack < m_fCoolTime_NormalAttack)
			m_fDelayTime_NormalAttack += fTimeDelta;
		else
		{
			m_bCoolDown_NormalAttack = false;
			m_fDelayTime_NormalAttack = 0.f;
		}
	}

	// 사망 애니매이션 지연 처리!
	if (m_creatureStat->iHp <= 0)
	{
		if (m_bIsDeathStart)
		{
			switch (m_eMonsterID)
			{
			case Engine::MI_SLIME:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Slime_Death", SOUND_SLIME, 0.6f);
				break;
			case Engine::MI_BAT:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Bat_Death", SOUND_BAT, 0.6f);
				break;
			case Engine::MI_WARRIOR:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Warrior_Death", SOUND_WARRIOR, 0.6f);
				break;
			case Engine::MI_WIZARD:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Wizard_Death", SOUND_WIZARD, 0.6f);
				break;
			case Engine::MI_ZOMBI:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Zombi_Death", SOUND_ZOMBI, 0.6f);
				break;
			case Engine::MI_DRUID:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Druid_Death", SOUND_DRUID, 0.6f);
				break;
			case Engine::MI_MONK:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Monk_Death", SOUND_MONK, 0.6f);
				break;
			}

			m_bIsDeathStart = false;
			m_pTextureCom = m_pDeath_TextureCom;
			dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.f); // 공격 애니매이션 프레임 속도는 1초에 8장
			dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrame(0);

			// Temp by Gwan
			pPlayer->AddKillCount();
			pPlayer->Set_PlayerExp(pPlayer->Get_CreatureExp() + Get_CreatureExp());

			Gold* gold = Gold::Create(m_pGraphicDev, L"Proto_Gold", _vec3(0.3f, 0.3f, 0.3f), COL_DROPITEM, L"Gold", ITEMID_CONSUMABLE, ITEM_GOLD, 15, 1, false, false);
			if (gold)
			{
				gold->Get_Transform()->Set_Pos(Get_Transform()->Get_Info(INFO_POS));
				gold->Get_Transform()->Set_Scale(_vec3(0.3f, 0.3f, 0.3f));
				Engine::Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"Gold", gold);
			}
		}

		_float fCurFrame = UINT(dynamic_cast<AnimationTexture*>(m_pTextureCom)->GetFrame());
		UINT iMaxNum = dynamic_cast<AnimationTexture*>(m_pTextureCom)->GetLastFrameIndex();
		
		if (UINT(fCurFrame) == iMaxNum)
		{
			dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrame(fCurFrame);
			m_fDelayTime_Death += fTimeDelta;

			if (m_fDelayTime_Death > 5.f)
			{
				return -1;
			}
		}
	}

	_int	iExit = Super::Update_GameObject(fTimeDelta);

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
	m_pColliderCom->ReBuild();
	Engine::AddCollider(m_pColliderCom);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	
	return iExit;
}

void Monster::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();

	OnCollided();
	m_pColliderCom->ReBuild();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	Texture* test = m_pTextureCom;

}

void Monster::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT Monster::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ textureKey, pComponent });

	return S_OK;
}

void Monster::OnCollided()
{
}

void Monster::OnDamaged()
{
	list<pair<CINFO, D3DXVECTOR3>> attackCol = m_pColliderCom->GetColSphere_Info(COL_PLAYERATTACK);

	for (auto it = attackCol.begin(); it != attackCol.end(); ++it)
	{
		ItemBase* item = static_cast<ItemBase*>(it->first.m_pThis);
		if (item)
		{
			switch (item->Get_ItemType())
			{
			case ITEM_MELEEWEAPON:
			{
				// 공격 계수는 나중에 정하자~
				m_creatureStat->iHp -= item->Get_ItemTotalAttack();

			}
			break;
			case ITEM_PROJECTILE:
			{
				
			}
			break;
			}
		}

	}
}

void Monster::CreateParticles(D3DCOLOR _dwColor)
{
	PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
	if (nullptr == pPSystem) { return; }
	PSMgr::GetInstance()->AddParticles(pPSystem);
	for (int i = 0; i < 16; i++)
	{
		pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), _dwColor);
	}
}

void Monster::CreateHitEffect(const D3DXVECTOR3& _vPos)
{
	Effect* pEffect = Effect_Bundle::Create(m_pGraphicDev, EFFECT_0, _vPos);
	Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_ENVI)->Add_GameObject(L"Effect", pEffect);
}

Monster* Monster::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey)
{
	Monster* pInstance = new Monster(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Monster::Free()
{
	Super::Free();
}

void Monster::Set_BehaviorStat(_float _fSight_Range, _float _fAttackAble_Range, _float _fCoolTime_NormalAttack, _float _fSpeed)
{
	m_fSight_Range = _fSight_Range;
	m_fAttackAble_Range = _fAttackAble_Range;
	m_fCoolTime_NormalAttack = _fCoolTime_NormalAttack;
	m_fSpeed = _fSpeed;
}

// 몬스터 일반 공격
void Monster::NormalAttack(_vec3 _vDir_Player)
{
	if (!m_bCoolDown_NormalAttack) // 일반 공격이 쿨다운이 아닌경우
	{
		
		if (m_bIsNormalAttackStart) // 일반 공격 시작할 경우 최초 1회 셋팅
		{
			switch (m_eMonsterID)
			{
			case Engine::MI_SLIME:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Slime_Attack", SOUND_SLIME, 0.6f);
				break;
			case Engine::MI_BAT:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Bat_Attack", SOUND_BAT, 0.6f);
				break;
			case Engine::MI_WARRIOR:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Warrior_Attack", SOUND_WARRIOR, 0.6f);
				break;
			case Engine::MI_WIZARD:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Wizard_Attack", SOUND_WIZARD, 0.6f);
				break;
			case Engine::MI_ZOMBI:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Zombi_Attack", SOUND_ZOMBI, 0.6f);
				break;
			case Engine::MI_DRUID:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Druid_Attack", SOUND_DRUID, 0.6f);
				break;
			case Engine::MI_MONK:
				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"Monster_Monk_Attack", SOUND_MONK, 0.6f);
				break;
			}

			m_bIsNormalAttackStart = false;
			m_pTextureCom = m_pAttack_TextureCom; // 애니메이션 공격으로 바꾸고
			dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.f); // 공격 애니매이션 프레임 속도는 1초에 8장
			dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrame(0);
		}

		if (m_bIsNormalAttackEnd) // 일반 공격 종료할 경우 대기 상태로 복귀
		{

			m_bCoolDown_NormalAttack = true;
			m_bIsNormalAttackStart = true;
			m_bIsNormalAttackEnd = false;

			if (m_eMonsterID == MI_WIZARD)
			{
				D3DXVec3Normalize(&_vDir_Player, &_vDir_Player);
				Effect* pEffect = Magic::Create(m_pGraphicDev, MAGIC_1, MAGIC_EX_1, m_pTransformCom->Get_Info(INFO_POS) + _vDir_Player * 2.f, _vDir_Player, 30.0f, 5, D3DCOLOR_ARGB(255, 0, 0, 0));
				pEffect->SetColType(COL_MONSTER_PROJ);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"Magic", pEffect);
			
			}
			else if (m_eMonsterID == MI_DRUID)
			{
				D3DXVec3Normalize(&_vDir_Player, &_vDir_Player);
				Effect* pEffect = Magic::Create(m_pGraphicDev, MAGIC_1, MAGIC_EX_0, m_pTransformCom->Get_Info(INFO_POS) + _vDir_Player * 2.f, _vDir_Player, 30.0f, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
				pEffect->SetColType(COL_MONSTER_PROJ);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"Magic", pEffect);
			}
			else if (m_eMonsterID == MI_MONK)
			{
				D3DXVec3Normalize(&_vDir_Player, &_vDir_Player);
				Effect* pEffect = Magic::Create(m_pGraphicDev, MAGIC_1, MAGIC_EX_1, m_pTransformCom->Get_Info(INFO_POS) + _vDir_Player * 2.f, _vDir_Player, 30.0f, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
				pEffect->SetColType(COL_MONSTER_PROJ);
				Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"Magic", pEffect);
			}
			else
			{
				_int iRemaining_Hp = dynamic_cast<Creature*>(pPlayer)->Get_CreatureHp() - m_creatureStat->iAttack;
				dynamic_cast<Creature*>(pPlayer)->Set_CreatureHp(iRemaining_Hp);
			}
			m_pTextureCom = m_pIdle_TextureCom;
			dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.f);
			dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrame(0);
		}

		

		// 일반 공격 종료 체크
		_float fCurFrame = UINT(dynamic_cast<AnimationTexture*>(m_pTextureCom)->GetFrame());
		UINT iMaxNum = dynamic_cast<AnimationTexture*>(m_pTextureCom)->GetLastFrameIndex();
		if (UINT(fCurFrame) >= iMaxNum)
			m_bIsNormalAttackEnd = true;
	}
}