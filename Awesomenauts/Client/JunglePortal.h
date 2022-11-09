#pragma once
#include "GameObject.h"
#include "Animation.h"

class CJunglePortal :
	public CGameObject, public CAnimation
{
private:
	explicit CJunglePortal();

public:
	virtual ~CJunglePortal();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CJunglePortal* Create();
};

