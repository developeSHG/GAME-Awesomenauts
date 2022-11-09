#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual TEX_INFO* GetTexInfo(
		const wstring & wstrStateKey = L"", 
		const int & iIndex = 0) override;

public:
	virtual HRESULT LoadTexture(
		const wstring & wstrFilePath, 
		const wstring & wstrStateKey = L"", 
		const int & iCount = 0) override;
	virtual void Release() override;

private:
	typedef vector<TEX_INFO*>					VEC_TEXINFO;
	typedef map<const wstring, VEC_TEXINFO>		MAP_MULTITEX;
	MAP_MULTITEX	m_mapMultiTex;
};

