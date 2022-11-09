#include "stdafx.h"
#include "Player.h"

#include "MyMap.h"
#include "Hurt.h"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Release();
}

int CPlayer::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	if (true == CScrollMgr::GetSpaceScroll())
		return NO_EVENT;
		
	if (STATE::DEAD == m_eCurrentState)
	{
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_HurtActive();
	}

	// 리스폰부활
	if (true == m_bState[STATE::DEAD])
	{
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_DeadCoolTimeActive();
		CHero::DeadRespawn(false);
		return NO_EVENT;
	}

	// 스킬쿨타임 체크
	SkillCoolTimeCheck();

	//CGameObject::LateInit();
	if (STATE::DEAD != m_eCurrentState)
		CAnimation::FrameMove(m_tFrame.fMoveSpeed);		// 프레임무브
	else
		CAnimation::FrameMoveDead(m_tFrame.fMoveSpeed);
	IsOffset();											// 스크롤 관리
	KeyInput();											// 키 조작

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	//SET_MATRIX_WORLD_NOSCROLL(m_tInfo);			// 월드 매트릭스 설정
//	CGameObject::UpdateRect();
	if (true == CScrollMgr::GetSpaceScroll())
		return;

	// 점프상태 시작
	if (true == m_bState[STATE::JUMP])
	{
		m_tJumpInfo.fPower -= m_pTimeMgr->GetDeltaTime() * 2000.f;
		m_tInfo.vPos.y -= m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fPower;
	}
	else if(false == m_bState[STATE::JUMP] && false == m_bIsColl && false == m_bIsCollMoveTile) // 중력적용
	{
		m_bState[STATE::JUMP_DOWN] = true;
		m_tJumpInfo.fPower = 1000.f;
		m_tJumpInfo.fGravity += m_pTimeMgr->GetDeltaTime() * 800.f;
		//if(true == m_bOffsetScrollActiveY)
			//CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 50.f * m_pTimeMgr->GetDeltaTime(), 0.f));
		m_tInfo.vPos.y += m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fGravity;
	}
	// 점프상태 끝

	CUnit::NewUpdateRect(false);
	CUnit::UpdateAttackRect(false);
	CUnit::UpdateSkillRect(false);
	if (true != m_bState[STATE::DEAD])
		CollMap();										// 타일,맵 충돌, 점프체크
	//IsOffset();									// Y스크롤 관리
	FrameCheck();									// State 체크
	AttackStateActive();							// State별 공격렉트 활성화
	DirCheck();										// 방향 체크
	CUnit::HurtActive();							// 공격받았을 때 Rgb값 변화
	CHero::LightIn();								// 조명부스터 탔을떄 함수
}

void CPlayer::Render()
{
	if (true == CScrollMgr::GetSpaceScroll() || true == m_bState[STATE::DEAD])
		return;

	if (false == m_bState[STATE::TAXI])
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, m_wstrStateKey[m_eCurrentState], m_tFrame.fCurrentFrame);
		NULL_CHECK(m_pTexInfo);
	
		float fCenterX = float(m_pTexInfo->tImgInfo.Width) * 0.5f;
		float fCenterY = float(m_pTexInfo->tImgInfo.Height);
		
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX -m_tImageSink.fX, fCenterY - m_tImageSink.fY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHurtRgb[HURT_RGB::G], m_iHurtRgb[HURT_RGB::B]));

		// Hp 랜더 시작
		if (DIR::RIGHT == m_eDir)
			CHero::HpRender(-5.f, -15.f, false);
		else
			CHero::HpRender(-5.f, -15.f, false);
		// Hp 랜더 끝

		// 호수 랜더 시작
		LakeRender();
		// 호수 랜더 끝
	}

	// 택시 렌더
	TaxiStateRender();

	// 레벨업 랜더 시작
	CUnit::LevelUpRender(false);
	// 레벨업 랜더 끝

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 0, 255, 0))
	LINE_RENDER_ATTACK(D3DCOLOR_ARGB(255, 255, 0, 0))
	LINE_RENDER_SKILL(D3DCOLOR_ARGB(255, 255, 0, 0))
#endif
}

