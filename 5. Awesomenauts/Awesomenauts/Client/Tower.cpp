#include "stdafx.h"
#include "Tower.h"

#include "Element.h"
#include "Player.h"
#include "Hurt.h"

CTower::CTower()
{
}


CTower::~CTower()
{
}

int CTower::Update()
{
	if (TRUE == m_bIsDead || TRUE == m_bState[STATE::DEAD])
		return OBJ_DEAD;

	if (STATE::DEAD == m_eCurrentState)
		CGameObject::LateInit();

	if (m_pTarget)
	{
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
	else
	{
		if (TEAM_TYPE::BLUE == m_eTeamType)
			m_eDir = DIR::LEFT;
		else if (TEAM_TYPE::RED == m_eTeamType)
			m_eDir = DIR::RIGHT;
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

void CTower::LateUpdate()
{
	if (DIR::RIGHT == m_eDir)
	{
		m_tInfo.vSize = { -1.45f, 1.8f, 0.f };
		m_tImageSink.fX = -15.f;
	}
	else if (DIR::LEFT == m_eDir)
	{
		m_tInfo.vSize = { 1.45f, 1.8f, 0.f };
		m_tImageSink.fX = -15.f;
	}

	SET_MATRIX_WORLD(m_tInfo);
	if(STATE::DEAD != m_eCurrentState)
		CAnimation::FrameMove(m_tFrame.fMoveSpeed);		// 프레임무브
	else
		CAnimation::FrameMoveDead(m_tFrame.fMoveSpeed);
	FrameCheck();									// State 체크
	CUnit::HurtActive();							// 공격받았을 때 Rgb값 변화
	CUnit::NewUpdateRect(true);						// 바디렉트 최신화
	CUnit::UpdateAttackRect(true);					// 공격렉트 최신화
}

void CTower::Render()
{
	if (false == m_bCulling || true == m_bState[STATE::DEAD])
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, m_wstrStateKey[m_eCurrentState], m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);
	
	float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - m_tImageSink.fX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHurtRgb[HURT_RGB::G], m_iHurtRgb[HURT_RGB::B]));

	// Hp 랜더 시작
	CHero::HpRender(10.f, - 60.f);
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

HRESULT CTower::LateInit()
{
	if (TEAM_TYPE::RED == m_eTeamType)
	{
		if (TOWER_POS::UP == m_eTowerPos)
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_TowerPosTextActive(0);
		else
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_TowerPosTextActive(1);
	}
	else if (TEAM_TYPE::BLUE == m_eTeamType)
	{
		if (TOWER_POS::UP == m_eTowerPos)
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_TowerPosTextActive(2);
		else
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_TowerPosTextActive(3);

	}
	SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_TextActive();

	return S_OK;
}

HRESULT CTower::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	if (TEAM_TYPE::RED == m_eTeamType)
		m_wstrObjectKey = L"RedTower";
	else if (TEAM_TYPE::BLUE == m_eTeamType)
		m_wstrObjectKey = L"BlueTower";

	m_fSpeed = 250.f;

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	if (DIR::RIGHT == m_eDir)
	{
		m_tInfo.vSize = { -1.45f, 1.8f, 0.f };
		m_tImageSink.fX = -15.f;
	}
	else if (DIR::LEFT == m_eDir)
	{
		m_tInfo.vSize = { 1.45f, 1.8f, 0.f };
		m_tImageSink.fX = -15.f;
	}
	m_tRectInfo[RECT_TYPE::BODY].fcx = 170.f;
	m_tRectInfo[RECT_TYPE::BODY].fcy = 130.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcx = 650.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcy = 130.f;
	m_tStatus.fMaxHp = 500;
	m_tStatus.fHp = m_tStatus.fMaxHp;
	m_tStatus.fAttack = 15;
	m_bTowerHp = true;

	return S_OK;
}

void CTower::Release()
{
}

void CTower::FrameCheck()
{
	if (STATE::DEAD != m_eNextState)
	{
		//if (false == m_bState[STATE::ATTACK])
		//	m_eNextState = STATE::IDLE;
		//else if(true == m_bState[STATE::ATTACK])
		//	m_eNextState = STATE::ATTACK;

		if (nullptr == m_pTarget)
			m_eNextState = STATE::IDLE;
		else if (nullptr != m_pTarget)
			m_eNextState = STATE::ATTACK;
	}

	if (m_eCurrentState == m_eNextState)
		return;

	switch (m_eNextState)
	{
	case STATE::IDLE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 10.f;
		m_tFrame.fMoveSpeed = 1.f;
		break;
	case STATE::ATTACK:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 7.f;
		m_tFrame.fMoveSpeed = 1.3f;
		break;
	case STATE::DEAD:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 15.f;
		m_tFrame.fMoveSpeed = 0.8f;
		break;
	}

	m_eCurrentState = m_eNextState;
}

HRESULT CTower::CreateElement()
{
	if (!(4.95f <= m_tFrame.fCurrentFrame && 5.05f >= m_tFrame.fCurrentFrame))
		return E_FAIL;

	CGameObject* pGameObject = nullptr;
	HRESULT hr;

	D3DXVECTOR3 vPos, vSize;
	
	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	if (DIR::RIGHT == m_eDir)
	{
		vPos = { float(m_tInfo.vPos.x + 115.f - fScrollX), float(m_tInfo.vPos.y - 110.f - fScrollY), 0.f };
		vSize = { 0.8f, 0.8f, 0.f };
	}
	else if (DIR::LEFT == m_eDir)
	{
		vPos = { float(m_tInfo.vPos.x - 115.f - fScrollX), float(m_tInfo.vPos.y - 110.f - fScrollY), 0.f };
		vSize = { 0.8f, 0.8f, 0.f };
	}

	vPos.x += fScrollX;
	vPos.y += fScrollY;

	D3DXVECTOR3 vTargetPos = { m_pTarget->Get_Pos().x, m_pTarget->Get_Pos().y - 20.f, 0.f };
	if (m_pTarget == m_pObjectMgr->Get_Player())
	{
		vTargetPos.x += fScrollX;
		vTargetPos.y += fScrollY;
	}

	D3DXVECTOR3 vDir = vTargetPos - vPos;

	const ELEMENT_INFO tElement =
	{
		L"Tower", L"Element", m_eDir, vDir, vSize, 20,
		300.f, FRAME(0.f, 2.f, 3.f), IMAGE_SINK(-10.f, 20.f), 30.f, 15.f, 5.f, 1.5f, IMAGE_SINK(-10.f, 50.f), 1.f
	};

	pGameObject = CElement::Create(vPos, tElement);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Element Create Failed", E_FAIL);

	if (TEAM_TYPE::RED == m_eTeamType)
		hr = m_pObjectMgr->AddObject(OBJECT::TYPE::RED_ELEMENT, pGameObject);
	else if (TEAM_TYPE::BLUE == m_eTeamType)
		hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BLUE_ELEMENT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower Element AddObject Failed", E_FAIL);
}

CTower * CTower::Create(D3DXVECTOR3& _vPos, TEAM_TYPE _eType, TOWER_POS _ePos)
{
	CTower* pInstance = new CTower;

	pInstance->Set_TeamType(_eType);
	pInstance->Set_TowerPos(_ePos);

	if (TEAM_TYPE::RED == _eType)
		pInstance->Set_Dir(DIR::RIGHT);
	else if (TEAM_TYPE::BLUE == _eType)
		pInstance->Set_Dir(DIR::LEFT);


	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(_vPos);

	return pInstance;
}
