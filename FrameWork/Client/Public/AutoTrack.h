#pragma once

#include "InteractableBase.h"

class AutoTrack : public InteractableBase
{
	using Super = InteractableBase;

private:
	explicit AutoTrack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit AutoTrack(const AutoTrack& rhs);
	virtual ~AutoTrack();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void Set_DesiredPos(_vec3& desiredPos);
	void Set_DesiredPos(InteractableDir intDir);

	bool IsOnTrack() { return m_bIsOnTrack; }
	void SetIsOnTrack(bool isOnTrack) { m_bIsOnTrack = isOnTrack; }

	bool IsLastTrack() { return m_bIsLastTrack; }
	void SetIsLastTrack(bool isLastTrack) { m_bIsLastTrack = isLastTrack; }

public:
	static AutoTrack* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, InteractableDir intDir, InteractableType intType, _vec3& startPos, bool bIsLastTrack = false);

private:
	virtual void OnCollided() override;

private:
	void AutoTrackIdle();
	void AutoTrackMove();
	void AutoTrackAutoMove();

private:
	HRESULT Add_Component(const wstring& textureKey);

private:
	virtual void Free() override;

private:
	_vec3 m_vDesiredPos {};

	float m_fDeltaTime { 0.f };

	bool m_bIsOnTrack { false };
	bool m_bIsLastTrack { false };
	float m_fResetTime { 0.f };
};
