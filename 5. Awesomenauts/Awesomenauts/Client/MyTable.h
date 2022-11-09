#pragma once
#include "GameObject.h"
class CMyTable :
	public CGameObject
{
private:
	explicit CMyTable();

public:
	virtual ~CMyTable();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	void RedHeroRender();
	void BlueHeroRender();

public:
	static CMyTable* Create();

private:
	bool m_bActive;
	TCHAR	m_szBuf[MIN_STR] = L"";
	float	m_fTimeCount = 0.f;
	float	m_fLimitCount = 0.f;
	int		m_iFPS_Count = 0;
};

