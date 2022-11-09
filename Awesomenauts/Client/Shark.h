#pragma once
#include "Minion.h"
class CShark :
	public CMinion
{
private:
	explicit CShark();

public:
	virtual ~CShark();

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
	HRESULT CreateElement();

public:
	static CShark* Create(TEAM_TYPE _eType, MINION_AI_TYPE _eAiType);
};

