#include "stdafx.h"
#include "Hero.h"

#include "MyMap.h"

CHero::CHero()
	: m_bTileDownActive(false),
	m_tTileDown(nullptr),
	m_bLightIn(false),
	m_eLightType(LIGHT_TYPE::TYPE_END),
	m_bIsCollMoveTile(false),
	m_bMoveTileDownActive(false),
	m_bCollUpTileCheck(false)
{
	m_eUnitType = UNIT_TYPE::HERO;
	m_bTowerHp = false;
	m_bGameStart = false;
	m_bHealGo = false;

	for (int i = 0; i < SKILL_END; ++i)
	{
		m_tCoolTime[i].fCurrentTime = 10.f;
		m_tCoolTime[i].fMaxTime = 10.f;
	}
	m_tCoolTime[SKILL_TYPE::ONE].fCurrentTime = 0.f;
	m_fDeadCoolTime = 0.f;
}

CHero::~CHero()
{
}

void CHero::HpRender(float _fx/*= 0.f*/, float _fy/*= 0.f*/, bool _bScroll/* = true*/)
{
	if (STATE::DEAD == m_eCurrentState)
		m_fHpAlpha -= m_pTimeMgr->GetDeltaTime() * 200;
	if (5 > m_fHpAlpha)
		m_fHpAlpha = 0;

	D3DXMATRIX matWorld, matScale, matTrans;

	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"HeroHpBack");
	NULL_CHECK(pTexInfo);

	float fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImgInfo.Height * 0.5f);

	if(false == m_bTowerHp)
		D3DXMatrixScaling(&matScale, 1.f, 0.85f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.3f, 1.2f, 0.f);
	if(_bScroll)
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 15.f + _fx - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - 120.f + _fy - CScrollMgr::GetScrollPos().y, 0.f);
	else
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 5.f + _fx, m_tInfo.vPos.y - 120.f + _fy, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_fHpAlpha, 255, 255, 255));

	if(TEAM_TYPE::RED == m_eTeamType)
		pTexInfo = m_pTextureMgr->GetTexInfo(L"RedHeroHp");
	else if(TEAM_TYPE::BLUE == m_eTeamType)
		pTexInfo = m_pTextureMgr->GetTexInfo(L"BlueHeroHp");
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

	pTexInfo = m_pTextureMgr->GetTexInfo(L"HeroHpBar");
	NULL_CHECK(pTexInfo);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_fHpAlpha, 255, 255, 255));
}

void CHero::CollTile()
{
	CGameObject* pMap = m_pObjectMgr->Get_Obj(OBJECT::MAP).front();
	RECT rc = {};

	CGameObject* pMoveTile = nullptr;

	for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
	{
		if (IntersectRect(&rc, &m_tRectInfo[RECT_TYPE::BODY].rcRect, &tTile->rcRect))
		{
			if (false == m_bState[STATE::JUMP] && true == m_bIsColl)
			{
				// 점프타일 충돌
				if (TILE_TYPE::RED_JUMP == tTile->eType
					|| TILE_TYPE::BLUE_JUMP == tTile->eType)
				{
					m_eNextState = STATE::MOVE;
					m_bState[STATE::JUMP] = true;
				}
				// 스탑타일 충돌
				if (m_tAbility.iDead % 2 == 1)
				{
					if (TILE_TYPE::BLUE_STOP == tTile->eType)
					{
						m_bState[STATE::IDLE] = true;
						break;
					}
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

		if (170.f >= D3DXVec3Length(&pDist))
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

			if (1320.f > m_tInfo.vPos.y)
			{
				m_bState[STATE::IDLE] = false;
				m_bState[STATE::JUMP] = true;
			}		
		}
	}
}

void CHero::LightIn()
{
	if (true == m_bLightIn)
	{
		D3DXVECTOR3 vCurrentPos = CScrollMgr::GetScrollPos();
		D3DXVECTOR3 vPoint;
		D3DXVECTOR3 vDir;

		float x = 0.f;
		float y = 0.f;

		static float fCenterJump = 3500.f;

		switch (m_eLightType)
		{
		case LIGHT_TYPE::RED:
			vPoint = { 3250.f, 470.f, 0.f };
			vDir = vPoint - vCurrentPos;
			D3DXVec3Normalize(&vDir, &vDir);
			x = vDir.x * 1300.f;
			y = vDir.y * 2050.f;
			CScrollMgr::SetScrollPos(D3DXVECTOR3(x* m_pTimeMgr->GetDeltaTime(), y* m_pTimeMgr->GetDeltaTime(), 0.f));
			break;
		case LIGHT_TYPE::BLUE:
			vPoint = { 5300.f, 465.f, 0.f };
			vDir = vPoint - vCurrentPos;
			D3DXVec3Normalize(&vDir, &vDir);
			x = vDir.x * 1300.f;
			y = vDir.y * 2050.f;
			CScrollMgr::SetScrollPos(D3DXVECTOR3(x* m_pTimeMgr->GetDeltaTime(), y* m_pTimeMgr->GetDeltaTime(), 0.f));
			break;
		case LIGHT_TYPE::CENTER:
			vPoint = { 4300.f, 0.f, 0.f };
			vDir = vPoint - vCurrentPos;
			D3DXVec3Normalize(&vDir, &vDir);
			fCenterJump += -2900.f * m_pTimeMgr->GetDeltaTime();
			y = vDir.y * fCenterJump;
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, y * m_pTimeMgr->GetDeltaTime(), 0.f));
			if (500 >= fCenterJump)
			{
				m_bLightIn = false;
				m_eLightType = LIGHT_TYPE::TYPE_END;
				fCenterJump = 3500.f;
			}
			break;
		}

		D3DXVECTOR3 vLength = vPoint - vCurrentPos;
		if(20.f > D3DXVec3Length(&vLength) && LIGHT_TYPE::CENTER != m_eLightType)
		{
			m_bLightIn = false;
			m_eLightType = LIGHT_TYPE::TYPE_END;
			fCenterJump = 3500.f;
		}
	}
}

void CHero::DeadRespawn(bool _bScroll)
{
	m_fDeadCoolTime += m_pTimeMgr->GetDeltaTime();

	if (10.0f >= m_fDeadCoolTime)
		return;

	if (false == _bScroll)
	{
		CScrollMgr::SetScrollRespawnPos();
		m_tInfo.vPos.y = 480.f;		

		//m_pSoundMgr->StopAll();
		//m_pSoundMgr->PlayBGM(L"Bgm.mp3");	
	}
	else
	{
		switch (m_eTeamType)
		{
		case TEAM_TYPE::RED:
			break;
		case TEAM_TYPE::BLUE:
			m_tInfo.vPos = { 9505.f, 1160.f, 0.f };
			break;
		}
	}
	m_bState[STATE::DEAD] = false;
	m_eNextState = STATE::IDLE;
	m_tStatus.fHp = m_tStatus.fMaxHp;
	m_fHpAlpha = 255;
	m_fDeadCoolTime = 0.f;
}

void CHero::CollMap()
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
		CHero::IsColl();
}

void CHero::IsColl()
{
	m_bState[STATE::JUMP] = false;
	m_tJumpInfo.fPower = 950.f;
	m_tJumpInfo.fGravity = 0.f;
	m_bTileDownActive = false;
	m_bMoveTileDownActive = false;
}