HRESULT CPlayer::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { WINCX/2 + 0.f, 480.f, 0.f };
	m_tInfo.vSize = { 1.2f, 1.2f, 0.f };
	m_eDir = DIR::RIGHT;
	m_wstrObjectKey = L"Player";
	m_fSpeed = 400.f;
	m_eTeamType = TEAM_TYPE::RED;

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	m_tJumpInfo.fPower = 1000.f;
	m_bScrollActiveX = true;
	m_bJumpScrollActiveY = false;
	m_bOffsetScrollActiveY = true;
	m_bLeftScrollActive = false;
	m_bRightScrollActive = false;
	m_bAfterSpeed = false;
	m_tStatus.fMaxHp = 150.f;
	m_tStatus.fHp = m_tStatus.fMaxHp;
	m_tStatus.fMaxExp = 100.f;
	m_tStatus.fExp = 0.f;
	m_tStatus.fAttack = 15.f + m_fChietAttack;

	m_tRectInfo[RECT_TYPE::BODY].fcx = 80.f;
	m_tRectInfo[RECT_TYPE::BODY].fcy = 80.f;
	m_tImageSink.fX = 238.f;
	m_tImageSink.fY = 38.f;

	m_bTaxiActive = false;
	m_tFrameTaxi.fCurrentFrame = 0.f;
	m_tFrameTaxi.fMaxFrameCount = 3.f;
	m_tFrameTaxi.fMoveSpeed = 1.f;
	m_bTaxiOK = false;
	m_fChietAttack = 0.f;
	return S_OK;
}

HRESULT CPlayer::LateInit()
{
	return S_OK;
}

void CPlayer::Release()
{
}

void CPlayer::LakeRender()
{
	if (!CScrollMgr::GetLakeRender())
		return;

	m_vLake = { m_tInfo.vPos.x, LAKE_Y - CScrollMgr::GetScrollPos().y, 0.f };
	D3DXVECTOR3 vDist = m_tInfo.vPos - m_vLake;

	D3DXMATRIX matScale, matTrans;
	switch (m_eDir)
	{
	case DIR::RIGHT:
		m_tInfo.vSize = { 1.2f, -1.2f, 0.f };
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - vDist.y, 0.f);
		m_tInfo.matWorld = matScale * matTrans;
		break;
	case DIR::LEFT:
		m_tInfo.vSize = { -1.2f, -1.2f, 0.f };
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - vDist.y, 0.f);
		m_tInfo.matWorld = matScale * matTrans;
		break;
	}
	float fCenterX = float(m_pTexInfo->tImgInfo.Width) * 0.5f;
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - m_tImageSink.fX, fCenterY - m_tImageSink.fY - vDist.y, 0.f), nullptr, D3DCOLOR_ARGB(75, 255, m_iHurtRgb[HURT_RGB::G], m_iHurtRgb[HURT_RGB::B]));
}

void CPlayer::AttackStateActive()
{
	ZeroMemory(&m_tRectInfo[RECT_TYPE::ATTACK].rcRect, sizeof(RECT));
	m_bAttackRectActive = false;

	if (STATE::ATTACK == m_eCurrentState)
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 4.f, 5.f, false);

	if (STATE::SKILL1 == m_eCurrentState)
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 13.f, 14.f, false);

	if (STATE::SKILL2 == m_eCurrentState)
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 10.f, 16.f, false);

	if (STATE::SKILL3 == m_eCurrentState)
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 10.f, 11.f, false);

	if (STATE::SKILL4 == m_eCurrentState)
		CUnit::AttackRectActive(m_tFrame.fCurrentFrame, 18.f, 19.f, false);
}

void CPlayer::FrameCheck()
{
	if (m_eCurrentState == m_eNextState)
		return;

	switch (m_eNextState)
	{
	case STATE::IDLE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 10.f;
		m_tFrame.fMoveSpeed = 1.5f;
		break;
	case STATE::MOVE:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 4.f;
		m_tFrame.fMoveSpeed = 2.f;
		break;
	case STATE::ATTACK:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 9.f;
		m_tFrame.fMoveSpeed = 1.5f;

		m_tRectInfo[RECT_TYPE::ATTACK].fcx = 50.f;
		m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
		m_tStatus.fAttack = 15.f + m_fChietAttack;
		break;
	case STATE::SKILL1:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 22.f;
		m_tFrame.fMoveSpeed = 0.6f;

		m_tRectInfo[RECT_TYPE::ATTACK].fcx = 200.f;
		m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
		m_tStatus.fAttack = 8.f;
		m_bAttackOnlyTarget = true;
		break;
	case STATE::SKILL2:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 21.f;
		m_tFrame.fMoveSpeed = 0.6f;

		m_tRectInfo[RECT_TYPE::ATTACK].fcx = 150.f;
		m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
		m_tStatus.fAttack = 2.f;
		m_bAttackOnlyTarget = true;
		break;
	case STATE::SKILL3:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 17.f;
		m_tFrame.fMoveSpeed = 0.6f;

		m_tRectInfo[RECT_TYPE::ATTACK].fcx = 550.f;
		m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
		m_tStatus.fAttack = 15.f;
		m_bAttackOnlyTarget = true;
		break;
	case STATE::SKILL4:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 30.f;
		m_tFrame.fMoveSpeed = 0.4f;

		m_tRectInfo[RECT_TYPE::ATTACK].fcx = 350.f;
		m_tRectInfo[RECT_TYPE::ATTACK].fcy = 80.f;
		m_tStatus.fAttack = 50.f;
		m_bAttackOnlyTarget = true;
		break;
	case STATE::DEAD:
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 15.f;
		m_tFrame.fMoveSpeed = 0.55f;
		m_tAbility.iDead += 1;

		m_pSoundMgr->StopAll();
		m_pSoundMgr->StopSound(CSoundMgr::CHANNELID::BGM);
		//m_pSoundMgr->MyPlaySound(L"Dead.wav", CSoundMgr::CHANNELID::EFFECT);
		m_pSoundMgr->MyPlaySound(L"death5.wav", CSoundMgr::CHANNELID::EFFECT);
		break;
	}

	m_eCurrentState = m_eNextState;
}

