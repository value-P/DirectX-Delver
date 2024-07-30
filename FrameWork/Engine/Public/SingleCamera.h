#pragma once

#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
	class Transform;

	class ENGINE_DLL SingleCamera : public Base
	{
	public:
		static SingleCamera* GetInstance()
		{
			if (nullptr == m_pInstance)
			{
				m_pInstance = new SingleCamera();
			}
			return m_pInstance;
		}
		static void DestroyInstance()
		{
			if (nullptr != m_pInstance)
			{
				delete m_pInstance;
				m_pInstance = nullptr;
			}
		}
	private:
		static SingleCamera* m_pInstance;

	private:
		explicit SingleCamera();
		virtual ~SingleCamera();

	public:			// 내가 설정해 줄 부분
		void Init_Camera(LPDIRECT3DDEVICE9 _pGraphicDevice);
		void SetPlayerTransform(Transform* _pPlayerTransform);
		void SetTimeDelta(const float& _fTimeDelta);

	public:			// 카메라 연출 중 인가
		bool IsPlaying() { return m_bPlay; }
					// 쉐이크 중 인가
		bool IsShakeing() { return m_bShake; }

	public:			// 플레이어 시점
		void PlayerViewpoint();

	public:
		void PlayCameraDirection();
		void StopCameraDirection();

	public:			// 카메라 연출
		// 한 곳을 몇 초간 비출 것인가
		void PlayCameraDirection1(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt, const float& _fSecond, const float& _fFov = 1.0471976f);
		void PlayCameraDirectionR1(const D3DXVECTOR3& _vEye, const float& _fRadianX, const float& _fRadianY, const float& _fSecond, const float& _fFov = 1.0471976f);

		// At을 고정하고 움직이면서 연출, 속도는 시간으로 계산됨
		void PlayCameraDirection2(const D3DXVECTOR3& _vEyeBegin, const D3DXVECTOR3& _vEyeEnd, const D3DXVECTOR3& _vAt, const float& _fSecond, const float& _fFov = 1.0471976f);

		// 보는 방향을 고정시키고 움직이면서 연출, 속도는 시간으로 계산됨
		void PlayCameraDirection3(const D3DXVECTOR3& _vEyeBegin, const D3DXVECTOR3& _vEyeEnd, const D3DXVECTOR3& _vDir, const float& _fSecond, const float& _fFov = 1.0471976f);
		void PlayCameraDirectionR3(const D3DXVECTOR3& _vEyeBegin, const D3DXVECTOR3& _vEyeEnd, const float& _fRadianX, const float& _fRadianY, const float& _fSecond, const float& _fFov = 1.0471976f);

		// 한 위치에서 회전하면서 연출, 속도는 시간으로 계산됨	( 아직 만드는 중임 )
		void PlayCameraDirection4(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt, bool _bLeftRot, const float& _fRadian, const float& _fSecond, const float& _fFov = 1.0471976f);
		void PlayCameraDirectionR4(const D3DXVECTOR3& _vEye, const float& _fRadianX, const float& _fRadianY, bool _bLeftRot, const float& _fRadian, const float& _fSecond, const float& _fFov = 1.0471976f);

		// 카메라 쉐이크 기능
		void PlayCameraShake(const float& _fSecond, const float& _fShakePower);

	private:
		void DefaultSetting();
		void PlayDirection1();
		void PlayDirection2();
		void PlayDirection3();
		void PlayDirection4();
		void PlayShake(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt);

	private:
		LPDIRECT3DDEVICE9 m_pGraphicDevice{ nullptr };
		Transform* m_pPlayerTransform{ nullptr };

		D3DXMATRIX m_matView, m_matProj;
		D3DXVECTOR3	m_vEye{ 0.0f,0.0f,0.0f }, m_vAt{ 0.0f,0.0f,0.0f }, m_vUp{ 0.0f,0.0f,0.0f };
		float m_fFov{ D3DX_PI / 3.f }, m_fAspect{ 4.0f / 3.0f }, m_fNear{ 0.1f }, m_fFar{ 1000.0f };

		// 연출에 필요한 데이터들
		bool m_bPlay{ false }, m_bLeft{ false }, m_bShake{ false };
		int m_iPlayNum{ 0 };
		float m_fTimeD{ 0.0f }, m_fTimeS{ 0.0f }, m_fDeltaTime{ 0.0f }, m_fCurRadian{ 0.0f }, m_fRotSpeed{ 0.0f }, m_fRadian{ 0.0f }, m_fShakePower{ 0.0f };

		D3DXVECTOR3 m_vLookDir{ 0.0f,0.0f,0.0f }, m_vMoveDir{ 0.0f,0.0f,0.0f };

	private:
		virtual void Free();
	};


}
