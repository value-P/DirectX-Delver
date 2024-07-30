#include "Export_System.h"
HRESULT		Init_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, GraphicDev** ppGraphicDev)
{
	return GraphicDev::GetInstance()->Init_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void			Render_Begin(D3DXCOLOR Color)
{
	GraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End()
{
	GraphicDev::GetInstance()->Render_End();
}

_float		Get_TimeDelta(const wstring pTimerTag)
{
	return TimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
void		Update_TimeDelta(const wstring pTimerTag)
{
	TimerMgr::GetInstance()->Update_TimeDelta(pTimerTag);
}
HRESULT		Init_Timer(const wstring pTimerTag)
{
	return TimerMgr::GetInstance()->Init_Timer(pTimerTag);
}

// FrameMgr
_bool			IsPermit_Call(const wstring pFrameTag, const _float& fTimeDelta)
{
	return FrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT			Init_Frame(const wstring pFrameTag, const _float& fCallLimit)
{
	return FrameMgr::GetInstance()->Init_Frame(pFrameTag, fCallLimit);
}

// FontMgr
HRESULT		Init_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const wstring pFontTag,
	const wstring pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return FontMgr::GetInstance()->Init_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void		Render_Font(const wstring pFontTag,	const wstring pString,	const _vec4* pPos, DWORD _dwFormat,	D3DXCOLOR Color)
{
	FontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, _dwFormat, Color);
}

bool Key_Down(int _iKey)
{
	return KeyMgr::GetInstance()->Key_Down(_iKey);
}

inline HRESULT Init_SoundMgr()
{
	return SoundMgr::GetInstance()->Init_SoundMgr();
}

inline void LoadSoundFile(int* outFileCnt)
{
	SoundMgr::GetInstance()->LoadSoundFile(outFileCnt);
}

void Play_Sound(const wstring& wsSoundKey, CHANNELID eID, float fVolume)
{
	SoundMgr::GetInstance()->Play_Sound(wsSoundKey,eID,fVolume);
}

void Play_Sound_NoInterval(const wstring& wsSoundKey, CHANNELID eID, float fVolume)
{
	SoundMgr::GetInstance()->Play_Sound_NoInterval(wsSoundKey, eID, fVolume);
}

void Play_BGM(const wstring& wsSoundKey, float fVolume)
{
	SoundMgr::GetInstance()->Play_BGM(wsSoundKey, fVolume);
}

void Stop_Sound(CHANNELID eID)
{
	SoundMgr::GetInstance()->Stop_Sound(eID);
}

void Stop_All()
{
	SoundMgr::GetInstance()->Stop_All();
}

inline void Set_ChannelVolume(CHANNELID eID, float fVolume)
{
	SoundMgr::GetInstance()->Set_ChannelVolume(eID, fVolume);
}

inline HRESULT Init_RandomMgr()
{
	return RandomMgr::GetInstance()->Init_RandomMgr();
}

inline int Rand_Num(int iMin, int iMax)
{
	return RandomMgr::GetInstance()->Rand_Num(iMin,iMax);
}

inline _float Rand_Num(_float fMin, _float fMax)
{
	return RandomMgr::GetInstance()->Rand_Num(fMin, fMax);
}

_byte	Get_DIKeyState(_ubyte byKeyID)
{
	return InputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return InputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return InputDev::GetInstance()->Get_DIMouseMove(eMouseState);

}
HRESULT	Init_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return InputDev::GetInstance()->Init_InputDev(hInst, hWnd);
}
void		Update_InputDev(void)
{
	InputDev::GetInstance()->Update_InputDev();
}

void			Release_System(void)
{
	RandomMgr::GetInstance()->DestroyInstance();
	SoundMgr::GetInstance()->DestroyInstance();
	InputDev::GetInstance()->DestroyInstance();
	FontMgr::GetInstance()->DestroyInstance();
	KeyMgr::GetInstance()->DestroyInstance();
	FrameMgr::GetInstance()->DestroyInstance();
	TimerMgr::GetInstance()->DestroyInstance();
	GraphicDev::GetInstance()->DestroyInstance();
}