void CPlayer::DirCheck()
{
	D3DXMATRIX matScale, matTrans;

	switch (m_eDir)
	{
	case DIR::RIGHT:
		m_tInfo.vSize.x = 1.2f;
		m_tInfo.vSize.y = 1.2f;
		break;
	case DIR::LEFT:
		m_tInfo.vSize.x = -1.2f;
		m_tInfo.vSize.y = 1.2f;
		break;
	}
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
}

void CPlayer::IsColl()
{
	m_bState[STATE::JUMP] = false;
	m_bState[STATE::JUMP_DOWN] = false;
	m_tJumpInfo.fPower = 950.f;
	m_tJumpInfo.fGravity = 0.f;
	m_bJumpScrollActiveY = false;
	m_bTileDownActive = false;
	m_bMoveTileDownActive = false;
}

void CPlayer::KeyInput()
{
	if (GetAsyncKeyState(VK_TAB))
	{
		m_bScrollActiveX = true;
	}
	if (GetAsyncKeyState('T'))
	{
		m_tInfo.vPos.y = 100.f;
	}

	if (STATE::ATTACK == m_eCurrentState /*|| STATE::SKILL1 == m_eCurrentState
		|| STATE::SKILL2 == m_eCurrentState*/)
	{
		if (0.f == m_tFrame.fCurrentFrame)
			m_bAttackOnlyTarget = true;
	}

	if (STATE::DEAD == m_eNextState || STATE::DEAD == m_eCurrentState || true == m_bState[STATE::TAXI])
		return;

	if (false == m_bState[STATE::ATTACK] && false == m_bState[STATE::SKILL1]
		&& false == m_bState[STATE::SKILL2] && false == m_bState[STATE::SKILL3]
		&& false == m_bState[STATE::SKILL4])
	{
		if(false == m_bState[STATE::JUMP] && false == m_bState[STATE::JUMP_DOWN] && true == m_bIsColl)
			m_eNextState = STATE::IDLE;
		else if(true == m_bState[STATE::JUMP])
			m_eNextState = STATE::IDLE;
		else if (true == m_bState[STATE::JUMP_DOWN] && false == m_bIsColl)
			m_eNextState = STATE::IDLE;
	}
	if(true == m_bState[STATE::JUMP_DOWN] && true == m_bIsColl)
	{ 
		m_eNextState = STATE::IDLE;
		m_bState[STATE::ATTACK] = false;
		m_bState[STATE::SKILL1] = false;
		m_bState[STATE::SKILL2] = false;
		m_bState[STATE::SKILL3] = false;
		m_bState[STATE::SKILL4] = false;
		m_bState[STATE::JUMP_DOWN] = false;
	}
	if(true == m_bLightIn)
		m_eNextState = STATE::MOVE;

	float fTimeDelta = m_pTimeMgr->GetDeltaTime();

	// 왼쪽, 오른쪽 이동
	static DIR eDir = DIR::RIGHT;
	if (STATE::IDLE == m_eCurrentState || STATE::MOVE == m_eCurrentState || STATE::JUMP == m_eCurrentState || STATE::ATTACK == m_eCurrentState)
	{
		if (m_pKeyMgr->KeyPressing(KEY_A) && false == m_bLightIn)
		{
			if(0.f == m_tFrame.fCurrentFrame || 2.f == m_tFrame.fCurrentFrame)
				m_pSoundMgr->MyPlaySound(L"Walk.mp3", CSoundMgr::CHANNELID::EFFECT);

			if (false == CScrollMgr::GetSCrollLockCheck())
				m_bAfterSpeed = true;

			m_eDir = DIR::LEFT;
			if (true == m_bLeftScrollActive)
			{
				m_bScrollActiveX = true;
				m_bLeftScrollActive = false;
			}

			if (STATE::JUMP != m_eCurrentState && true != m_bState[STATE::ATTACK])
				m_eNextState = STATE::MOVE;
			else if (true == m_bState[STATE::ATTACK])
			{
				m_eNextState = STATE::ATTACK;
				Active_AttackState(STATE::ATTACK);
				m_bAfterSpeed = false;
			}

			if (m_tInfo.vPos.x + CScrollMgr::GetScrollPos().x <= WINCX / 2 + 15.f
				|| m_tInfo.vPos.x + CScrollMgr::GetScrollPos().x >= WINCX / 2 + 8450.f)
			{
				m_tInfo.vPos.x -= fTimeDelta * m_fSpeed;
				m_bAfterSpeed = false;
			}
			else if (true == m_bScrollActiveX && STATE::ATTACK != m_eCurrentState
				|| true == m_bScrollActiveX && true == m_bState[STATE::JUMP])
				CScrollMgr::SetScrollPos(D3DXVECTOR3(-m_fSpeed * fTimeDelta, 0.f, 0.f));
			else
				m_bRightScrollActive = true;

			eDir = DIR::LEFT;
			AfterSpeed(eDir, true);
		}
		else if (m_pKeyMgr->KeyPressing(KEY_D) && false == m_bLightIn)
		{
			if (0.f == m_tFrame.fCurrentFrame || 2.f == m_tFrame.fCurrentFrame)
				m_pSoundMgr->MyPlaySound(L"Walk.mp3", CSoundMgr::CHANNELID::EFFECT);

			if (false == CScrollMgr::GetSCrollLockCheck())
				m_bAfterSpeed = true;

			m_eDir = DIR::RIGHT;
			if (true == m_bRightScrollActive)
			{
				m_bScrollActiveX = true;
				m_bRightScrollActive = false;
			}

			if(STATE::JUMP != m_eCurrentState && true != m_bState[STATE::ATTACK])
				m_eNextState = STATE::MOVE;
			else if (true == m_bState[STATE::ATTACK])
			{
				m_eNextState = STATE::ATTACK;
				Active_AttackState(STATE::ATTACK);	
				m_bAfterSpeed = false;
			}

			if (m_tInfo.vPos.x + CScrollMgr::GetScrollPos().x <= WINCX / 2 + 15.f 
				|| m_tInfo.vPos.x + CScrollMgr::GetScrollPos().x >= WINCX / 2 + 8450.f)
			{
				m_tInfo.vPos.x += fTimeDelta * m_fSpeed;
				m_bAfterSpeed = false;
			}
			//m_tInfo.vPos.x += fTimeDelta * m_fSpeed;
			else if (true == m_bScrollActiveX && STATE::ATTACK != m_eCurrentState
				|| true == m_bScrollActiveX && true == m_bState[STATE::JUMP])
				CScrollMgr::SetScrollPos(D3DXVECTOR3(m_fSpeed * fTimeDelta, 0.f, 0.f));
			else
				m_bLeftScrollActive = true;

			eDir = DIR::RIGHT;
			AfterSpeed(eDir, true);
		}
		else if (true == m_bAfterSpeed && true == m_bScrollActiveX && false == CScrollMgr::GetSCrollLockCheck() 
			&& false == m_bLightIn)
			m_bAfterSpeed = AfterSpeed(eDir);

	}

	// 점프
	if (m_pKeyMgr->KeyDown(KEY_W) && false == m_bState[STATE::JUMP] && false == m_bState[STATE::JUMP_DOWN])
	{
		m_pSoundMgr->MyPlaySound(L"Jump.wav", CSoundMgr::CHANNELID::EFFECT);
		//m_eNextState = STATE::JUMP;
		m_bState[STATE::JUMP] = true;
		m_bState[STATE::ATTACK] = false;
		m_bState[STATE::SKILL1] = false;
		m_bIsColl = false;
	}

	// (가능한 지형에서만)아래쪽 이동
	if (m_pKeyMgr->KeyDown(KEY_S))
	{
		if (false == m_bIsCollMoveTile && true == m_bCollUpTileCheck)
		{
			CGameObject* pMap = CObjectMgr::GetInstance()->Get_Obj(OBJECT::MAP).front();

			for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
			{
				if (TILE_TYPE::COLL_UP == tTile->eType)
				{
					RECT rcBody = m_tRectInfo[RECT_TYPE::BODY].rcRect;
					rcBody.bottom += 5;
					RECT rc = {};
					if (IntersectRect(&rc, &rcBody, &tTile->rcRect))
					{
						m_bTileDownActive = true;
						m_tTileDown = tTile;
						break;
					}
				}
			}
		}
		else
		{
			m_bMoveTileDownActive = true;
		}
	}
	if (m_tTileDown)
	{
		RECT rcBody = m_tRectInfo[RECT_TYPE::BODY].rcRect;
		rcBody.bottom += 5;
		RECT rc = {};
		if (!IntersectRect(&rc, &rcBody, &m_tTileDown->rcRect))
		{
			m_bTileDownActive = false;
			m_tTileDown = nullptr;
		}
	}

	// 평타
	if (m_pKeyMgr->KeyPressing(KEY_LBUTTON) 
		&& false == m_bState[STATE::ATTACK] && false == m_bState[STATE::SKILL1]
		&& false == m_bState[STATE::SKILL2] && false == m_bState[STATE::SKILL3]
		&& false == m_bState[STATE::SKILL4])
	{
		m_pSoundMgr->MyPlaySound(L"Attack.wav", CSoundMgr::CHANNELID::EFFECT);
		m_eNextState = STATE::ATTACK;
		Active_AttackState(STATE::ATTACK);
		m_bAfterSpeed = false;
	}

	// 스킬1
	if (m_pKeyMgr->KeyPressing(KEY_RBUTTON)
		&& false == m_bState[STATE::ATTACK] && false == m_bState[STATE::SKILL1]
		&& false == m_bState[STATE::SKILL2] && false == m_bState[STATE::SKILL3]
		&& false == m_bState[STATE::SKILL4] && false == m_bState[STATE::JUMP])
	{
		if (0 != m_tCoolTime[SKILL_TYPE::ONE].fCurrentTime)
		{
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::COOLTIME);
			return;
		}
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skills.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill1.wav", CSoundMgr::CHANNELID::EFFECT);
		m_eNextState = STATE::SKILL1;
		Active_AttackState(STATE::SKILL1);
		m_bAfterSpeed = false;
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::ONE);
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillActive();
		m_tCoolTime[SKILL_TYPE::ONE].fCurrentTime = m_tCoolTime[SKILL_TYPE::ONE].fMaxTime;
	}

	// 스킬2
	else if (m_pKeyMgr->KeyDown(KEY_Q)
		&& false == m_bState[STATE::ATTACK] && false == m_bState[STATE::SKILL1]
		&& false == m_bState[STATE::SKILL2] && false == m_bState[STATE::SKILL3]
		&& false == m_bState[STATE::SKILL4] && false == m_bState[STATE::JUMP])
	{
		if (0 != m_tCoolTime[SKILL_TYPE::TWO].fCurrentTime)
		{
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::COOLTIME);
			return;
		}
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skills.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill2.wav", CSoundMgr::CHANNELID::EFFECT);
		m_eNextState = STATE::SKILL2;
		Active_AttackState(STATE::SKILL2);
		m_bAfterSpeed = false;
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::TWO);
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillActive();
		m_tCoolTime[SKILL_TYPE::TWO].fCurrentTime = m_tCoolTime[SKILL_TYPE::TWO].fMaxTime;
	}
	
	// 스킬3
	else if (m_pKeyMgr->KeyDown(KEY_E)
		&& false == m_bState[STATE::ATTACK] && false == m_bState[STATE::SKILL1]
		&& false == m_bState[STATE::SKILL2] && false == m_bState[STATE::SKILL3]
		&& false == m_bState[STATE::SKILL4] && false == m_bState[STATE::JUMP])
	{
		if (0 != m_tCoolTime[SKILL_TYPE::THREE].fCurrentTime)
		{
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::COOLTIME);
			return;
		}
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skills.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill3.wav", CSoundMgr::CHANNELID::EFFECT);
		m_eNextState = STATE::SKILL3;
		Active_AttackState(STATE::SKILL3);
		m_bAfterSpeed = false;
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::THREE);
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillActive();
		m_tCoolTime[SKILL_TYPE::THREE].fCurrentTime = m_tCoolTime[SKILL_TYPE::THREE].fMaxTime;
	}

	// 스킬4
	else if (m_pKeyMgr->KeyDown(KEY_R)
		&& false == m_bState[STATE::ATTACK] && false == m_bState[STATE::SKILL1]
		&& false == m_bState[STATE::SKILL2] && false == m_bState[STATE::SKILL3]
		&& false == m_bState[STATE::SKILL4] && false == m_bState[STATE::JUMP])
	{
		if (0 != m_tCoolTime[SKILL_TYPE::FOUR].fCurrentTime)
		{
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::COOLTIME);
			return;
		}
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skills.wav", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill4.wav", CSoundMgr::CHANNELID::EFFECT);
		m_eNextState = STATE::SKILL4;
		Active_AttackState(STATE::SKILL4);
		m_bAfterSpeed = false;
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::FOUR);
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillActive();
		m_tCoolTime[SKILL_TYPE::FOUR].fCurrentTime = m_tCoolTime[SKILL_TYPE::FOUR].fMaxTime;
	}

	// 귀한
	if (m_pKeyMgr->KeyDown(KEY_F)
		&& false == m_bState[STATE::ATTACK] && false == m_bState[STATE::SKILL1]
		&& false == m_bState[STATE::SKILL2] && false == m_bState[STATE::SKILL3]
		&& false == m_bState[STATE::SKILL4] && false == m_bState[STATE::JUMP]
		&& false == m_bTaxiActive)
	{
		CSoundMgr::GetInstance()->MyPlaySound(L"Skill.wav", CSoundMgr::CHANNELID::EFFECT);
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillTextActive(SKILL_TYPE::TAXI);
		SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_SkillActive();
		m_bTaxiActive = true;

		m_tInfoTaxi.vPos = { WINCX + 300.f, WINCY - 231.f, 0.f };

		if(m_tInfo.vPos.y < 400.f)
			m_tInfoTaxi.vPos = { WINCX + 300.f, WINCY - 362.f, 0.f };
	}

	//치트키 (체력회복)
	if (m_pKeyMgr->KeyDown(KEY_U))
	{
		if (m_tStatus.fMaxHp > m_tStatus.fHp)
			m_tStatus.fHp += 10.f;
	}

	//치트키 (레벨업)
	if (m_pKeyMgr->KeyDown(KEY_I))
	{
		CUnit::Set_StatusExpUp(40.f);
	}

	//치트키 (공격력업)
	if (m_pKeyMgr->KeyDown(KEY_O))
	{
		m_fChietAttack += 10.f;
	}

	//치트키 (스킬 쿨타임해제)
	if (m_pKeyMgr->KeyDown(KEY_P))
	{
		for (int i = 0; i < SKILL_TYPE::SKILL_END; ++i)
			m_tCoolTime[i].fCurrentTime = 0.f;
	}

}

