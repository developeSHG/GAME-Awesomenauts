#include "stdafx.h"
#include "Bar.h"


CBar::CBar()
{
}

CBar::~CBar()
{
}

int CBar::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	for (int i = 0; i < TYPE::TYPE_END; ++i)
	{
		if (m_tBarInfo[i].vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tBarInfo[i].vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		{
			if (900.f < CScrollMgr::GetScrollPos().x && 2500.f > CScrollMgr::GetScrollPos().x)
				m_bBarCulling[RED1] = true;
			else
				m_bBarCulling[RED1] = false;

			if (1335.f < CScrollMgr::GetScrollPos().x && 2900.f > CScrollMgr::GetScrollPos().x)
				m_bBarCulling[RED2] = true;
			else
				m_bBarCulling[RED2] = false;

			if (2900.f < CScrollMgr::GetScrollPos().x && 4400.f > CScrollMgr::GetScrollPos().x)
				m_bBarCulling[RED3] = true;
			else
				m_bBarCulling[RED3] = false;

			if (6200.f < CScrollMgr::GetScrollPos().x && 7700.f > CScrollMgr::GetScrollPos().x)
				m_bBarCulling[BLUE1] = true;
			else
				m_bBarCulling[BLUE1] = false;

			if (5800.f < CScrollMgr::GetScrollPos().x && 7300.f > CScrollMgr::GetScrollPos().x)
				m_bBarCulling[BLUE2] = true;
			else
				m_bBarCulling[BLUE2] = false;

			if (4200.f < CScrollMgr::GetScrollPos().x && 5800.f > CScrollMgr::GetScrollPos().x)
				m_bBarCulling[BLUE3] = true;
			else
				m_bBarCulling[BLUE3] = false;
		}
		if (false == m_bBarCulling[i])
			continue;

		m_tBarFrame[i].fCurrentFrame += m_tBarFrame[i].fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * m_tBarFrame[i].fMoveSpeed;

		if (m_tBarFrame[i].fCurrentFrame >= m_tBarFrame[i].fMaxFrameCount)
			m_tBarFrame[i].fCurrentFrame = 0.f;

		SET_MATRIX_WORLD(m_tBarInfo[i]);					// 월드 매트릭스 설정
	}

	return NO_EVENT;
}

void CBar::LateUpdate()
{
}

void CBar::Render()
{
	if (true == m_bBarCulling[TYPE::RED1])
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Red1", m_tBarFrame[RED1].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tBarInfo[RED1].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (true == m_bBarCulling[TYPE::RED2])
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Red2", m_tBarFrame[RED2].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tBarInfo[RED2].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (true == m_bBarCulling[TYPE::RED3])
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Red3", m_tBarFrame[RED3].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tBarInfo[RED3].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (true == m_bBarCulling[TYPE::BLUE1])
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Blue1", m_tBarFrame[BLUE1].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tBarInfo[BLUE1].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (true == m_bBarCulling[TYPE::BLUE2])
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Blue2", m_tBarFrame[BLUE2].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tBarInfo[BLUE2].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (true == m_bBarCulling[TYPE::BLUE3])
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Blue3", m_tBarFrame[BLUE3].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tBarInfo[BLUE3].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT CBar::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_wstrObjectKey = L"Bar";

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	m_tBarInfo[RED1].vPos = { 2332.f, 1373.f, 0.f };
	m_tBarInfo[RED2].vPos = { 2712.f, 1213.f, 0.f };
	m_tBarInfo[RED3].vPos = { 4273.f, 1061.f, 0.f };
	m_tBarInfo[BLUE1].vPos = { 7591.f, 1373.f, 0.f };
	m_tBarInfo[BLUE2].vPos = { 7211.f, 1213.f, 0.f };
	m_tBarInfo[BLUE3].vPos = { 5653.f, 1061.f, 0.f };
	for (int i = 0; i < TYPE::TYPE_END; ++i)
	{
		m_tBarInfo[i].vSize = { 1.4f, 1.4f, 0.f };
		m_tBarFrame[i].fCurrentFrame = 0.f;
		m_tBarFrame[i].fMoveSpeed = 0.5f;
		m_bBarCulling[i] = false;
	}
	m_tBarFrame[RED1].fMaxFrameCount = 37.f;
	m_tBarFrame[RED2].fMaxFrameCount = 35.f;
	m_tBarFrame[RED3].fMaxFrameCount = 36.f;
	m_tBarFrame[BLUE1].fMaxFrameCount = 37.f;
	m_tBarFrame[BLUE2].fMaxFrameCount = 35.f;
	m_tBarFrame[BLUE3].fMaxFrameCount = 36.f;

	return S_OK;
}

void CBar::Release()
{
}

CBar * CBar::Create()
{
	CBar* pInstance = new CBar;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
