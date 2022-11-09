#pragma once
#include "Unit.h"
class CMinion
	: public CUnit
{
protected:
	explicit CMinion();

public:
	virtual ~CMinion();

public:
	virtual void HpRender(float _fx = 0.f, float _fy = 0.f);
	virtual void CollMap();
	virtual void IsColl();

public:
	void CreateUpdate();
	void CreateRender();
	void AISystem();
	void RedAI();
	void BlueAI();
	void JungleAI();

public:
	// Set
	void Set_AiType(MINION_AI_TYPE _eAiType) { m_eAiType = _eAiType; }

protected:
	MINION_AI_TYPE m_eAiType;
	bool m_bCreate;
	bool m_bCreateAni;
	FRAME m_tCrateFrame;
	list<RECT> m_listJumpRect;
	float m_fBornSizeX;
	float m_fJungleDirCount;
	float m_fJungleIdleCount;
	bool m_bJungleIdle;
};

