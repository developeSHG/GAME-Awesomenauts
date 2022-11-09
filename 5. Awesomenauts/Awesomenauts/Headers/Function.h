#pragma once

#ifndef __FUNCTION_H__

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
DWORD SafeRelease(T& com)
{
	DWORD refcnt = com->Release();

	if (0 == refcnt)
		com = nullptr;

	return refcnt;
}

class CMyStrCmp
{
public:
	CMyStrCmp() {}
	CMyStrCmp(const TCHAR* pString)
		: m_pString(pString)
	{}
	~CMyStrCmp() {}

public:
	template<typename T>
	bool operator()(T& rObj)
	{
		return !lstrcmp(rObj.first, m_pString);
	}

private:
	const TCHAR* m_pString;
};


#define __FUNCTION_H__
#endif