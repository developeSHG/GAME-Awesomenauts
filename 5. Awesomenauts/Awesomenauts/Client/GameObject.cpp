#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_bIsInit(FALSE),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance())
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_wstrObjectKey = L"";
	m_pTexInfo = nullptr;
	ZeroMemory(&m_tRectInfo, sizeof(RECT_INFO));
	m_fSpeed = 0.f;
	m_bCulling = false;
}

CGameObject::~CGameObject()
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

void CGameObject::UpdateMyRect(RECT & _rcRect, float _fcx, float _fcy)
{
	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	_rcRect.left = LONG(m_tInfo.vPos.x - _fcx / 2.f - fScrollX);
	_rcRect.top = LONG(m_tInfo.vPos.y - _fcy / 2.f - fScrollY);
	_rcRect.right = LONG(m_tInfo.vPos.x + _fcx / 2.f - fScrollX);
	_rcRect.bottom = LONG(m_tInfo.vPos.y + _fcy / 2.f - fScrollY);
}
