#include "stdafx.h"
#include "Minion.h"

#include "MyMap.h"
#include "MoveTile.h"

CMinion::CMinion()
{
	m_eAiType = M_AI_TYPE_END;
	m_eUnitType = UNIT_TYPE::MINION;
	m_bCreate = true;
	m_bCreateAni = true;

	m_tCrateFrame.fCurrentFrame = 0.f;
	m_tCrateFrame.fMaxFrameCount = 5.f;
	m_tCrateFrame.fMoveSpeed = 1.7f;
	
	m_tStatus.fExp = 5.f;
	m_fJungleDirCount = 0.f;
	m_fJungleIdleCount = 0.f;
	m_bJungleIdle = false;
}


CMinion::~CMinion()
{
}

void CMinion::HpRender(float _fx/*= 0.f*/, float _fy/*= 0.f*/)
{
	if (STATE::DEAD == m_eCurrentState)
		m_fHpAlpha -= m_pTimeMgr->GetDeltaTime() * 200;
	if (5 > m_fHpAlpha)
		m_fHpAlpha = 0;

	D3DXMATRIX matWorld, matScale, matTrans;

	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"MinionHpBack");
	NULL_CHECK(pTexInfo);

	float fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImgInfo.Height * 0.5f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 0.f + _fx - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - 105.f + _fy - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_fHpAlpha, 255, 255, 255));

	if (TEAM_TYPE::RED == m_eTeamType)
		pTexInfo = m_pTextureMgr->GetTexInfo(L"MinionRedHp");
	else if (TEAM_TYPE::BLUE == m_eTeamType)
		pTexInfo = m_pTextureMgr->GetTexInfo(L"MinionBlueHp");
	else if (TEAM_TYPE::JUNGLE == m_eTeamType)
		pTexInfo = m_pTextureMgr->GetTexInfo(L"MinionJungleHp");
	NULL_CHECK(pTexInfo);

	float fPasent = float(m_tStatus.fHp) / float(m_tStatus.fMaxHp);
	float fWidth = pTexInfo->tImgInfo.Width * fPasent;

	RECT rc = {};
	rc.left = 0.f;
	rc.right = fWidth;
	rc.top = 0.f;
	rc.bottom = pTexInfo->tImgInfo.Height;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_fHpAlpha, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"MinionHpBar");
	NULL_CHECK(pTexInfo);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_fHpAlpha, 255, 255, 255));
}

void CMinion::AISystem()
{
	if (true == m_bCreate)
		return;

	if (true == m_bState[STATE::ATTACK])
	{
		if (0.f == m_tFrame.fCurrentFrame)
		{
			m_bAttackOnlyTarget = true;
		
		}
	}

	switch (m_eTeamType)
	{
	case TEAM_TYPE::RED:
		RedAI();
		break;
	case TEAM_TYPE::BLUE:
		BlueAI();
		break;
	case TEAM_TYPE::JUNGLE:
		JungleAI();
		break;
	}
}

void CMinion::CollMap()
{
	CGameObject* pMap = CObjectMgr::GetInstance()->Get_Obj(OBJECT::MAP).front();

	m_bIsColl = false;
	bool bCollAllTileCheck = false;

	for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
	{
		RECT rc = {};
		if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tTile->rcRect))
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
						if (tTile->vPos.y > m_tInfo.vPos.y)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 2.f;
						}
						else
						{
							m_bState[STATE::JUMP] = false;
							m_tInfo.vPos.y += fY;
						}
					}
					else
					{
						if (tTile->vPos.x > m_tInfo.vPos.x)
							m_tInfo.vPos.x -= fX - 1.f;
						else
							m_tInfo.vPos.x += fX - 1.f;

					}
				}
				if (TILE_TYPE::COLL_UP == tTile->eType)
				{
					if (fX > fY && 0 > m_tJumpInfo.fPower)
					{
						if (tTile->vPos.y > m_tInfo.vPos.y)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 2.f;
						}
						/*			else
						{
						m_bState[STATE::JUMP] = false;
						m_tInfo.vPos.y += fY;
						}*/

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
						if (tTile->vPos.y > m_tInfo.vPos.y)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 2.f;
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
						if (tTile->vPos.x > m_tInfo.vPos.x)
						{
							m_tInfo.vPos.x -= fX - 1.f;
						}
						else
						{
							m_tInfo.vPos.x += fX - 1.f;
						}

					}
				}
				if (TILE_TYPE::COLL_UP == tTile->eType)
				{
					if (fX > fY)
					{
						if (tTile->vPos.y > m_tInfo.vPos.y)
						{
							m_bIsColl = true;
							m_tInfo.vPos.y -= fY - 2.f;
						}
						/*				else
						{
						m_bState[STATE::JUMP] = false;
						m_tInfo.vPos.y += fY;
						}*/
					}
					else
					{
						/*if (0 > m_tJumpInfo.fPower && false == m_bState[STATE::JUMP] || STATE::JUMP_DOWN == m_eCurrentState)
						{
						if (tTile->vPos.x > m_tInfo.vPos.x + fScrollX)
						{
						m_tInfo.vPos.x -= fX - 1.f;

						if (STATE::JUMP_DOWN == m_eCurrentState  && CKeyMgr::GetInstance()->KeyPressing(KEY_A))
						m_bLeftScrollActive = true;
						}
						else
						{
						m_tInfo.vPos.x += fX - 1.f;

						if (STATE::JUMP_DOWN == m_eCurrentState && CKeyMgr::GetInstance()->KeyPressing(KEY_D))
						m_bRightScrollActive = true;
						}
						m_bScrollActiveX = false;
						m_bAfterSpeed = false;
						}*/
					}

				}
			}
		}
	}

	if (true == m_bIsColl)
		CMinion::IsColl();
}

