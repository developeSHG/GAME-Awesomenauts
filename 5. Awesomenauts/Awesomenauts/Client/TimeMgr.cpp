#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	: m_fDeltaTime(0.f)
{
}


CTimeMgr::~CTimeMgr()
{
}

float CTimeMgr::GetDeltaTime() const
{
	return m_fDeltaTime;
}

void CTimeMgr::InitTime()
{
	// 메인보드(하드웨어)가 갖고 있는 고해상도 타이머 값을 얻어오는 함수.
	// 이 타이머 값은 CPU의 진동수에 맞춰 진행된다. 타이머 값 = CPU 진동 누적수
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// CPU의 초당 진동수를 얻어오는 함수.
	QueryPerformanceFrequency(&m_CpuTick);
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	if(m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuTick.QuadPart)
		QueryPerformanceFrequency(&m_CpuTick);

	m_fDeltaTime = 
		float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;

	m_OldTime.QuadPart = m_CurTime.QuadPart;
}