void CPlayer::IsOffset()
{
	// X좌표 스크롤
	if (false == CScrollMgr::GetSCrollLockCheck())
	{
		if (WINCX / 2 + 5.0f < m_tInfo.vPos.x)
		{
			CScrollMgr::SetScrollPos(D3DXVECTOR3(300.f * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
			m_tInfo.vPos.x += -300.f * m_pTimeMgr->GetDeltaTime();
		}
		else if (WINCX / 2 - 5.f > m_tInfo.vPos.x)
		{
			CScrollMgr::SetScrollPos(D3DXVECTOR3(-300.f * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
			m_tInfo.vPos.x += 300.f * m_pTimeMgr->GetDeltaTime();
		}
	}

	// Y좌표 스크롤
	float fOffset = 480.f; // y좌표의 중점. 

	if (1500.f > CScrollMgr::GetScrollPos().y)
	{
		if (true == m_bJumpScrollActiveY && false == m_bIsCollMoveTile)
		{
			float fScrollY = CScrollMgr::GetScrollPos().y;

			if (WINCY - 100.f < m_tInfo.vPos.y)
			{
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 1000.f * m_pTimeMgr->GetDeltaTime(), 0.f));
				m_tInfo.vPos.y -= 1000.f * m_pTimeMgr->GetDeltaTime();
			}
			else if (fOffset + 50.f < m_tInfo.vPos.y)
			{
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 275.f * m_pTimeMgr->GetDeltaTime(), 0.f));
				m_tInfo.vPos.y -= 275.f * m_pTimeMgr->GetDeltaTime();
			}
			else if (fOffset - 150.f > m_tInfo.vPos.y)
			{
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -300.f * m_pTimeMgr->GetDeltaTime(), 0.f));
				m_tInfo.vPos.y -= -300.f * m_pTimeMgr->GetDeltaTime();
			}
		}
		else if (true == m_bOffsetScrollActiveY && false == m_bIsCollMoveTile)
		{
			float fScrollY = CScrollMgr::GetScrollPos().y;

			D3DXVECTOR3 vOffset = m_tInfo.vPos;
			vOffset.y = fOffset;
			D3DXVECTOR3 vLeng = vOffset - m_tInfo.vPos;
		
			if (3.f < D3DXVec3Length(&vLeng))
			{
				if (fOffset < m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 275.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					m_tInfo.vPos.y -= 275.f * m_pTimeMgr->GetDeltaTime();
				}
				else if (fOffset > m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -300.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					m_tInfo.vPos.y -= -300.f * m_pTimeMgr->GetDeltaTime();
				}
			}
		}
		else if (true == m_bIsCollMoveTile)
		{
			float fScrollY = CScrollMgr::GetScrollPos().y;

			D3DXVECTOR3 vOffset = m_tInfo.vPos;
			vOffset.y = fOffset;
			D3DXVECTOR3 vLeng = vOffset - m_tInfo.vPos;

			if (3.f < D3DXVec3Length(&vLeng))
			{
				if (fOffset < m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 90.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					//m_tInfo.vPos.y -= 275.f * m_pTimeMgr->GetDeltaTime();
				}
				else if (fOffset > m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -90.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					//m_tInfo.vPos.y -= -300.f * m_pTimeMgr->GetDeltaTime();
				}
			}
		}
	}	
	else
	{
		fOffset = 350.f; // y좌표의 중점. 

		if (true == m_bJumpScrollActiveY && false == m_bIsCollMoveTile)
		{
			float fScrollY = CScrollMgr::GetScrollPos().y;

			if (fOffset + 50.f < m_tInfo.vPos.y)
			{
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 400.f * m_pTimeMgr->GetDeltaTime(), 0.f));
				m_tInfo.vPos.y -= 400.f * m_pTimeMgr->GetDeltaTime();
			}
			else if (fOffset - 150.f > m_tInfo.vPos.y)
			{
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -400.f * m_pTimeMgr->GetDeltaTime(), 0.f));
				m_tInfo.vPos.y -= -400.f * m_pTimeMgr->GetDeltaTime();
			}
		}
		else if (true == m_bOffsetScrollActiveY && false == m_bIsCollMoveTile)
		{
			float fScrollY = CScrollMgr::GetScrollPos().y;

			D3DXVECTOR3 vOffset = m_tInfo.vPos;
			vOffset.y = fOffset;
			D3DXVECTOR3 vLeng = vOffset - m_tInfo.vPos;

			if (3.f < D3DXVec3Length(&vLeng))
			{
				if (fOffset < m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 400.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					m_tInfo.vPos.y -= 400.f * m_pTimeMgr->GetDeltaTime();
				}
				else if (fOffset > m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -400.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					m_tInfo.vPos.y -= -400.f * m_pTimeMgr->GetDeltaTime();
				}
			}
		}
		else if (true == m_bIsCollMoveTile)
		{
			float fScrollY = CScrollMgr::GetScrollPos().y;

			D3DXVECTOR3 vOffset = m_tInfo.vPos;
			vOffset.y = fOffset;
			D3DXVECTOR3 vLeng = vOffset - m_tInfo.vPos;

			if (3.f < D3DXVec3Length(&vLeng))
			{
				if (fOffset < m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 90.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					//m_tInfo.vPos.y -= 275.f * m_pTimeMgr->GetDeltaTime();
				}
				else if (fOffset > m_tInfo.vPos.y)
				{
					CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -90.f * m_pTimeMgr->GetDeltaTime(), 0.f));
					//m_tInfo.vPos.y -= -300.f * m_pTimeMgr->GetDeltaTime();
				}
			}
		}
	}

}

