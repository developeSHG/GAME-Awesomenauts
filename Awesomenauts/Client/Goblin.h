#pragma once
#include "Minion.h"
class CGoblin :
	public CMinion
{
private:
	explicit CGoblin();

public:
	virtual ~CGoblin();

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
	static CGoblin* Create(TEAM_TYPE _eType, MINION_AI_TYPE _eAiType);
};

