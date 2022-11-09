#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

TEX_INFO* CTextureMgr::GetTexInfo(
	const wstring & wstrObjectKey, 
	const wstring & wstrStateKey, 
	const int & iIndex)
{
	auto iter_find = m_MapTexture.find(wstrObjectKey);

	if (m_MapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, iIndex);
}

HRESULT CTextureMgr::LoadTextureFromImgPathFile(const wstring & wstrFilePath)
{
	wifstream fin;

	fin.open(wstrFilePath);

	if (fin.fail())
	{
		ERR_MSG(wstrFilePath.c_str());
		ERR_MSG(L"불러오기 실패");

		return E_FAIL;
	}

	TCHAR szObjectKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szCount[MIN_STR] = L"";
	TCHAR szRelativePath[MAX_STR] = L"";

	HRESULT hr = 0;

	while (true)
	{
		fin.getline(szObjectKey, MAX_STR, '|');	// '|' 단위로 끊어서 읽어들임.
		fin.getline(szStateKey, MAX_STR, '|');

		if (!lstrcmpiW(szStateKey, L"S"))
		{
			fin.getline(szRelativePath, MAX_STR); // 개행 단위로 끊어서 읽어들임.
			hr = LoadTexture(SINGLE_TEXTURE, szRelativePath, szObjectKey);

			if (FAILED(hr))
			{
				fin.close();
				return E_FAIL;
			}
			continue;
		}
		fin.getline(szCount, MIN_STR, '|');
		fin.getline(szRelativePath, MAX_STR); // 개행 단위로 끊어서 읽어들임.

		if (fin.eof())
			break;

		hr = LoadTexture(MULTI_TEXTURE, szRelativePath, szObjectKey,
			szStateKey, _ttoi(szCount));

		if (FAILED(hr))
		{
			fin.close();
			return E_FAIL;
		}
	}

	fin.close();

	return S_OK;
}

HRESULT CTextureMgr::LoadTexture(
	TEXTURE_TYPE eTextureType,
	const wstring& wstrFilePath, 
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	const int& iCount)
{
	CTexture* pTexture = nullptr;

	auto iter_find = m_MapTexture.find(wstrObjectKey);

	switch (eTextureType)
	{
	case SINGLE_TEXTURE:
		if (m_MapTexture.end() != iter_find)
			return E_FAIL;

		pTexture = new CSingleTexture;
		m_MapTexture[wstrObjectKey] = pTexture;
		break;
	case MULTI_TEXTURE:
		if (m_MapTexture.end() == iter_find)
		{
			pTexture = new CMultiTexture;
			m_MapTexture[wstrObjectKey] = pTexture;
		}
		break;
	}

	HRESULT hr = m_MapTexture[wstrObjectKey]->LoadTexture(
		wstrFilePath, wstrStateKey, iCount);
	FAILED_CHECK_MSG_RETURN(hr, L"LoadTexture Failed", E_FAIL);	

	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& MyPair : m_MapTexture)
		SafeDelete(MyPair.second);

	m_MapTexture.clear();
}