void CPlayer::CollMap()
{
	CGameObject* pMap = CObjectMgr::GetInstance()->Get_Obj(OBJECT::MAP).front();

	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	m_bScrollActiveX = true;
	m_bJumpScrollActiveY = true;
	m_bIsColl = false;
	m_bIsCollMoveTile = false;
	bool bCollAllTileCheck = false;
	
	for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tTile->rcRect) && LIGHT_TYPE::CENTER != m_eLightType)
		{
			float fX = float(rc.right - rc.left);
			float fY = float(rc.bottom - rc.top);
			
			// 점프상태 일때
			if (true == m_bState[STATE::JUMP])
			{
				if (TILE_TYPE::COLL_ALL == tTile->eType)
				{
					if (fX > fY)
					{
						if (tTile->vPos.y > m_tInfo.vPos.y + fScrollY)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 1.f;
						}
						else
						{
							m_bState[STATE::JUMP] = false;
							m_tInfo.vPos.y += fY;
						}
					}
					else
					{
						if (tTile->vPos.x > m_tInfo.vPos.x + fScrollX)
							m_tInfo.vPos.x -= fX - 1.f;
						else
							m_tInfo.vPos.x += fX - 1.f;

						m_bScrollActiveX = false;
						m_bAfterSpeed = false;
					}
				}
				if (TILE_TYPE::COLL_UP == tTile->eType)
				{
					if (fX > fY && 0 > m_tJumpInfo.fPower)
					{
						if (tTile->vPos.y > m_tInfo.vPos.y + fScrollY)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 1.f;
						}
					}
				}
			}
			// 점프상태 아닐때
			else if (false == m_bState[STATE::JUMP])
			{
				if (TILE_TYPE::COLL_ALL == tTile->eType)
				{
					if (fX > fY)
					{
						if (tTile->vPos.y > m_tInfo.vPos.y + fScrollY)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 1.f;
						}
						else
						{
							m_bState[STATE::JUMP] = false;
							m_tInfo.vPos.y += fY;
						}
						bCollAllTileCheck = true;
					}
					else
					{
						if (tTile->vPos.x > m_tInfo.vPos.x + fScrollX)
						{
							m_tInfo.vPos.x -= fX - 1.f;

							if (true == m_bState[JUMP_DOWN] && CKeyMgr::GetInstance()->KeyPressing(KEY_A))
								m_bLeftScrollActive = true;
						}
						else
						{
							m_tInfo.vPos.x += fX - 1.f;

							if (true == m_bState[JUMP_DOWN] && CKeyMgr::GetInstance()->KeyPressing(KEY_D))
								m_bRightScrollActive = true;
						}
						if(m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom - 15.f > tTile->rcRect.top)
							m_bScrollActiveX = false;
						m_bAfterSpeed = false;
					}
				}
				if (TILE_TYPE::COLL_UP == tTile->eType && false == m_bTileDownActive)
				{
					if (fX > fY)
					{
						if (tTile->vPos.y > m_tInfo.vPos.y + fScrollY)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 1.f;
						}
			/*			else
						{
							m_bState[STATE::JUMP] = false;
							m_tInfo.vPos.y += fY;
						}*/
						m_bCollUpTileCheck = true;
					}
				}
			}
		}
	}

	if (true == m_bIsColl)
		IsColl();
	if (true == bCollAllTileCheck)
		m_bCollUpTileCheck = false;
}

