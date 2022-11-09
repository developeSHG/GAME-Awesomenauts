#include "stdafx.h"
#include "Goblin.h"


CGoblin::CGoblin()
{
}


CGoblin::~CGoblin()
{
}

int CGoblin::Update()
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


	// �ø� ����
	if (m_tInfo.vPos.x - 1485.f < CScrollMgr::GetScrollPos().x && m_tInfo.vPos.x + 120.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// �ø� ��

	return NO_EVENT;
}

void CGoblin::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);

	CMinion::CreateUpdate();								// ���� �ִϸ��̼� ������Ʈ

	if (true == m_bCreate)
		return;
	
	if (STATE::DEAD != m_eCurrentState)
		CAnimation::FrameMove(m_tFrame.fMoveSpeed);			// �����ӹ���
	else
		CAnimation::FrameMoveDead(m_tFrame.fMoveSpeed);

	CUnit::NewUpdateRect(true);								// �ٵ�Ʈ �ֽ�ȭ
	//CUnit::UpdateAttackRect(true);						// ���ݷ�Ʈ �ֽ�ȭ
	CMinion::CollMap();										// Ÿ��,�� �浹, ����üũ
	FrameCheck();											// State üũ
	AttackStateActive();									// State�� ���ݷ�Ʈ Ȱ��ȭ
	CUnit::HurtActive();									// ���ݹ޾��� �� Rgb�� ��ȭ
}

void CGoblin::Render()
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
	// �̴Ͼ� ���� ���� ��ȯ�ִϸ��̼� ���� ����
	CMinion::CreateRender();
	// �̴Ͼ� ���� ���� ��ȯ�ִϸ��̼� ���� ��

	// Hp ���� ����
	CMinion::HpRender();
	// Hp ���� ��

	// ȣ�� ���� ����
	CUnit::LakeRender();
	// ȣ�� ���� ��

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 0, 255, 0))
	LINE_RENDER_OBSERVE(D3DCOLOR_ARGB(255, 0, 0, 255))
	LINE_RENDER_ATTACK(D3DCOLOR_ARGB(255, 255, 0, 0))
#endif
}

HRESULT CGoblin::Initialize()
{
	// CGameObject ��Ӻ��� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
	if (TEAM_TYPE::RED == m_eTeamType)
	{
		m_wstrObjectKey = L"RedGoblin";
		m_tInfo.vSize = { 0.82f, 0.82f, 0.f };
		m_tInfo.vPos = { 2505.f, 1660.f, 0.f };
	}
	else if (TEAM_TYPE::BLUE == m_eTeamType)
	{
		m_wstrObjectKey = L"BlueGoblin";
		m_tInfo.vSize = { 0.82f, 0.82f, 0.f };
		m_tInfo.vPos = { 7450.f, 1660.f, 0.f };
	}
	m_fBornSizeX = m_tInfo.vSize.x;
	m_fSpeed = 185.f;

	// CAnimation, �������� (�ʱ�ȭ ���� �ʿ�� �ڵ�)x
	//if (DIR::RIGHT == m_eDir)
	//	m_tImageSink.fX = 15;
	//else if (DIR::LEFT == m_eDir)
	//	m_tImageSink.fX = 0;
	m_tImageSink.fY = 27.f;
	m_tRectInfo[RECT_TYPE::BODY].fcx = 80.f;
	m_tRectInfo[RECT_TYPE::BODY].fcy = 80.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcx = 250.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcy = 80.f;
	m_tRectInfo[RECT_TYPE::ATTACK].fcx = 15.f;
	m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
	m_tStatus.fMaxHp = 90.f;
	m_tStatus.fHp = m_tStatus.fMaxHp;
	m_tStatus.fAttack = 7.f;

	// �ø� ����
	if (m_tInfo.vPos.x - 1485.f < CScrollMgr::GetScrollPos().x && m_tInfo.vPos.x + 120.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return S_OK;
	// �ø� ��

	m_pSoundMgr->MyPlaySound(L"warrior4.wav", CSoundMgr::CHANNELID::EFFECT);

	return S_OK;
}

void CGoblin::Release()
{
}

void CGoblin::FrameCheck()
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
		m_tFrame.fMaxFrameCount = 6.f;
		m_tFrame.fMoveSpeed = 1.3f;
		break;
	case STATE::MOVE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 6.f;
		m_tFrame.fMoveSpeed = 1.5f;
		break;
	case STATE::ATTACK:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 12.f;
		m_tFrame.fMoveSpeed = 1.f;
		break;
	case STATE::DEAD:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 12.f;
		m_tFrame.fMoveSpeed = 1.f;

		if (m_bCulling)
			m_pSoundMgr->MyPlaySound(L"death1.wav", CSoundMgr::CHANNELID::EFFECT);
		break;
	}

	m_eCurrentState = m_eNextState;
}

void CGoblin::AttackStateActive()
{
	//ZeroMemory(&m_tRectInfo[RECT_TYPE::ATTACK].rcRect, sizeof(RECT));
	CUnit::UpdateAttackRect(true);						// ���ݷ�Ʈ �ֽ�ȭ
	m_bAttackRectActive = false;

	if (STATE::ATTACK == m_eCurrentState)
	{
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 5.f, 7.f, true);

		if (5.f <= m_tFrame.fCurrentFrame && 5.5f >= m_tFrame.fCurrentFrame)
		{
			if (false == m_bCulling)
				return;

			CSoundMgr::GetInstance()->MyPlaySound(L"����2.wav", CSoundMgr::CHANNELID::EFFECT);
		}
	}
}

CGoblin * CGoblin::Create(TEAM_TYPE _eType, MINION_AI_TYPE _eAiType)
{
	CGoblin* pInstance = new CGoblin;

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
