#pragma once

#include "Creature.h"

namespace Engine
{
	class Texture;
}

class ItemBase;
class InventoryBase;
class QuickSlot;
class TradeInv;
class MyHUD;

class NonPlayerCharacter : public Creature
{
	using Super = Creature;

private:
	explicit NonPlayerCharacter(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit NonPlayerCharacter(const NonPlayerCharacter& rhs);
	virtual ~NonPlayerCharacter();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	

	TradeInv* Get_TradeInv() { return m_pTradeInv; }
	NPCTYPE Get_NPCType() { return m_eNpcType; }
	vector<vector<wstring>> Get_NPCDialog() { return m_pNPCDialogVec; }

public:
	void Set_NPCType(NPCTYPE npcType) { m_eNpcType = npcType; }
	void Set_NPCDialog(vector<vector<wstring>> _pNPCDialogVec) { m_pNPCDialogVec = _pNPCDialogVec; }
	void Set_IsDialogClosed(bool _bIs_DialogClosed) { m_bIs_DialogClosed = _bIs_DialogClosed; }
	void Set_FrameIndax(_float _fIdx_Frame) { m_fIdx_Frame = _fIdx_Frame; }
	void Set_m_bQuest_NPC(bool _bQuest_NPC) { m_bQuest_NPC = _bQuest_NPC; }
public:
	void Talknpc();
	
	

public:
	static NonPlayerCharacter* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _float _fIdx_Frame);

protected:
	virtual void OnCollided() override;

private:
	HRESULT Add_Component(wstring textureKey);

private:
	virtual void	Free();

private:
	MyHUD* m_pMyHUD = nullptr;

	TradeInv* m_pTradeInv = nullptr;

	float m_fFrame = 0.f;

	NPCTYPE m_eNpcType = NPC_END;
	vector<vector<wstring>> m_pNPCDialogVec;

	_float m_fIdx_Frame = 0.f;
	_float m_fAlpha_Font = 255.f;

	bool m_bIs_DialogClosed = false;
	bool m_bIs_ColPlayer = false;

	bool m_bQuest_NPC = false;
	bool m_bFontON_TechniqueTransfer = false;
	bool m_bFontON_TechniqueExplain = false;

};