bool CPlayer::AfterSpeed(DIR _e, bool _b /*= false*/)
{
	static float fAfterSpeed = m_fSpeed;

	if (true == _b)
	{
		fAfterSpeed = m_fSpeed;
		return false;
	}

	fAfterSpeed -= m_pTimeMgr->GetDeltaTime() * 800.f;
	switch (_e)
	{
	case DIR::LEFT:
		CScrollMgr::SetScrollPos(D3DXVECTOR3(-fAfterSpeed * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
		break;
	case DIR::RIGHT:
		CScrollMgr::SetScrollPos(D3DXVECTOR3(fAfterSpeed * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
		break;
	}

	if (0.f >= fAfterSpeed)
	{
		fAfterSpeed = m_fSpeed;
		return false;
	}
	return true;
}

void CPlayer::SkillCoolTimeCheck()
{
	if (0 < m_tCoolTime[SKILL_TYPE::ONE].fCurrentTime)
		m_tCoolTime[SKILL_TYPE::ONE].fCurrentTime -= m_pTimeMgr->GetDeltaTime() * 0.8f;
	else
		m_tCoolTime[SKILL_TYPE::ONE].fCurrentTime = 0.f;

	if (3 <= m_tStatus.iLevel)
	{
		if (0 < m_tCoolTime[SKILL_TYPE::TWO].fCurrentTime)
			m_tCoolTime[SKILL_TYPE::TWO].fCurrentTime -= m_pTimeMgr->GetDeltaTime() * 0.6f;
		else
			m_tCoolTime[SKILL_TYPE::TWO].fCurrentTime = 0.f;
	}

	if (5 <= m_tStatus.iLevel)
	{
		if (0 < m_tCoolTime[SKILL_TYPE::THREE].fCurrentTime)
			m_tCoolTime[SKILL_TYPE::THREE].fCurrentTime -= m_pTimeMgr->GetDeltaTime() * 0.5f;
		else
			m_tCoolTime[SKILL_TYPE::THREE].fCurrentTime = 0.f;
	}

	if (7 <= m_tStatus.iLevel)
	{
		if (0 < m_tCoolTime[SKILL_TYPE::FOUR].fCurrentTime)
			m_tCoolTime[SKILL_TYPE::FOUR].fCurrentTime -= m_pTimeMgr->GetDeltaTime() * 0.4f;
		else
			m_tCoolTime[SKILL_TYPE::FOUR].fCurrentTime = 0.f;
	}
}

void CPlayer::TaxiStateRender()
{
	if (false == m_bTaxiActive)
		return;

	m_tFrameTaxi.fCurrentFrame += m_tFrameTaxi.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * 6.f;

	if (m_tFrameTaxi.fCurrentFrame >= m_tFrameTaxi.fMaxFrameCount)
		m_tFrameTaxi.fCurrentFrame = 0.f;

	m_tInfoTaxi.vPos.x -= 500.f * m_pTimeMgr->GetDeltaTime();

	D3DXMATRIX matWorld;
	D3DXMatrixScaling(&matWorld, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matWorld, m_tInfoTaxi.vPos.x, m_tInfoTaxi.vPos.y, 0.f);

	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, L"Taxi", (int)m_tFrameTaxi.fCurrentFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = float(pTexInfo->tImgInfo.Width) * 0.5f;
	float fCenterY = float(pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_tInfo.vPos.x >= m_tInfoTaxi.vPos.x && false == m_bTaxiOK)
		m_bState[STATE::TAXI] = true;

	if (true == m_bState[STATE::TAXI] && false == m_bTaxiOK)
	{
		if (-150.f > m_tInfoTaxi.vPos.x)
		{
			CScrollMgr::SetScrollTaxiPos();
			m_tInfoTaxi.vPos = { WINCX + 350.f, WINCY - 231.f, 0.f };
			m_tInfo.vPos.y = 480.f;
			m_bTaxiOK = true;
		}
	}
	else if (true == m_bTaxiOK)
	{
		if (m_tInfo.vPos.x >= m_tInfoTaxi.vPos.x)
			m_bState[STATE::TAXI] = false;

		if (-150.f > m_tInfoTaxi.vPos.x)
		{
			m_bTaxiActive = false;
			m_bTaxiOK = false;
		}
	}
}

CPlayer* CPlayer::Create(/*D3DXVECTOR3 _vPos*/ /*= { WINCX / 2 - 45.f, 480.f, 0.f }*/)
{
	CPlayer* pInstance = new CPlayer;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	//pInstance->Set_Pos(_vPos);

	return pInstance;
}

