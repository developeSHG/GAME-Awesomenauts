#pragma once
#include "GameObject.h"
#include "Animation.h"

class CBluePortal :
	public CGameObject, public CAnimation
{
private:
	explicit CBluePortal();

public:
	virtual ~CBluePortal();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBluePortal* Create();
};

