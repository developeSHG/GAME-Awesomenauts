#pragma once
#include "GameObject.h"
#include "Animation.h"

class CRedPortal :
	public CGameObject, public CAnimation
{
private:
	explicit CRedPortal();

public:
	virtual ~CRedPortal();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CRedPortal* Create();

private:
	bool m_bActive;
	bool m_bOpen;
};

