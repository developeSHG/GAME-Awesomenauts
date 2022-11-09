#include "stdafx.h"
#include "RedPortal.h"


CRedPortal::CRedPortal()
{
}

CRedPortal::~CRedPortal()
{
}

int CRedPortal::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// 컬링 시작
	if (1000.f < CScrollMgr::GetScrollPos().x && 2700.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	if (false == m_bActive)
	{
		if (1950.f < CScrollMgr::GetScrollPos().x)
		{
			m_bActive = true;
		}
	}
	else
	{
		if (false == m_bOpen)
		{
			m_tFrame.fCurrentFrame += m_tFrame.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * 0.15f;

			if (m_tFrame.fCurrentFrame >= 14.f)
			{
				m_tFrame.fCurrentFrame = 14.f;
				m_bOpen = true;
			}
		}
		else
			CAnimation::FrameMove(m_tFrame.fMoveSpeed, 21.f);	// 프레임무브
	}

	return NO_EVENT;
}

void CRedPortal::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CRedPortal::Render()
{
	if (false == m_bCulling)
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Red", m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CRedPortal::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 2500, 1660, 0.f };
	m_tInfo.vSize = { -1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Portal";

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	//m_tFrame.fCurrentFrame = m_tFrame.fCurrentFrame;
	m_tFrame.fCurrentFrame = 0.f;
	m_tFrame.fMaxFrameCount = 40.f;
	m_tFrame.fMoveSpeed = 0.4f;
	m_bActive = false;
	m_bOpen = false;

	return S_OK;
}

void CRedPortal::Release()
{
}

CRedPortal * CRedPortal::Create()
{
	CRedPortal* pInstance = new CRedPortal;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
