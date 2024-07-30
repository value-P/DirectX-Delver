#include "SoundMgr.h"

IMPLEMENT_SINGLETON(SoundMgr)

Engine::SoundMgr::SoundMgr()
{
}

Engine::SoundMgr::~SoundMgr()
{
	Free();
}

HRESULT Engine::SoundMgr::Init_SoundMgr()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	// 초반 필요한 몇개의 사운드만 찾아서 넣도록
	LoadLoadingSoundFile();

	return S_OK;
}

void Engine::SoundMgr::Play_Sound(const wstring& wsSoundKey, CHANNELID eID, float fVolume)
{
	auto iter = m_mapSound.find(wsSoundKey);

	if (iter == m_mapSound.end()) return;

	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, false, &m_pChannelArr[eID]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void Engine::SoundMgr::Play_Sound_NoInterval(const wstring& wsSoundKey, CHANNELID eID, float fVolume)
{
	auto iter = m_mapSound.find(wsSoundKey);

	if (iter == m_mapSound.end()) return;

	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void Engine::SoundMgr::Play_BGM(const wstring& wsSoundKey, float fVolume)
{
	auto iter = m_mapSound.find(wsSoundKey);

	if (iter == m_mapSound.end()) return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void Engine::SoundMgr::Stop_Sound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void Engine::SoundMgr::Stop_All()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void Engine::SoundMgr::Set_ChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void Engine::SoundMgr::LoadSoundFile(int* outFileCnt)
{
	filesystem::directory_iterator iter("../../Resource/Sound/AllSound");

	FMOD_SOUND* pSound = nullptr;
	string filePath;
	wstring soundKey;

	*outFileCnt = int(iter->file_size());

	while (iter != filesystem::end(iter))
	{
		if ((*iter).is_regular_file())
		{
			filePath = (*iter).path().string();
			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, filePath.c_str(), FMOD_HARDWARE, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				// 사운드 파일의 확장자 명을 뗀 이름을 키값으로 사용
				soundKey = (*iter).path().filename().stem().wstring();
				m_mapSound.emplace(soundKey, pSound);
			}
		}

		++iter;
	}

}

void Engine::SoundMgr::LoadLoadingSoundFile()
{
	filesystem::directory_iterator iter("../../Resource/Sound/BgSound");

	FMOD_SOUND* pSound = nullptr;
	string filePath;
	wstring soundKey;

	while (iter != filesystem::end(iter))
	{
		if ((*iter).is_regular_file())
		{
			filePath = (*iter).path().string();
			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, filePath.c_str(), FMOD_HARDWARE, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				// 사운드 파일의 확장자 명을 뗀 이름을 키값으로 사용
				soundKey = (*iter).path().filename().stem().wstring();
				m_mapSound.emplace(soundKey, pSound);
			}
		}

		++iter;
	}
}

void Engine::SoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
