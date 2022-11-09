#pragma once
#include "Minion.h"
class CStormBreaker :
	public CMinion
{
private:
	explicit CStormBreaker();

public:
	virtual ~CStormBreaker();

	// CGameObject��(��) ���� ��ӵ�
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
	static CStormBreaker* Create(TEAM_TYPE _eType, MINION_AI_TYPE _eAiType);
};