void CMinion::IsColl()
{
	m_bState[STATE::JUMP] = false;
	m_tJumpInfo.fPower = 1010.f;
	m_tJumpInfo.fGravity = 0.f;
}

void CMinion::CreateUpdate()
{
	if (true == m_bCreateAni)
	{
		m_tCrateFrame.fCurrentFrame += m_tCrateFrame.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * m_tCrateFrame.fMoveSpeed;

		if (m_tCrateFrame.fCurrentFrame >= m_tCrateFrame.fMaxFrameCount - 3.5f)
			m_bCreate = false;
		if (m_tCrateFrame.fCurrentFrame >= m_tCrateFrame.fMaxFrameCount)
		{
			m_tCrateFrame.fCurrentFrame = 0.f;
			m_bCreateAni = false;
		}
	}
}

void CMinion::CreateRender()
{
	if (false == m_bCreateAni)
		return;

	D3DXMATRIX matWorld, matScale, matTrans;

	switch (m_eTeamType)
	{
	case TEAM_TYPE::RED:
		D3DXMatrixTranslation(&matTrans, 2505.f - CScrollMgr::GetScrollPos().x, 1685.f - CScrollMgr::GetScrollPos().y, 0.f);
		break;
	case TEAM_TYPE::BLUE:
		D3DXMatrixTranslation(&matTrans, 7450.f - CScrollMgr::GetScrollPos().x, 1685.f - CScrollMgr::GetScrollPos().y, 0.f);
		break;
	case TEAM_TYPE::JUNGLE:
		D3DXMatrixTranslation(&matTrans, 4950.f - CScrollMgr::GetScrollPos().x, 1530.f - CScrollMgr::GetScrollPos().y, 0.f);
		break;
	}

	D3DXMatrixScaling(&matScale, 1.15f, 1.15f, 0.f);		
	matWorld = matScale * matTrans;

	SET_TEX_INFO_MULTI(L"Portal", L"Effect", m_tCrateFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY - 30.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHurtRgb[HURT_RGB::G], m_iHurtRgb[HURT_RGB::B]));
}

