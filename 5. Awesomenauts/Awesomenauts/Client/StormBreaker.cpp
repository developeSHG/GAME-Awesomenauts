#include "stdafx.h"
#include "StormBreaker.h"


CStormBreaker::CStormBreaker()
{
}


CStormBreaker::~CStormBreaker()
{
}

int CStormBreaker::Update()
{
	if (TRUE == m_bIsDead || TRUE == m_bState[STATE::DEAD])
		return OBJ_DEAD;

	CMinion::AISystem();

	if (m_pTarget)
	{
		if (m_pTarget == m_pObjectMgr->Get_Player())
		{
			if (m_pTarget->Get_Pos().x < m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x)
				m_eDir = DIR::LEFT;
			else
				m_eDir = DIR::RIGHT;
		}
		else
		{
			if (m_pTarget->Get_Pos().x < m_tInfo.vPos.x)
				m_eDir = DIR::LEFT;
			else
				m_eDir = DIR::RIGHT;
		}

		if (STATE::DEAD == SCAST<CUnit*>(m_pTarget)->Get_CurrentState())
			m_pTarget = nullptr;
	}


	// 컬링 시작
	if (m_tInfo.vPos.x - 1485.f < CScrollMgr::GetScrollPos().x && m_tInfo.vPos.x + 120.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	return NO_EVENT;
}

void CStormBreaker::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);

	CMinion::CreateUpdate();								// 생성 애니메이션 업데이트

	if (true == m_bCreate)
		return;

	if (STATE::DEAD != m_eCurrentState)
		CAnimation::FrameMove(m_tFrame.fMoveSpeed);			// 프레임무브
	else
		CAnimation::FrameMoveDead(m_tFrame.fMoveSpeed);

	CUnit::NewUpdateRect(true);								// 바디렉트 최신화
	CMinion::CollMap();										// 타일,맵 충돌, 점프체크
	FrameCheck();											// State 체크
	AttackStateActive();									// State별 공격렉트 활성화
	CUnit::HurtActive();									// 공격받았을 때 Rgb값 변화
}

void CStormBreaker::Render()
{
	if (false == m_bCulling || true == m_bState[STATE::DEAD])
		return;

	if (false == m_bCreate)
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, m_wstrStateKey[m_eCurrentState], m_tFrame.fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
		float fCenterY = float(m_pTexInfo->tImgInfo.Height);

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX - m_tImageSink.fX, fCenterY - m_tImageSink.fY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHurtRgb[HURT_RGB::G], m_iHurtRgb[HURT_RGB::B]));
	}
	// 미니언 생성 시작 소환애니메이션 렌더 시작
	CMinion::CreateRender();
	// 미니언 생성 시작 소환애니메이션 렌더 끝

	// Hp 랜더 시작
	if (TEAM_TYPE::RED == m_eTeamType)
		CMinion::HpRender(-2.f, -12.5f);
	else
		CMinion::HpRender(2.f, -12.5f);
	// Hp 랜더 끝

	// 호수 랜더 시작
	CUnit::LakeRender();
	// 호수 랜더 끝

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 0, 255, 0))
		LINE_RENDER_OBSERVE(D3DCOLOR_ARGB(255, 0, 0, 255))
		LINE_RENDER_ATTACK(D3DCOLOR_ARGB(255, 255, 0, 0))
#endif
}

HRESULT CStormBreaker::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	if (TEAM_TYPE::RED == m_eTeamType)
	{
		m_wstrObjectKey = L"RedStormBreaker";
		m_tInfo.vSize = { 0.975f, 0.975f, 0.f };
		m_tInfo.vPos = { 2505.f, 1660.f, 0.f };
	}
	else if (TEAM_TYPE::BLUE == m_eTeamType)
	{
		m_wstrObjectKey = L"BlueStormBreaker";
		m_tInfo.vSize = { 0.975f, 0.975f, 0.f };
		m_tInfo.vPos = { 7450.f, 1660.f, 0.f };
	}
	m_fBornSizeX = m_tInfo.vSize.x;
	m_fSpeed = 185.f;
	//m_fSpeed = 500.f;

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)x
	if (DIR::RIGHT == m_eDir)
		m_tImageSink.fX = 77.f;
	else if (DIR::LEFT == m_eDir)
		m_tImageSink.fX = -77.f;
	m_tImageSink.fY = 34.f;
	m_tRectInfo[RECT_TYPE::BODY].fcx = 75.f;
	m_tRectInfo[RECT_TYPE::BODY].fcy = 100.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcx = 290.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcy = 80.f;
	m_tRectInfo[RECT_TYPE::ATTACK].fcx = 100.f;
	m_tRectInfo[RECT_TYPE::ATTACK].fcy = 30.f;
	m_tStatus.fMaxHp = 90.f;
	m_tStatus.fHp = m_tStatus.fMaxHp;
	m_tStatus.fAttack = 4.5f;

	// 컬링 시작
	if (m_tInfo.vPos.x - 1485.f < CScrollMgr::GetScrollPos().x && m_tInfo.vPos.x + 120.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return S_OK;
	// 컬링 끝

	m_pSoundMgr->MyPlaySound(L"worker1.wav", CSoundMgr::CHANNELID::EFFECT);

	return S_OK;
}

void CStormBreaker::Release()
{
}

void CStormBreaker::FrameCheck()
{
	//if (STATE::DEAD != m_eNextState)
	//{
	//	if (false == m_bState[STATE::ATTACK])
	//		m_eNextState = STATE::MOVE;
	//	else if (true == m_bState[STATE::ATTACK])
	//		m_eNextState = STATE::ATTACK;
	//}

	if (m_eCurrentState == m_eNextState)
		return;

	switch (m_eNextState)
	{
	case STATE::IDLE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 5.f;
		m_tFrame.fMoveSpeed = 2.f;
		break;
	case STATE::MOVE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 6.f;
		m_tFrame.fMoveSpeed = 2.f;
		break;
	case STATE::ATTACK:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 8.f;
		m_tFrame.fMoveSpeed = 1.35f;
		break;
	case STATE::DEAD:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 13.f;
		m_tFrame.fMoveSpeed = 1.f;

		if (m_bCulling)
			m_pSoundMgr->MyPlaySound(L"death5.wav", CSoundMgr::CHANNELID::EFFECT);
		break;
	}

	m_eCurrentState = m_eNextState;
}

void CStormBreaker::AttackStateActive()
{
	//ZeroMemory(&m_tRectInfo[RECT_TYPE::ATTACK].rcRect, sizeof(RECT));
	CUnit::UpdateAttackRect(true);						// 공격렉트 최신화
	m_bAttackRectActive = false;

	if (STATE::ATTACK == m_eCurrentState)
	{
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 2.f, 3.f, true);

		if (2.f <= m_tFrame.fCurrentFrame && 2.5f >= m_tFrame.fCurrentFrame)
		{
			if (false == m_bCulling)
				return;
			CSoundMgr::GetInstance()->MyPlaySound(L"공격1.wav", CSoundMgr::CHANNELID::EFFECT);
		}
	}

}

CStormBreaker * CStormBreaker::Create(TEAM_TYPE _eType, MINION_AI_TYPE _eAiType)
{
	CStormBreaker* pInstance = new CStormBreaker;

	pInstance->Set_TeamType(_eType);
	pInstance->Set_AiType(_eAiType);

	if (TEAM_TYPE::RED == _eType)
		pInstance->Set_Dir(DIR::RIGHT);
	else if (TEAM_TYPE::BLUE == _eType)
		pInstance->Set_Dir(DIR::LEFT);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
