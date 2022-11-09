#pragma once
#include "GameObject.h"
class CHurt :
	public CGameObject
{
private:
	explicit CHurt();

public:
	virtual ~CHurt();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// Set
	void Set_HurtActive() { m_bHurtActive = true; m_iAlpha = 255;}
	void Set_SkillActive() { m_bSkillActive = true; m_iSkillAlpha = 255; CSoundMgr::GetInstance()->MyPlaySound(L"Summons.wav", CSoundMgr::CHANNELID::EFFECT);}
	void Set_HealActive(bool _b) { m_bHealActive = _b;}
	void Set_LevelUpActive() { m_bLevelUpActive = true; m_iLevelUpAlpha = 255; m_iLevelUpTextAlpha = 255; }
	void Set_TextActive() { m_bTextActive = true; m_iTextAlpha = 255; CSoundMgr::GetInstance()->MyPlaySound(L"Summons.wav", CSoundMgr::CHANNELID::EFFECT);}
	void Set_SkillTextActive(SKILL_TYPE _e) { m_bSkillTextActive = true; m_iSkillTextAlpha = 255; m_fSkillTextCount = 0.f; m_eSkillType = _e; }
	void Set_DeadCoolTimeActive() { if (!m_bDeadCoolTime) { m_bDeadCoolTime = true; m_iDeadCoolTimeFrame = 0; } }
	void Set_TowerPosTextActive(int _i) { m_bTowerPos = true; m_iTowerPosTextAlpha = 255; m_fTowerPosTextCount = 0.f; m_iTowerPos = _i; }

public:
	static CHurt* Create();

public:
	bool m_bHurtActive;
	int m_iAlpha;

	bool m_bSkillActive;
	int m_iSkillAlpha;
	TEX_INFO* m_pTexInfo_SKill;

	bool m_bHealActive;
	float m_fHealAlpha;

	bool m_bLevelUpActive;
	int m_iLevelUpAlpha;
	float m_fLevelUpTextCount;
	int m_iLevelUpTextAlpha;
	TEX_INFO* m_pTexInfo_LevelUpText;

	TEX_INFO* m_pTexInfo_SkillText;

	bool m_bTextActive;
	int m_iTextAlpha;

	bool m_bSkillTextActive;
	int m_iSkillTextAlpha;
	SKILL_TYPE m_eSkillType;
	float m_fSkillTextCount;

	bool m_bDeadCoolTime;
	int m_iDeadCoolTimeFrame;
	float m_fDeadCoolTimeCount;

	bool m_bTowerPos;
	int m_iTowerPos;
	int m_iTowerPosTextAlpha;
	float m_fTowerPosTextCount;

	int m_iVictoryAlpha;
	float m_fVictoryCount;
	bool m_bVictory;
};

