#include "stdafx.h"
#include "FrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
{
}


CFrameMgr::~CFrameMgr()
{
}

void CFrameMgr::InitTime()
{
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CPUTick);
}

bool CFrameMgr::LimitFrame(float fLimit)
{
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceCounter(&m_OldTime);
		QueryPerformanceCounter(&m_CurTime);
		QueryPerformanceFrequency(&m_CPUTick);
	}

	m_fLimitCount += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;
	m_fTimeCount += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;
	m_OldTime = m_CurTime;

	if ( 1.f / fLimit <= m_fLimitCount)
	{
		++m_iFPS_Count;
		m_fLimitCount = 0.f;
		return true;
	}	

	return false;
}

void CFrameMgr::RenderFPS()
{
	if (1.f <= m_fTimeCount)
	{				
		swprintf_s(m_szBuf, L"FPS: %d", m_iFPS_Count);
		m_fTimeCount = 0.f;
		m_iFPS_Count = 0;
	}

#ifdef _DEBUG
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 600.f, 100.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 0, 255, 0));
	CDeviceMgr::GetInstance()->GetSprite()->End();
#endif
}