void CMinion::RedAI()
{
	if (STATE::DEAD == m_eNextState)
		return; 

	if (false == m_bState[STATE::ATTACK] || nullptr == m_pTarget)
	{
		if (false == m_bState[STATE::JUMP] && true == m_bIsColl && false == m_bState[STATE::IDLE])
			m_eNextState = STATE::MOVE;
		else if (true == m_bState[STATE::JUMP])
			m_eNextState = STATE::MOVE;
		else if (true == m_bState[STATE::IDLE])
			m_eNextState = STATE::IDLE;

		if (!m_pTarget)
			m_eDir = DIR::RIGHT;
		m_tInfo.vSize.x = m_fBornSizeX;
		// 기본 앞으로가기
		if (false == m_bState[STATE::IDLE] && DIR::RIGHT == m_eDir)
		{
			m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * m_fSpeed;
		}
		else if (false == m_bState[STATE::IDLE] && DIR::LEFT == m_eDir)
		{
			m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * -m_fSpeed;
			m_tInfo.vSize.x *= -1.f;
		}
	}
	else if (true == m_bState[STATE::ATTACK])
	{
		m_eNextState = STATE::ATTACK;
	}

	CGameObject* pMap = m_pObjectMgr->Get_Obj(OBJECT::MAP).front();
	RECT rc = {};

	CGameObject* pMoveTile = nullptr;
	switch (m_eAiType)
	{
	case MINION_AI_TYPE::UP:
		for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
		{
			if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tTile->rcRect))
			{
				if (false == m_bState[STATE::JUMP] && true == m_bIsColl)
				{
					// 점프타일 충돌
					if (TILE_TYPE::RED_JUMP == tTile->eType)
					{
						m_eNextState = STATE::MOVE;
						m_bState[STATE::JUMP] = true;
					}
					// 스탑타일 충돌
					if (TILE_TYPE::RED_STOP == tTile->eType)
					{
						m_bState[STATE::IDLE] = true;
						break;
					}
				}
			}
		}

		// 레드1 무브타일
		pMoveTile = m_pObjectMgr->Get_Obj(OBJECT::MOVETILE).front();

		// 무브타일 가까워지면 점프
		if (true == m_bState[STATE::IDLE])
		{
			D3DXVECTOR3 pDist = pMoveTile->Get_Pos() - m_tInfo.vPos;

			if (200.f >= D3DXVec3Length(&pDist))
			{
				m_bState[STATE::IDLE] = false;
				m_bState[STATE::JUMP] = true;
			}
		}

		// 무브타일 충돌
		for (auto& tMoveTile : m_pObjectMgr->Get_Obj(OBJECT::MOVETILE))
		{
			if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tMoveTile->Get_RectBodyInfo()))
			{
				float fY = rc.bottom - rc.top;

				m_bState[STATE::IDLE] = true;
				m_bIsColl = true;
				IsColl();
				m_tInfo.vPos.y -= fY - 5.f;

				if (1370.f > m_tInfo.vPos.y)
				{
					m_bState[STATE::IDLE] = false;
					m_bState[STATE::JUMP] = true;
				}		
			}
		}
		break;
	case MINION_AI_TYPE::DOWN:
		for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
		{
			if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tTile->rcRect))
			{
				if (false == m_bState[STATE::JUMP] && true == m_bIsColl)
				{
					// 점프타일 충돌
					if (TILE_TYPE::RED_JUMP == tTile->eType)
					{
						m_eNextState = STATE::MOVE;
						m_bState[STATE::JUMP] = true;
					}
				}
			}
		}
		break;
	}

	// 점프, 중력상태 시작
	if (true == m_bState[STATE::JUMP])
	{
		m_tJumpInfo.fPower -= m_pTimeMgr->GetDeltaTime() * 2300.f;
		m_tInfo.vPos.y -= m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fPower;
	}
	// 중력적용
	else if (false == m_bState[STATE::JUMP] && false == m_bIsColl) 
	{
		m_eNextState = STATE::MOVE;
		m_tJumpInfo.fPower = 50.f;
		m_tJumpInfo.fGravity += m_pTimeMgr->GetDeltaTime() * 800.f;
		m_tInfo.vPos.y += m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fGravity;
	}
	// 점프, 중력상태 끝
}

