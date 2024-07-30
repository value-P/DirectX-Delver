#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	// ���ø��� ����� ������������ �ڷ����� ���������� ���� ��
	// ����� �ν��Ͻ�ȭ �ϱ� ���Ͽ� �����δ� Ʋ

	template<typename T>
	void	Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete [] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = NULL;
		}

		return dwRefCnt;
	}


	// Functor
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag) : m_pTargetTag(pTag){}
		~CTag_Finder(void) {}

	public:
		template<typename T> 
		_bool		operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
				return true;
			
			return false;
		}

	private:
		const _tchar* m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Safe_Release(pInstance);

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// ���������̳� ������
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};

}

#endif // Engine_Function_h__

inline unsigned long FtoDW(float _f)
{
	return *(unsigned long*)&_f;
}

inline float GetRandomFloat(const float& _fLowBound, const float& _fHighBound)
{
	if (_fLowBound >= _fHighBound)
	{
		return _fLowBound;
	}

	float f = (rand() % 10000) * 0.0001f;

	return (f * (_fHighBound - _fLowBound)) + _fLowBound;
}

inline void GetRandomVector(D3DXVECTOR3* _pOut, D3DXVECTOR3* _pMin, D3DXVECTOR3* _pMax)
{
	_pOut->x = GetRandomFloat(_pMin->x, _pMax->x);
	_pOut->y = GetRandomFloat(_pMin->y, _pMax->y);
	_pOut->z = GetRandomFloat(_pMin->z, _pMax->z);
}