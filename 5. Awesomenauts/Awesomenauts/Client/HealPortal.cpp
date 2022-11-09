#include "stdafx.h"
#include "HealPortal.h"

#include "Hurt.h"
#include "Player.h"

CHealPortal::CHealPortal()
{
}


CHealPortal::~CHealPortal()
{
}

int CHealPortal::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	for (auto& pDest : m_pObjectMgr->Get_RedUnitList())
	{
		if (pDest == m_pObjectMgr->Get_Player())
		{
			float fX = CScrollMgr::GetScrollPos().x;
			float fY = CScrollMgr::GetScrollPos().y;
			if (1960.f < pDest->Get_Pos().x + fX && 2200.f > pDest->Get_Pos().x + fX
				&& 1300.f < pDest->Get_Pos().y + fY&& 1600.f > pDest->Get_Pos().y + fY)
			{
				SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_HealActive(true);
				SCAST<CUnit*>(pDest)->Set_HealHp();
				m_fTimeCount += m_pTimeMgr->GetDeltaTime();

				if (1.f < m_fTimeCount)
				{
					m_pSoundMgr->MyPlaySound(L"포탈.MP3", CSoundMgr::CHANNELID::EFFECT);
					m_fTimeCount = 0.f;
				}
			}
			else
			{
				m_fTimeCount = 2.f;
				SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_HealActive(false);
			}

			continue;
		}

		if (2000.f < pDest->Get_Pos().x && 2200.f > pDest->Get_Pos().x
			&& 1300.f < pDest->Get_Pos().y && 1600.f > pDest->Get_Pos().y)
		{
			SCAST<CUnit*>(pDest)->Set_HealHp();
		}
	}

	for (auto& pDest : m_pObjectMgr->Get_BlueUnitList())
	{
		if (7622.f < pDest->Get_Pos().x && 7962.f > pDest->Get_Pos().x
			&& 1300.f < pDest->Get_Pos().y && 1600.f > pDest->Get_Pos().y)
		{
			SCAST<CUnit*>(pDest)->Set_HealHp();
		}
	}

	// 컬링 시작
	switch (m_eTeam)
	{
	case TEAM::RED:
		if (600.f > CScrollMgr::GetScrollPos().x || 2300.f < CScrollMgr::GetScrollPos().x)
			m_bCulling = false;
		else
			m_bCulling = true;
		break;
	case TEAM::BLUE:
		if (6400.f > CScrollMgr::GetScrollPos().x || 8000.f < CScrollMgr::GetScrollPos().x)
			m_bCulling = false;
		else
			m_bCulling = true;
		break;
	}

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	CAnimation::FrameMove(m_tFrame.fMoveSpeed);	// 프레임무브

	return NO_EVENT;
}

void CHealPortal::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CHealPortal::Render()
{
	if (false == m_bCulling)
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Heal", m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CHealPortal::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vSize = { 1.4f, 1.3f, 0.f };
	m_wstrObjectKey = L"Portal";

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	//m_tFrame.fCurrentFrame = m_tFrame.fCurrentFrame;
	m_tFrame.fCurrentFrame = 0.f;
	m_tFrame.fMaxFrameCount = 14.f;
	m_tFrame.fMoveSpeed = 1.f;
	if (2000 < m_tInfo.vPos.x && 3000 > m_tInfo.vPos.x)
		m_eTeam = TEAM::RED;
	else
		m_eTeam = TEAM::BLUE;

	m_fTimeCount = 0.f;

	return S_OK;
}

void CHealPortal::Release()
{
}

CHealPortal * CHealPortal::Create(D3DXVECTOR3& vPos)
{
	CHealPortal* pInstance = new CHealPortal;

	pInstance->Set_Pos(vPos);
	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}


	return pInstance;
}