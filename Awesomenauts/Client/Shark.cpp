#include "stdafx.h"
#include "Shark.h"

#include "Element.h"

CShark::CShark()
{
}


CShark::~CShark()
{
}

int CShark::Update()
{
	if (TRUE == m_bIsDead || TRUE == m_bState[STATE::DEAD])
		return OBJ_DEAD;

	CMinion::AISystem();

	if (m_pTarget)
	{
		m_bState[STATE::ATTACK] = true;
		CreateElement();

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

void CShark::LateUpdate()
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
															//CUnit::UpdateAttackRect(true);						// 공격렉트 최신화
	CMinion::CollMap();										// 타일,맵 충돌, 점프체크
	FrameCheck();											// State 체크
	AttackStateActive();									// State별 공격렉트 활성화
	CUnit::HurtActive();									// 공격받았을 때 Rgb값 변화
}

void CShark::Render()
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
	CMinion::HpRender();
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

HRESULT CShark::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	if (TEAM_TYPE::RED == m_eTeamType)
	{
		m_wstrObjectKey = L"RedShark";
		m_tInfo.vSize = { 0.87f, 0.87f, 0.f };
		m_tInfo.vPos = { 2505.f, 1660.f, 0.f };
	}
	else if (TEAM_TYPE::BLUE == m_eTeamType)
	{
		m_wstrObjectKey = L"BlueShark";
		m_tInfo.vSize = { 0.87f, 0.87f, 0.f };
		m_tInfo.vPos = { 7450.f, 1660.f, 0.f };
	}
	m_fBornSizeX = m_tInfo.vSize.x;
	m_fSpeed = 185.f;
	//m_fSpeed = 500.f;

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)x
	//if (DIR::RIGHT == m_eDir)
	//	m_tImageSink.fX = 15;
	//else if (DIR::LEFT == m_eDir)
	//	m_tImageSink.fX = 15;
	m_tImageSink.fY = 5;
	m_tRectInfo[RECT_TYPE::BODY].fcx = 80.f;
	m_tRectInfo[RECT_TYPE::BODY].fcy = 80.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcx = 450.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcy = 80.f;
	//m_tRectInfo[RECT_TYPE::ATTACK].fcx = 15.f;
	//m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
	m_tStatus.fMaxHp = 90.f;
	m_tStatus.fHp = m_tStatus.fMaxHp;
	m_tStatus.fAttack = 3.f;

	// 컬링 시작
	if (m_tInfo.vPos.x - 1485.f < CScrollMgr::GetScrollPos().x && m_tInfo.vPos.x + 120.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return S_OK;
	// 컬링 끝

	m_pSoundMgr->MyPlaySound(L"archer5.wav", CSoundMgr::CHANNELID::EFFECT);

	return S_OK;
}

void CShark::Release()
{
}

void CShark::FrameCheck()
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
		m_tFrame.fMaxFrameCount = 7.f;
		m_tFrame.fMoveSpeed = 1.3f;
		break;
	case STATE::MOVE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 8.f;
		m_tFrame.fMoveSpeed = 1.3f;
		break;
	case STATE::ATTACK:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 11.f;
		m_tFrame.fMoveSpeed = 0.85f;
		break;
	case STATE::DEAD:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 6.f;
		m_tFrame.fMoveSpeed = 1.f;

		if (m_bCulling)
			m_pSoundMgr->MyPlaySound(L"death2.wav", CSoundMgr::CHANNELID::EFFECT);
		break;
	}

	m_eCurrentState = m_eNextState;
}

void CShark::AttackStateActive()
{
	//ZeroMemory(&m_tRectInfo[RECT_TYPE::ATTACK].rcRect, sizeof(RECT));
	CUnit::UpdateAttackRect(true);						// 공격렉트 최신화
	m_bAttackRectActive = false;

	if (STATE::ATTACK == m_eCurrentState)
	{
			CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 5.f, 8.f, true);

			if (5.f <= m_tFrame.fCurrentFrame && 5.5f >= m_tFrame.fCurrentFrame)
			{
				if (false == m_bCulling)
					return;

				CSoundMgr::GetInstance()->MyPlaySound(L"공격1.wav", CSoundMgr::CHANNELID::EFFECT);
			}
	}


}

HRESULT CShark::CreateElement()
{
	if (!(5.9f <= m_tFrame.fCurrentFrame && 6.1f >= m_tFrame.fCurrentFrame))
		return E_FAIL;

	CGameObject* pGameObject = nullptr;
	HRESULT hr;

	D3DXVECTOR3 vPos, vSize;

	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	if (DIR::RIGHT == m_eDir)
	{
		vPos = { float(m_tInfo.vPos.x + 15.f - fScrollX), float(m_tInfo.vPos.y - 52.5f - fScrollY), 0.f };
		vSize = { 0.8f, 0.8f, 0.f };
	}
	else if (DIR::LEFT == m_eDir)
	{
		vPos = { float(m_tInfo.vPos.x - 15.f - fScrollX), float(m_tInfo.vPos.y - 42.5f - fScrollY), 0.f };
		vSize = { 0.8f, 0.8f, 0.f };
	}

	vPos.x += fScrollX;
	vPos.y += fScrollY;

	D3DXVECTOR3 vTargetPos = { m_pTarget->Get_Pos().x, m_pTarget->Get_Pos().y - 30.f, 0.f };
	if (m_pTarget == m_pObjectMgr->Get_Player())
	{
		vTargetPos.x += fScrollX;
		vTargetPos.y += fScrollY;
	}

	D3DXVECTOR3 vDir = vTargetPos - vPos;

	const ELEMENT_INFO tElement =
	{
		m_wstrObjectKey, L"Element", m_eDir, vDir, vSize, 10,
		225.f, FRAME(0.f, 4.f, 2.f), IMAGE_SINK(-10.f, 25.f), 30.f, 15.f, 5.f, 1.5f, IMAGE_SINK(-10.f, 40.f), 0.5f
	};

	pGameObject = CElement::Create(vPos, tElement);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Element Create Failed", E_FAIL);

	if (TEAM_TYPE::RED == m_eTeamType)
		hr = m_pObjectMgr->AddObject(OBJECT::TYPE::RED_ELEMENT, pGameObject);
	else if (TEAM_TYPE::BLUE == m_eTeamType)
		hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BLUE_ELEMENT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower Element AddObject Failed", E_FAIL);
}

CShark * CShark::Create(TEAM_TYPE _eType, MINION_AI_TYPE _eAiType)
{
	CShark* pInstance = new CShark;

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
