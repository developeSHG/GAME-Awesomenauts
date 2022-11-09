#pragma once
class CTexture
{
public:
	explicit CTexture();

public:
	virtual ~CTexture();

public:
	virtual TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"",
		const int& iIndex = 0) PURE; 

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath,			/* 텍스처 경로 */
		const wstring& wstrStateKey = L"",		/* 상태 키값 멀티 텍스처일 때만 사용 */
		const int& iCount = 0) PURE;			/* 불러올 이미지 개수. 텍스처일 때만 사용 */
	virtual void Release() PURE;
};