void CMinion::BlueAI()
{
	if (STATE::DEAD == m_eNextState)
		return;

	if (false == m_bState[STATE::ATTACK] || nullptr == m_pTarget)
	{
		if (false == m_bState[STATE::JUMP] && true == m_bIsColl && false == m_bState[STATE::IDLE])
			m_eNextState = STATE::MOVE;
		else if (true == m_bState[STATE::JUMP])
			m_eNextState = STATE::MOVE;
		else if (true == m_bState[STATE::IDLE])
			m_eNextState = STATE::IDLE;

		if (!m_pTarget)
			m_eDir = DIR::LEFT;
		m_tInfo.vSize.x = m_fBornSizeX;
		// 기본 앞으로가기
		if (false == m_bState[STATE::IDLE] && DIR::RIGHT == m_eDir)
		{
			m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * m_fSpeed;
			m_tInfo.vSize.x *= -1.f;
		}
		else if (false == m_bState[STATE::IDLE] && DIR::LEFT == m_eDir)
		{
			m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * -m_fSpeed;
		}
	}
	else if (true == m_bState[STATE::ATTACK])
	{
		m_eNextState = STATE::ATTACK;
	}

	CGameObject* pMap = m_pObjectMgr->Get_Obj(OBJECT::MAP).front();
	RECT rc = {};

	CGameObject* pMoveTile = nullptr;
	switch (m_eAiType)
	{
	case MINION_AI_TYPE::UP:
		for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
		{
			if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tTile->rcRect))
			{
				if (false == m_bState[STATE::JUMP] && true == m_bIsColl)
				{
					// 점프타일 충돌
					if (TILE_TYPE::BLUE_JUMP == tTile->eType)
					{
						m_eNextState = STATE::MOVE;
						m_bState[STATE::JUMP] = true;
					}
					// 스탑타일 충돌
					if (TILE_TYPE::BLUE_STOP == tTile->eType)
					{
						m_bState[STATE::IDLE] = true;
						break;
					}
				}
			}
		}

		// 레드1 무브타일
		pMoveTile = m_pObjectMgr->Get_Obj(OBJECT::MOVETILE).back();

		// 무브타일 가까워지면 점프
		if (true == m_bState[STATE::IDLE])
		{
			D3DXVECTOR3 pDist = pMoveTile->Get_Pos() - m_tInfo.vPos;

			if (200.f >= D3DXVec3Length(&pDist))
			{
				m_bState[STATE::IDLE] = false;
				m_bState[STATE::JUMP] = true;
			}
		}

		// 무브타일 충돌
		for (auto& tMoveTile : m_pObjectMgr->Get_Obj(OBJECT::MOVETILE))
		{
			if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tMoveTile->Get_RectBodyInfo()))
			{
				float fY = rc.bottom - rc.top;

				m_bState[STATE::IDLE] = true;
				m_bIsColl = true;
				IsColl();
				m_tInfo.vPos.y -= fY - 5.f;

				if (1370.f > m_tInfo.vPos.y)
				{
					m_bState[STATE::IDLE] = false;
					m_bState[STATE::JUMP] = true;
				}
			}
		}
		break;
	case MINION_AI_TYPE::DOWN:
		for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
		{
			if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tTile->rcRect))
			{
				if (false == m_bState[STATE::JUMP] && true == m_bIsColl)
				{
					// 점프타일 충돌
					if (TILE_TYPE::BLUE_JUMP == tTile->eType)
					{
						m_eNextState = STATE::MOVE;
						m_bState[STATE::JUMP] = true;
					}
				}
			}
		}
		break;
	}

	// 점프상태 시작
	if (true == m_bState[STATE::JUMP])
	{
		m_tJumpInfo.fPower -= m_pTimeMgr->GetDeltaTime() * 2300.f;
		m_tInfo.vPos.y -= m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fPower;
	}
	else if (false == m_bState[STATE::JUMP] && false == m_bIsColl) // 중력적용
	{
		m_eNextState = STATE::MOVE;
		m_tJumpInfo.fPower = 50.f;
		m_tJumpInfo.fGravity += m_pTimeMgr->GetDeltaTime() * 800.f;
		m_tInfo.vPos.y += m_pTimeMgr->GetDeltaTime() * m_tJumpInfo.fGravity;
	}
	// 점프상태 끝
}

void CMinion::JungleAI()
{
	if (STATE::DEAD == m_eNextState)
		return;

	// 기본 앞으로가기
	if (STATE::MOVE == m_eCurrentState && DIR::RIGHT == m_eDir)
		m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * m_fSpeed;
	else if (STATE::MOVE == m_eCurrentState && DIR::LEFT == m_eDir)
		m_tInfo.vPos.x += m_pTimeMgr->GetDeltaTime() * -m_fSpeed;
	
	m_tInfo.vSize.x = m_fBornSizeX;
	if(DIR::RIGHT == m_eDir)
		m_tInfo.vSize.x *= -1.f;

	if (false == m_bState[STATE::ATTACK] || nullptr == m_pTarget)
	{
		if (true == m_bIsColl && false == m_bState[STATE::IDLE])
			m_eNextState = STATE::MOVE;
		else if (true == m_bState[STATE::IDLE])
			m_eNextState = STATE::IDLE;

		m_fJungleDirCount += m_pTimeMgr->GetDeltaTime();
		m_fJungleIdleCount += m_pTimeMgr->GetDeltaTime();

		if (15.f < m_fJungleIdleCount)
		{
			m_bJungleIdle = true;
			m_eNextState = STATE::IDLE;
		}
		if (25.f < m_fJungleIdleCount)
		{
			m_bJungleIdle = false;
			m_fJungleIdleCount = 0.f;
			m_fJungleDirCount = 0.f;
		}
		if (8.5f < m_fJungleDirCount && false == m_bJungleIdle)
		{
			m_eDir = DIR(rand() % int(DIR::DIR_END));
			m_eNextState = STATE::MOVE;
			m_fJungleDirCount = 0.f;
		}
	}
	else if (true == m_bState[STATE::ATTACK])
	{
		m_eNextState = STATE::ATTACK;
	}

	if (4085.f >= m_tInfo.vPos.x)
		m_eDir = DIR::RIGHT;
	else if (5820.f <= m_tInfo.vPos.x)
		m_eDir = DIR::LEFT;
}
