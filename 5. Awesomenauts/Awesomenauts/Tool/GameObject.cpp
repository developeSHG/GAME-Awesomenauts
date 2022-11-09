#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_bIsInit(FALSE),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_wstrObjectKey = L"";
	m_pTexInfo = nullptr;
	ZeroMemory(&m_tRectInfo, sizeof(RECT_INFO));
	m_fSpeed = 0.f;
	m_bAfterSpeed = false;
}

CGameObject::~CGameObject()
{
}

void CGameObject::MiniRender()
{
}

HRESULT CGameObject::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}

	return S_OK;
}

void CGameObject::UpdateRect(DIR _e)
{
	switch (_e)
	{
	case DIR::RIGHT:
		m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x);
		m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy));
		m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx));
		m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y);
		break;
	case DIR::LEFT:
		m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x);
		m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy));
		m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx));
		m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y);
		break;
	}
}

//bool CGameObject::AfterSpeed(DIR _e, bool _b /* = false */)
//{
//	static float fAfterSpeed = m_fSpeed;
//
//	if (true == _b)
//	{
//		fAfterSpeed = m_fSpeed;
//		return false;
//	}
//
//	fAfterSpeed -= m_pTimeMgr->GetDeltaTime() * 800.f;
//	switch (_e)
//	{
//	case DIR::LEFT:
//		m_tInfo.vPos.x -= m_pTimeMgr->GetDeltaTime() * fAfterSpeed;
//		break;
//	case DIR::RIGHT:
//		m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * fAfterSpeed;
//		break;
//	}
//
//	if (0.f >= fAfterSpeed)
//	{
//		fAfterSpeed = m_fSpeed;
//		return false;
//	}
//	return true;
//}
