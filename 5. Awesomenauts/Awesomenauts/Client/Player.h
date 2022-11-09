#pragma once

#include "GameObject.h"
#include "Hero.h"

class CPlayer : public CHero
{
private:
	explicit CPlayer();

public:
	virtual ~CPlayer();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	virtual void LakeRender() override;
	virtual void AttackStateActive() override;
	virtual void FrameCheck() override;
	virtual void DirCheck();
	virtual void CollMap();
	virtual void IsColl();

public:
	// Get
	const bool& Get_AfterSpeed() const { return m_bAfterSpeed; }

public:
	// Set
	void Set_ScrollActiveX(bool _b) { m_bScrollActiveX = _b; }

public:
	void KeyInput();
	void IsOffset();
	bool AfterSpeed(DIR _e, bool _b = false);
	void SkillCoolTimeCheck();
	void TaxiStateRender();

public:
	static CPlayer* Create(/*D3DXVECTOR3 _vPos = { WINCX / 2 - 45.f, 480.f, 0.f }*/);

private:
	bool m_bScrollActiveX;
	bool m_bJumpScrollActiveY;
	bool m_bOffsetScrollActiveY;
	bool m_bLeftScrollActive;
	bool m_bRightScrollActive;
	bool m_bAfterSpeed;
	FRAME m_tFrameTaxi;
	INFO m_tInfoTaxi;
	bool m_bTaxiActive;
	bool m_bTaxiOK;
	float m_fChietAttack;
};

