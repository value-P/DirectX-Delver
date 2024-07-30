#pragma once
#include "Engine_Define.h"

namespace Engine
{
	class Texture;
}

struct TEXDAT
{
	std::string texName;				// �ؽ�ó ��ǥ�̸�
	std::wstring texPath;				// ���� �ؽ�ó ���
	Engine::Texture* pTex = nullptr;	// ���� �ؽ�ó(�����ֱ��)
};

struct COLLIDERDAT
{
	Engine::_bool bHasCollider;
	Engine::COLLAYER eColLayer;
	Engine::COMPONENTID eComponentID;
};

