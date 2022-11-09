#pragma once
class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	~CTimeMgr();

public:
	float GetDeltaTime() const;

public:
	void InitTime();
	void UpdateTime();

private:
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_CpuTick;

	float			m_fDeltaTime;
};

