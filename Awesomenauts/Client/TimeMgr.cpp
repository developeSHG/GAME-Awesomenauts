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
	// ���κ���(�ϵ����)�� ���� �ִ� ���ػ� Ÿ�̸� ���� ������ �Լ�.
	// �� Ÿ�̸� ���� CPU�� �������� ���� ����ȴ�. Ÿ�̸� �� = CPU ���� ������
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// CPU�� �ʴ� �������� ������ �Լ�.
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
