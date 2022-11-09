#pragma once
#include "Unit.h"
class CHero 
	: public CUnit
{
public:
	enum LIGHT_TYPE
	{
		RED, BLUE, CENTER, TYPE_END
	};
	typedef struct tagHeroAbility
	{
		int iDead = 0;
		int iKillHero = 0;
		int iKillMinion = 0;
	}HERO_ABILITY;

protected:
	explicit CHero();

public:
	virtual ~CHero();

public:
	virtual void HpRender(float _fx = 0.f, float _fy = 0.f, bool _bScroll = true);
	virtual void CollMap();
	virtual void IsColl();

public:
	// Get
	const LIGHT_TYPE& Get_LightType() const { return m_eLightType; }
	const bool& Get_MoveTileDownActive() const { return m_bMoveTileDownActive; }
	const SKILL_COOLTIME& Get_SkillCoolTime(SKILL_TYPE _e) const { return m_tCoolTime[_e]; }
	const HERO_ABILITY& Get_Ability() { return m_tAbility; }

public:
	// Set
	void Set_LightIn() { if (false == m_bLightIn)m_bLightIn = true; }
	void Set_LightType(const LIGHT_TYPE _eType) { m_eLightType = _eType; }
	void Set_IsCollMoveTile(const bool& _b) { m_bIsCollMoveTile = _b; }
	void Set_SkillCoolTimeActive(const SKILL_TYPE _e) { m_tCoolTime[_e].fCurrentTime = 0.f; }
	void Set_AbilityDead() { m_tAbility.iDead += 1; }
	void Set_AbilityKillHero() { m_tAbility.iKillHero += 1; }
	void Set_AbilityKillMinion() { m_tAbility.iKillMinion+= 1; }

public:
	void CollTile();
	void LightIn();
	void DeadRespawn(bool _bScroll = true);

protected:
	bool m_bTileDownActive;					// 밑으로 갈수있는 타일에 S키 활성화
	TILE* m_tTileDown;						// 밑으로 가는 타일
	bool m_bLightIn;
	LIGHT_TYPE m_eLightType;
	bool m_bIsCollMoveTile;
	bool m_bMoveTileDownActive;				// 밑으로 갈수있는 무브타일에 S키 활성화
	bool m_bCollUpTileCheck;				// 기본타일과 같이 위에있을 때 밑으로 갈수있는 타일만 체크해서 S키 가능하게 하는 데이터
	bool m_bTowerHp;
	bool m_bGameStart;
	bool m_bHealGo;
	bool m_bLevelUp;
	SKILL_COOLTIME m_tCoolTime[SKILL_TYPE::SKILL_END];
	float m_fDeadCoolTime;
	HERO_ABILITY m_tAbility;
};

