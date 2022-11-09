#include "stdafx.h"
#include "JunglePortal.h"


CJunglePortal::CJunglePortal()
{
}


CJunglePortal::~CJunglePortal()
{
}

int CJunglePortal::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// 컬링 시작
	if (3400.f < CScrollMgr::GetScrollPos().x && 5300.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	CAnimation::FrameMove(m_tFrame.fMoveSpeed);	// 프레임무브

	return NO_EVENT;
}

void CJunglePortal::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CJunglePortal::Render()
{
	if (false == m_bCulling)
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Jungle", m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CJunglePortal::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	//m_tInfo.vPos = { 4945.f, 1241.f, 0.f };
	m_tInfo.vPos = { 4950.f, 1530.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Portal";

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	m_tFrame.fCurrentFrame = 0.f;
	m_tFrame.fMaxFrameCount = 14.f;
	m_tFrame.fMoveSpeed = 1.f;

	return S_OK;
}

void CJunglePortal::Release()
{
}

CJunglePortal * CJunglePortal::Create()
{
	CJunglePortal* pInstance = new CJunglePortal;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}