#include "AnimationTexture.h"

Engine::AnimationTexture::AnimationTexture(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Texture(_pGraphicDevice)
{
}

Engine::AnimationTexture::AnimationTexture(const AnimationTexture& _rhs)
	: Texture(dynamic_cast<const Texture&>(_rhs)), m_iMaxNum(_rhs.m_iMaxNum), m_fFrameSpeed(_rhs.m_fFrameSpeed), m_pPath(_rhs.m_pPath)
{
}

Engine::AnimationTexture::~AnimationTexture()
{
}

AnimationTexture* Engine::AnimationTexture::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const wstring _pPath, const _uint& _iCnt, const float& _fFrameSpeed)
{
	AnimationTexture* pInstance = new AnimationTexture(_pGraphicDevice);

	if (FAILED(pInstance->Init_AnimationTexture(_pPath, _iCnt, _fFrameSpeed)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_AnimationTexture Failed", L"AnimationTexture::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

Component* Engine::AnimationTexture::Clone()
{
	return new AnimationTexture(*this);
}

HRESULT Engine::AnimationTexture::Init_AnimationTexture(const wstring _pPath, const _uint& _iCnt, const float& _fFrameSpeed)
{
	m_vecTexture.reserve(_iCnt);

	m_iMaxNum = _iCnt;
	m_fFrameSpeed = _fFrameSpeed;
	m_pPath = _pPath;
	IDirect3DBaseTexture9* pTexture = nullptr;

	for (_uint i = 0; i < _iCnt; i++)
	{
		TCHAR szFileName[128] = L"";
		wsprintf(szFileName, _pPath.c_str(), i);
		if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
		{
			return E_FAIL;
		}
		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

int Engine::AnimationTexture::Update_Component(const _float& _fTimeDelta)
{
	m_fFrame += m_fFrameSpeed * _fTimeDelta;

	if ((float)m_iMaxNum <= m_fFrame)
	{
		memset(&m_fFrame, 0, 4);
		m_iRepeatCnt++;
	}

	return 0;
}

IDirect3DBaseTexture9* Engine::AnimationTexture::Get_Texture()
{
	return m_vecTexture[(UINT)m_fFrame];
}

IDirect3DBaseTexture9* Engine::AnimationTexture::Get_Texture(const UINT& _iIndex)
{
	if (m_vecTexture.size() <= _iIndex)
		return nullptr;

	return m_vecTexture[_iIndex];
}

void Engine::AnimationTexture::SetFrame(const float& _fFrame)
{
	memcpy_s(&m_fFrame, sizeof(float), &_fFrame, sizeof(float));
}

void Engine::AnimationTexture::SetFrameSpeed(const float& _fFrameSpeed)
{
	m_fFrameSpeed = _fFrameSpeed;
}

void Engine::AnimationTexture::Set_Texture()
{
	m_pGraphicDev->SetTexture(0, m_vecTexture[(UINT)m_fFrame]);
}

void Engine::AnimationTexture::Set_Texture(const UINT& _iIndex)
{
	if (m_vecTexture.size() <= _iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[_iIndex]);
}

void Engine::AnimationTexture::Free()
{
	Texture::Free();
}

wstring Engine::AnimationTexture::GeT_CurTextureState()
{
	size_t Underbar_First = m_pPath.find_last_of(L'_');
	size_t Underbar_Second = m_pPath.substr(0, Underbar_First).find_last_of(L'/');
	wstring m_pCurTextureState = m_pPath.substr(Underbar_Second + 1, Underbar_First - Underbar_Second - 1);

	m_pCurTextureState = m_pCurTextureState + L"_" + to_wstring(_uint(m_fFrame));

	return m_pCurTextureState;
}