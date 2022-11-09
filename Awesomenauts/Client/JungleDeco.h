#pragma once
#include "GameObject.h"
class CJungleDeco :
	public CGameObject
{
private:
	explicit CJungleDeco();

public:
	virtual ~CJungleDeco();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CJungleDeco* Create();

private:
	TEX_INFO* m_pTexInfo2;
};

