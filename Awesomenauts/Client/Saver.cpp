#include "stdafx.h"
#include "Saver.h"


CSaver::CSaver()
{
}


CSaver::~CSaver()
{
}

int CSaver::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// ��������Ȱ
	if (TRUE == m_bState[STATE::DEAD])
		CHero::DeadRespawn();

	if (STATE::DEAD == m_eNextState)
		return NO_EVENT;

	if (1950.f < CScrollMgr::GetScrollPos().x && false == m_bGameStart)
		m_bGameStart = true;

	//if (30.f > m_tStatus.fHp)
	//	m_bHealGo = true;

	if (false == m_bState[STATE::ATTACK])
	{
		m_fSpeed = 250.f;
		if (false == m_bState[STATE::JUMP] && true == m_bIsColl && false == m_bState[STATE::IDLE])
			m_eNextState = STATE::MOVE;
		else if (true == m_bState[STATE::JUMP])
		{
			m_eNextState = STATE::MOVE;
			m_fSpeed = 170.f;
		}
		else if (true == m_bState[STATE::IDLE])
			m_eNextState = STATE::IDLE;

		if (!m_pTarget)
			m_eDir = DIR::LEFT;

		if (true == m_bGameStart)
		{
			//if (m_bHealGo = false)
			{
				// �⺻ �����ΰ���
				if (false == m_bState[STATE::IDLE] && DIR::RIGHT == m_eDir)
				{
					m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * m_fSpeed;
					//m_tInfo.vSize.x *= -1.f;
				}
				else if (false == m_bState[STATE::IDLE] && DIR::LEFT == m_eDir)
				{
					m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * -m_fSpeed;
				}
			}
		/*	else
			{
				
			}*/
		}
	}
	else if (true == m_bState[STATE::ATTACK])
	{
		m_eNextState = STATE::ATTACK;

		if (0.f == m_tFrame.fCurrentFrame)
			m_bAttackOnlyTarget = true;
	}

	CHero::CollTile();

	// ����, �߷»��� ����
	if (true == m_bState[STATE::JUMP])
	{
		m_tJumpInfo.fPower -= m_pTimeMgr->GetDeltaTime() * 2300.f;
		m_tInfo.vPos.y -= m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fPower;
	}
	// �߷�����
	else if (false == m_bState[STATE::JUMP] && false == m_bIsColl)
	{
		m_eNextState = STATE::MOVE;
		m_tJumpInfo.fPower = 50.f;
		m_tJumpInfo.fGravity += m_pTimeMgr->GetDeltaTime() * 800.f;
		m_tInfo.vPos.y += m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fGravity;
	}
	// ����, �߷»��� ��

	CUnit::TargetDirCheck();						// Ÿ�� ����üũ

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

void CSaver::LateUpdate()
{
	if (STATE::DEAD != m_eCurrentState)
		CAnimation::FrameMove(m_tFrame.fMoveSpeed);			// �����ӹ���
	else
		CAnimation::FrameMoveDead(m_tFrame.fMoveSpeed);

	CUnit::NewUpdateRect(true);								// �ٵ�Ʈ �ֽ�ȭ
	CHero::CollMap();										// Ÿ��,�� �浹, ����üũ
	FrameCheck();											// State üũ
	DirCheck();												// ���� üũ
	AttackStateActive();									// State�� ���ݷ�Ʈ Ȱ��ȭ
	CUnit::HurtActive();									// ���ݹ޾��� �� Rgb�� ��ȭ

	SET_MATRIX_WORLD(m_tInfo);
}

void CSaver::Render()
{
	if (false == m_bCulling || true == m_bState[STATE::DEAD])
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, m_wstrStateKey[m_eCurrentState], m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - m_tImageSink.fX, fCenterY - m_tImageSink.fY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHurtRgb[HURT_RGB::G], m_iHurtRgb[HURT_RGB::B]));

	// Hp ���� ����
	CHero::HpRender(0.f, -35.f);
	// Hp ���� ��

	// ȣ�� ���� ����
	CUnit::LakeRender();
	// ȣ�� ���� ��

	// ������ ���� ����
	CUnit::LevelUpRender();
	// ������ ���� ��

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 0, 255, 0))
		LINE_RENDER_OBSERVE(D3DCOLOR_ARGB(255, 0, 0, 255))
		LINE_RENDER_ATTACK(D3DCOLOR_ARGB(255, 255, 0, 0))
