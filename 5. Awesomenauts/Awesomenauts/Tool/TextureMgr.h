#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	enum TEXTURE_TYPE { SINGLE_TEXTURE, MULTI_TEXTURE };

private:
	CTextureMgr();
	~CTextureMgr();

public:
	TEX_INFO* GetTexInfo(
		const wstring& wstrObjectKey,
		const wstring & wstrStateKey = L"",
		const int & iIndex = 0);

public:
	HRESULT LoadTextureFromImgPathFile(const wstring& wstrFilePath);
	HRESULT LoadTexture(
		TEXTURE_TYPE eTextureType,
		const wstring& wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		const int& iCount = 0);
	void Release();

private:
	map<const wstring, CTexture*>	m_MapTexture;
};

