#pragma once
#include "Minion.h"
class CLion :
	public CMinion
{
private:
	explicit CLion();

public:
	virtual ~CLion();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual void FrameCheck() override;
	virtual void AttackStateActive() override;

public:
	static CLion* Create();
};

