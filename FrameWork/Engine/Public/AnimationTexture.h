#pragma once

#include "Texture.h"

namespace Engine 
{
	class ENGINE_DLL AnimationTexture : public Texture
	{
	private:
		explicit AnimationTexture(LPDIRECT3DDEVICE9 _pGraphicDevice);
		explicit AnimationTexture(const AnimationTexture& _rhs);
		virtual ~AnimationTexture();

	public:
		static AnimationTexture* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const wstring _pPath, const _uint& _iCnt = 1, const float& _fFrameSpeed = 1.0f);
		virtual Component* Clone();

	public:
		HRESULT	Init_AnimationTexture(const wstring _pPath, const _uint& _iCnt, const float& _fFrameSpeed);

	public:
		virtual int Update_Component(const _float& _fTimeDelta);

	public:		// Get Function
		virtual IDirect3DBaseTexture9* Get_Texture();						// ���� �ؽ��ĸ� ������
		virtual IDirect3DBaseTexture9* Get_Texture(const UINT& _iIndex);		// Index�� �ؽ��ĸ� ������
		const float& GetFrame() { return m_fFrame; }
		const float& GetFrameSpeed() { return m_fFrameSpeed; }
		const UINT& GetRepeatCnt() { return m_iRepeatCnt; }

		UINT GetLastFrameIndex() { return m_iMaxNum-1; }
		wstring GeT_CurTextureState();

	public:		// Set Function
		void SetFrame(const float& _fFrame);
		void SetFrameSpeed(const float& _fFrameSpeed);		// 1.0f�� �ʴ� 1��

	public:		// Class Function
		virtual void Set_Texture();
		virtual void Set_Texture(const UINT& _iIndex);		// ���ϴ� ��ȣ�� �ؽ��ĸ� �����Ϸ��� �̰ɷ� ������

	private:
		float m_fFrame{ 0.0f };				// ���� ������
		float m_fFrameSpeed{ 1.0f };		// 1.0f�� �ʴ� 1��
		UINT m_iMaxNum{ 0 };			// �̹��� ������ �ڵ����� ��
		UINT m_iRepeatCnt{ 0 };
		wstring m_pPath;
		wstring m_pCurTextureState;

	private:
		virtual void Free();
	};
}
