#include "stdafx.h"
#include "ScrollMgr.h"

//D3DXVECTOR3 CScrollMgr::m_vScroll = { 0.f, 0.f ,0.f };
D3DXVECTOR3 CScrollMgr::m_vScroll = { 0.f, -SPACE_Y + SPACE_START_Y ,0.f};
bool CScrollMgr::m_bScrollLock = false;
bool CScrollMgr::m_bLakeRender = false;
//bool CScrollMgr::m_bSpaceScroll = false;
bool CScrollMgr::m_bSpaceScroll = true;

CScrollMgr::CScrollMgr()
{
}


CScrollMgr::~CScrollMgr()
{
}

const D3DXVECTOR3& CScrollMgr::GetScrollPos()
{
	return m_vScroll;
}

void CScrollMgr::SetScrollPos(const D3DXVECTOR3& vMove)
{
	m_vScroll += vMove;
}

void CScrollMgr::SetScrollTaxiPos()
{
	m_vScroll = { 102.f, 904.f, 0.f };
}

void CScrollMgr::SetScrollRespawnPos()
{
	m_vScroll = { 1221.f, 999.f, 0.f };
}

void CScrollMgr::ScrollLock()
{
	if (true == m_bSpaceScroll)
		return;

	if (m_vScroll.x <= 4)
	{
		m_vScroll.x = 4;
		m_bScrollLock = true;
	}
	else if (8510.f <= m_vScroll.x)
	{
		m_vScroll.x = 8510.f;
		m_bScrollLock = true;
	}
	else
		m_bScrollLock = false;


	//if (CKeyMgr::GetInstance()->KeyPressing(KEY_D) && true == m_bScrollLock && 4 == m_vScroll.x)
	//	m_bScrollLock = false;

	//if (m_vScroll.y < 0)
	//	m_vScroll.y = 0;

	//if (WINCX - TILECX * TILEX > m_iScrollX)
	//	m_iScrollX = WINCX - TILECX * TILEX;
	//if (WINCY - TILECY * TILEY > m_iScrollY)
	//	m_iScrollY = WINCY - TILECY * TILEY;
	// -1110, -1315

	if (2670.f < m_vScroll.x && 5980.f > m_vScroll.x
		&& 1300.f < m_vScroll.y)
		m_bLakeRender = true;
	else
		m_bLakeRender = false;
}
