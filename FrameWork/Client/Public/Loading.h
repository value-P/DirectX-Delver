#pragma once
#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
	class Scene;
}

class CLoading : public Base
{
public:
	enum LOADINGID {	LOADING_STAGE, LOADING_VILLEAGE,
						LOADING_STAGE1,LOADING_STAGE2,LOADING_STAGE3,
						LOADING_STAGE4, LOADING_BOSS, LOADING_END };
 
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	_bool					Get_Finish() const		{ return m_bFinish; }
	CRITICAL_SECTION*		Get_Crt()				{ return &m_Crt; }
	LOADINGID				Get_LoadingID() const	{ return m_eLoading; }
	const wstring			Get_String() const		{ return m_szString; }

public:
	HRESULT				Init_Loading(LOADINGID eLoading);
	_uint				Loading_ForStage();
	_uint				Loading_ForStage(LOADINGID eLoading);

public:
	static unsigned int CALLBACK		Thread_Main(void* pArg);

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	LOADINGID			m_eLoading;
	_tchar				m_szString[128];

	Scene*				m_pCurScene = nullptr;
public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading, Scene* pCurScene = nullptr);

private:
	virtual void			Free();
};

