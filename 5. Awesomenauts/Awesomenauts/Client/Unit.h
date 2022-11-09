#pragma once
#include "GameObject.h"
#include "Animation.h"

class CUnit :
	public CGameObject, public CAnimation
{
public:
	enum HURT_RGB
	{
		R, G, B, RGB_END
	};

protected:
	explicit CUnit();

public:
	virtual ~CUnit();

public:
	virtual void HpRender(float _fx = 0.f, float _fy = 0.f){}
	virtual void AISystem(){}
	virtual void CollMap(){}
	virtual void IsColl(){}
	virtual void LakeRender();
	virtual void AttackRectActive(float _fCurrentFrame, float _fStart, float _fEnd, bool _bScroll = true);
	virtual void AttackStateActive() {}

public:
	// Get
	const JUMP_INFO& Get_JumpInfo() const { return m_tJumpInfo; }
	const TEAM_TYPE& Get_TeamType() const { return m_eTeamType; }
	const bool& Get_Hurt() const { return m_bHurt; }
	const STATE& Get_CurrentState() const { return m_eCurrentState; }
	const bool& Get_Alpha() const { return m_bAlpha; }
	const CGameObject* Get_Target() const { return m_pTarget; }
	const STATUS_INFO& Get_Status() const { return m_tStatus; }
	const float& Get_StatusAttack() const { return m_tStatus.fAttack; }
	const bool& Get_AttackRectActive() const { return m_bAttackRectActive; }
	const bool& Get_AttackOnlyTarget() const { return m_bAttackOnlyTarget; }
	const bool& Get_IsColl() const { return m_bIsColl; }
	const float &Get_JumpPower() const { return m_tJumpInfo.fPower; }

public:
	// Set
	void Set_Target(CGameObject* _pObject) { m_pTarget = _pObject; }
	void Set_JumpPower(const float _fPower) { m_tJumpInfo.fPower = _fPower; }
	void Set_IsColl(const bool& _bIs) { m_bIsColl = _bIs; }
	void Set_Dir(const DIR _eDir) { m_eDir = _eDir; }
	void Set_TeamType(const TEAM_TYPE _eType) { m_eTeamType = _eType; }
	void Set_Hurt() { m_bHurt = true; m_iHurtRgb[HURT_RGB::G] = 90; m_iHurtRgb[HURT_RGB::B] = 31; }
	//void Set_Hurt() { if (false == m_bHurt) { m_iHurtRgb[HURT_RGB::G] = 40; m_iHurtRgb[HURT_RGB::B] = 40; } m_bHurt = true; }
	void Set_NextState(STATE _eState) { m_eNextState = _eState; }
	void Set_StatusHpHurt(float _f) { m_tStatus.fHp += -_f; if (0.f >= m_tStatus.fHp) { m_eNextState = STATE::DEAD; m_tStatus.fHp = 0.f; } }
	void Set_AttackOnlyTarget(bool _b) { m_bAttackOnlyTarget = _b; }
	void Set_AttackRectActive(bool _b) { m_bAttackRectActive = _b; }
	void Set_HealHp() { 
		if (m_tStatus.fMaxHp > m_tStatus.fHp)
			m_tStatus.fHp += 4.f * m_pTimeMgr->GetDeltaTime();
		else
			m_tStatus.fHp = m_tStatus.fMaxHp;

		//Set_StatusExpUp(1.f);
	}
	void Set_Medicin() { 
		if (m_tStatus.fMaxHp > m_tStatus.fHp)
			m_tStatus.fHp += 15.f; 
		if (m_tStatus.fMaxHp < m_tStatus.fHp)
			m_tStatus.fHp = m_tStatus.fMaxHp;
	}
	void Set_StatusExpUp(float _f);

public:
	void LevelUpRender(bool _bScroll = true, float _fx=0.f, float _fy = 0.f);
	void UpdateRect(bool _bScroll = true);
	void NewUpdateRect(bool _bScroll = true);
	void UpdateAttackRect(bool _bScroll = true);
	void UpdateSkillRect(bool _bScroll = true);
	void HurtActive();
	void TargetDirCheck();

protected:
	CGameObject* m_pTarget;
	D3DXVECTOR3 m_vLake;
	JUMP_INFO m_tJumpInfo;
	bool m_bIsColl;							// 타일, 땅이랑 충돌체크할 자료형
	DIR m_eDir;
	UNIT_TYPE m_eUnitType;
	TEAM_TYPE m_eTeamType;
	bool m_bHurt;
	int m_iHurtRgb[HURT_RGB::RGB_END];
	int m_iAlpha;
	int m_fHpAlpha;
	bool m_bAlpha;
	STATUS_INFO m_tStatus;
	bool m_bAttackRectActive;
	bool m_bAttackOnlyTarget;
	bool m_bLevelUp;
	FRAME m_tFrameLevelUp;
};

