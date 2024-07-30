#pragma once
#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
	class ENGINE_DLL SoundMgr :
		public Base
	{
		DECLARE_SINGLETON(SoundMgr)

	private:
		explicit SoundMgr();
		virtual ~SoundMgr();

	public:
		HRESULT		Init_SoundMgr();

	public:
		void	Play_Sound(const wstring& wsSoundKey, CHANNELID eID, float fVolume);
		void	Play_Sound_NoInterval(const wstring& wsSoundKey, CHANNELID eID, float fVolume);
		void	Play_BGM(const wstring& wsSoundKey, float fVolume);
		void	Stop_Sound(CHANNELID eID);
		void	Stop_All();
		void	Set_ChannelVolume(CHANNELID eID, float fVolume);

	public:
		void	LoadSoundFile(int* outFileCnt);			// 나머지 모든 사운드 파일 로드

	private:
		void	LoadLoadingSoundFile();		// 초반 로딩화면에서만 쓰일 몇개의 사운드만 로드

	private:
		map<wstring, FMOD_SOUND*> m_mapSound;
		FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
		FMOD_SYSTEM* m_pSystem = nullptr;

	public:
		virtual	void Free();
	};
}

