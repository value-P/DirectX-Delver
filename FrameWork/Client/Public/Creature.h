#pragma once

#include "GameObject.h"

namespace Engine
{
	class RcTex;
	class Transform;
	class Calculator;
	class Collider;
}

class ItemBase;

// Temp, Need to revise
struct CreatureStat
{
	CreatureStat() {}
	CreatureStat(_int hp, _int maxHp, _int attack = 0, _int defense = 0, _uint money = 0, _int exp = 0, _int maxExp = 0, _uint level = 1)
		: iHp(hp), iMaxHp(maxHp), iAttack(attack), iDefense(defense), iMoney(money), iExp(exp), iMaxExp(maxExp), iLv(level) {}

	_int iHp = 0;
	_int iMaxHp = 0;

	_int iAttack = 0;
	_int iRangedAttack = 0;
	_int iMagicAttack = 0;
	_int iDefense = 0;
	_float iMove_Speed = 0.f;

	_uint iMoney = 0; // Player Only
	_int iExp = 0;
	_int iMaxExp = 0; // Player Only
	_uint iLv = 1; // Player Only
};

// Player와 Monster의 공통된 기능 및 변수들을 모을 상위 클래스
// 만드는 목적 : Player와 Monster를 편하게 관리하기 위해서
// 따로 Instance할 예정이 전혀 없다(Creature만 따로 생성X)
class Creature : public GameObject
{
	using Super = Engine::GameObject; // typedef라고 생각하면 편하다 / Super:: == GameObject::

protected:
	explicit Creature(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Creature(const Creature& rhs);
	virtual ~Creature();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	CreatureStat* Get_CreatureStat() { return m_creatureStat; }
	void Set_CreatureStat(CreatureStat* creatureStat);

	_int Get_CreatureHp() { return m_creatureStat->iHp; }
	void Set_CreatureHp(_int hp) { m_creatureStat->iHp = hp; }

	_int Get_CreatureMaxHp() { return m_creatureStat->iMaxHp; }
	void Set_CreatureMaxHp(_int maxHp) { m_creatureStat->iMaxHp = maxHp; }

	_int Get_CreatureAttack() { return m_creatureStat->iAttack; }
	void Set_CreatureAttack(_int attack) { m_creatureStat->iAttack = attack; }
	
	
	//
	_int Get_CreatureRangedAttack() { return m_creatureStat->iRangedAttack; }
	void Set_CreatureRangedAttack(_int RangedAttack) { m_creatureStat->iRangedAttack = RangedAttack; }

	_int Get_CreatureMagicAttack() { return m_creatureStat->iMagicAttack; }
	void Set_CreatureMagicAttack(_int MagicAttack) { m_creatureStat->iMagicAttack = MagicAttack; }

	_float Get_CreatureMove_Speed() { return m_creatureStat->iMove_Speed; }
	void Set_CreatureMove_Speed(_float Move_Speed) { m_creatureStat->iMove_Speed = Move_Speed; }
	//
	
	
	_int Get_CreatureDefense() { return m_creatureStat->iDefense; }
	void Set_CreatureDefense(_int defense) { m_creatureStat->iDefense = defense; }

	_uint Get_PlayerMoney() { return m_creatureStat->iMoney; }
	void Set_PlayerMoney(_uint money) { m_creatureStat->iMoney = money; }

	_uint Get_PlayerLevel() { return m_creatureStat->iLv; }
	void Set_PlayerLevel(_uint level) { m_creatureStat->iLv = level; }

	_int Get_CreatureExp() { return m_creatureStat->iExp; }
	void Set_PlayerExp(_int exp) { m_creatureStat->iExp = exp; }

	_int Get_PlayerMaxExp() { return m_creatureStat->iMaxExp; }
	void Set_PlayerMaxExp(_int maxExp) { m_creatureStat->iMaxExp = maxExp; }

protected:
	HRESULT			Add_Component(); // Player와 Monster의 공통된 Component는 여기서 추가할 예정
	void			PlaneCollision(bool* _bOnTerrain = nullptr);
	virtual void	OnDamaged() {}

protected:
	virtual void	Free();

protected:
	Collider* m_pColliderCom{ nullptr };

	_float		m_fSpeed = 0.f;

	CreatureStat* m_creatureStat{ nullptr };
};
