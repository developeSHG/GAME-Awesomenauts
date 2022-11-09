#pragma once
class CAnimation
{
public:
	enum STATE
	{
		IDLE, MOVE, JUMP, JUMP_DOWN, ATTACK, SKILL1, SKILL2, SKILL3, SKILL4, DEAD, TAXI, END
	};

protected:
	CAnimation();

public:
	virtual ~CAnimation();

protected:
	virtual void FrameCheck() {}
	virtual void DirCheck() {}


public:
	// Get
	const STATE& Get_State() const { return m_eCurrentState; }
	const bool& Get_boolState(const STATE _eState) const { return m_bState[_eState]; }
	const float& Get_CurrentFrame() const { return m_tFrame.fCurrentFrame; }

public:
	// Set
	void Set_FrameEnd(float _fMaxFrameCount) { m_tFrame.fMaxFrameCount = _fMaxFrameCount; }
	void Set_bStateActive(STATE _eState) { m_bState[_eState] = true; }

protected:
	void FrameMove(const float _fSpeed = 1.f, const float _fFrameStart = 0.f);
	void FrameMoveDead(const float _fSpeed = 1.f);
	void Active_AttackState(const STATE _e) { m_bState[STATE::ATTACK] = false; m_bState[STATE::SKILL1] = false; 
	m_bState[STATE::SKILL2] = false; m_bState[STATE::SKILL3] = false; m_bState[STATE::SKILL4] = false; m_bState[_e] = true; }
	void InGravity(INFO& _tInfo, RECT_INFO& _tRectInfo);

private:
	CTimeMgr*	m_pTimeMgrAni;

protected:
	FRAME	m_tFrame;
	wstring m_wstrStateKey[STATE::END];		// State 별 애니메이션 키값
	STATE m_eCurrentState;
	STATE m_eNextState;

protected:
	IMAGE_SINK m_tImageSink;				// 애니메이션 별 Pos값 맞추기 위해 구조체
	bool m_bState[STATE::END];				// State에 맞는 조건 체크
};

