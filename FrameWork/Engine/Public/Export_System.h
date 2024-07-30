#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "RandomMgr.h"

BEGIN(Engine)

// GraphicDev
inline HRESULT		Init_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, GraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End();

// TimerMgr
inline _float		Get_TimeDelta(const wstring pTimerTag);
inline void			Update_TimeDelta(const wstring pTimerTag);
inline HRESULT		Init_Timer(const wstring pTimerTag);

// FrameMgr
inline _bool			IsPermit_Call(const wstring pFrameTag, const _float& fTimeDelta);
inline HRESULT			Init_Frame(const wstring pFrameTag, const _float& fCallLimit);

// FontMgr
inline HRESULT		Init_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const wstring pFontTag,
	const wstring pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void		Render_Font(const wstring pFontTag,
	const wstring pString,
	const _vec4* pPos,
	DWORD _dwFormat,
	D3DXCOLOR Color);

// InputDev
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
inline HRESULT	Init_InputDev(HINSTANCE hInst, HWND hWnd);
inline void		Update_InputDev(void);

// KeyMgr
inline bool Key_Down(int _iKey);

// SoundMgr
inline HRESULT Init_SoundMgr();
inline void	LoadSoundFile(int* outFileCnt);
inline void Play_Sound(const wstring& wsSoundKey, CHANNELID eID, float fVolume);
inline void	Play_Sound_NoInterval(const wstring& wsSoundKey, CHANNELID eID, float fVolume);
inline void	Play_BGM(const wstring& wsSoundKey, float fVolume);
inline void	Stop_Sound(CHANNELID eID);
inline void	Stop_All();
inline void	Set_ChannelVolume(CHANNELID eID, float fVolume);

// RandomMgr
inline HRESULT Init_RandomMgr();
inline int    Rand_Num(int iMin, int iMax);
inline _float  Rand_Num(_float fMin, _float fMax);

inline void			Release_System(void);

#include "Export_System.inl"

END

#endif // Export_System_h__