#endif
}

HRESULT CSaver::Initialize()
{
	// CGameObject ��Ӻ��� (�ʱ�ȭ ���� �ʿ�� �ڵ�)

	m_wstrObjectKey = L"Saver";
	m_tInfo.vSize = { 0.65f, 0.65f, 0.f };
	m_tInfo.vPos = { 9505.f, 1160.f, 0.f };
	m_fSpeed = 250.f;
	m_eTeamType = TEAM_TYPE::BLUE;
	m_eDir = DIR::LEFT;

	// CAnimation, �������� (�ʱ�ȭ ���� �ʿ�� �ڵ�)x
	//if (DIR::RIGHT == m_eDir)
	//	m_tImageSink.fX = 40;
	//else if (DIR::LEFT == m_eDir)
	//	m_tImageSink.fX = -40;
	m_tImageSink.fX = -40.f;
	m_tImageSink.fY = 125.f;
	m_tRectInfo[RECT_TYPE::BODY].fcx = 80.f;
	m_tRectInfo[RECT_TYPE::BODY].fcy = 80.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcx = 250.f;
	m_tRectInfo[RECT_TYPE::OBSERVE].fcy = 80.f;
	m_tRectInfo[RECT_TYPE::ATTACK].fcx = 20.f;
	m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
	m_tStatus.fMaxHp = 300.f;
	m_tStatus.fHp = m_tStatus.fMaxHp;
	m_tStatus.fAttack = 20.f;
	m_tStatus.fMaxExp = 30.f;

	return S_OK;
}

HRESULT CSaver::LateInit()
{
	return E_NOTIMPL;
}

void CSaver::Release()
{
}

void CSaver::FrameCheck()
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
		m_tFrame.fMaxFrameCount = 8.f;
		m_tFrame.fMoveSpeed = 1.3f;
		break;
	case STATE::MOVE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 7.f;
		m_tFrame.fMoveSpeed = 1.5f;
		break;
	case STATE::ATTACK:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 7.f;
		m_tFrame.fMoveSpeed = 0.7f;
		break;
	case STATE::SKILL1:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 9.f;
		m_tFrame.fMoveSpeed = 1.f;
		break;
	case STATE::DEAD:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 10.f;
		m_tFrame.fMoveSpeed = 1.f;
		m_tAbility.iDead += 1;

		if(m_bCulling)
			CSoundMgr::GetInstance()->MyPlaySound(L"death4.wav", CSoundMgr::CHANNELID::EFFECT);
		break;
	}

	m_eCurrentState = m_eNextState;
}

void CSaver::AttackStateActive()
{
	//ZeroMemory(&m_tRectInfo[RECT_TYPE::ATTACK].rcRect, sizeof(RECT));
	CUnit::UpdateAttackRect(true);						// ���ݷ�Ʈ �ֽ�ȭ
	m_bAttackRectActive = false;

	if (STATE::ATTACK == m_eCurrentState)
	{
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 3.f, 5.f, true);

		if (3.f <= m_tFrame.fCurrentFrame && 3.5f >= m_tFrame.fCurrentFrame)
		{
			if (false == m_bCulling)
				return;

			CSoundMgr::GetInstance()->MyPlaySound(L"beck_fireball.ogg", CSoundMgr::CHANNELID::EFFECT);
		}
	}

}

void CSaver::DirCheck()
{
	switch (m_eDir)
	{
	case DIR::RIGHT:
		m_tInfo.vSize = { -0.65f, 0.65f, 0.f };
		break;
	case DIR::LEFT:
		m_tInfo.vSize = { 0.65f, 0.65f, 0.f };
		break;
	}
}

CSaver * CSaver::Create()
{
	CSaver* pInstance = new CSaver;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
