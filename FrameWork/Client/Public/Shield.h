#pragma once

#include "ItemBase.h"

// 왼손 장착
// 따로 관리 필요할 지도
// 장착했을 때 처리 따로 필요
class Shield : public ItemBase
{
	using Super = ItemBase;

protected:
	explicit Shield(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Shield(const Shield& rhs);
	virtual ~Shield();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	static Shield* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice, _uint itemCount = 1, _bool isStackable = false, _bool isActivated = false, _uint attack = 0, _uint defense = 0);
	virtual ItemBase* Clone() override;

protected:
	void StateIdle();
	void StateMove();
	void StateWeak();
	void StateStrong();

protected:
	HRESULT			Add_Component(wstring textureKey);
	virtual void OnCollided() override;

protected:
	virtual void Free();

private:
	float m_fDeltaTime = 0.f;
	float m_fShakingTime = 0.f;
	bool m_bIsDirUp = true;
};
