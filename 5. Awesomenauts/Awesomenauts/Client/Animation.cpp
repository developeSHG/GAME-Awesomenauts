#include "stdafx.h"
#include "Animation.h"

#include "GameObject.h"
#include "MyMap.h"

CAnimation::CAnimation()
	: m_pTimeMgrAni(CTimeMgr::GetInstance())
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	for (int i = 0; i < STATE::END; ++i)
		m_wstrStateKey[i] = L"";
	m_wstrStateKey[STATE::IDLE] = L"Idle";
	m_wstrStateKey[STATE::MOVE] = L"Move";
	m_wstrStateKey[STATE::JUMP] = L"Jump";
	//m_wstrStateKey[STATE::JUMP_DOWN] = L"Jump";
	m_wstrStateKey[STATE::ATTACK] = L"Attack";
	m_wstrStateKey[STATE::SKILL1] = L"Skill1";
	m_wstrStateKey[STATE::SKILL2] = L"Skill2";
	m_wstrStateKey[STATE::SKILL3] = L"Skill3";
	m_wstrStateKey[STATE::SKILL4] = L"Skill4";
	m_wstrStateKey[STATE::DEAD] = L"Dead";
	m_eCurrentState = STATE::END;
	m_eNextState = STATE::IDLE;
	ZeroMemory(&m_tImageSink, sizeof(IMAGE_SINK));
	ZeroMemory(m_bState, sizeof(m_bState));
}


CAnimation::~CAnimation()
{
}

void CAnimation::FrameMove(const float _fSpeed/* = 1.f*/, const float _fFrameStart/* = 0.f*/)
{
	// 1초에 m_fMaxFrameCount만큼 애니메이션 재생.
	m_tFrame.fCurrentFrame += m_tFrame.fMaxFrameCount * m_pTimeMgrAni->GetDeltaTime() * _fSpeed;

	if (STATE::ATTACK == m_eCurrentState)
		if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
			m_bState[STATE::ATTACK] = false;

	if (STATE::SKILL1 == m_eCurrentState)
		if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
			m_bState[STATE::SKILL1] = false;

	if (STATE::SKILL2 == m_eCurrentState)
		if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
			m_bState[STATE::SKILL2] = false;

	if (STATE::SKILL3 == m_eCurrentState)
		if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
			m_bState[STATE::SKILL3] = false;

	if (STATE::SKILL4 == m_eCurrentState)
		if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
			m_bState[STATE::SKILL4] = false;

	if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
		m_tFrame.fCurrentFrame = _fFrameStart;
}

void CAnimation::FrameMoveDead(const float _fSpeed)
{
	// 1초에 m_fMaxFrameCount만큼 애니메이션 재생.
	m_tFrame.fCurrentFrame += m_tFrame.fMaxFrameCount * m_pTimeMgrAni->GetDeltaTime() * _fSpeed;

	if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
	{
		m_tFrame.fCurrentFrame = 0.f;
		m_bState[STATE::DEAD] = true;
	}
}

void CAnimation::InGravity(INFO& _tInfo, RECT_INFO& _tRectInfo)
{
	//CGameObject* pMap = CObjectMgr::GetInstance()->Get_Obj(OBJECT::MAP).front();

	//float fScrollX = CScrollMgr::GetScrollPos().x;
	//float fScrollY = CScrollMgr::GetScrollPos().y;

	////m_bScrollActiveX = true;
	////m_bJumpScrollActiveY = true;
	//m_bIsColl = false;

	//for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
	//{
	//	RECT rc = {};
	//	if (IntersectRect(&rc, &_tRectInfo.rcRect, &tTile->rcRect))
	//	{
	//		float fX = float(rc.right - rc.left);
	//		float fY = float(rc.bottom - rc.top);

	//		// 점프상태 일때
	//		if (true == m_bState[STATE::JUMP])
	//		{
	//			if (TILE_TYPE::COLL_ALL == tTile->eType)
	//			{
	//				if (fX > fY)
	//				{
	//					if (tTile->vPos.y > _tInfo.vPos.y + fScrollY)
	//					{
	//						m_bIsColl = true;
	//						_tInfo.vPos.y -= fY - 2.f;
	//					}
	//					else
	//					{
	//						m_bState[STATE::JUMP] = false;
	//						_tInfo.vPos.y += fY;
	//					}
	//				}
	//				else
	//				{
	//					if (tTile->vPos.x > _tInfo.vPos.x + fScrollX)
	//						_tInfo.vPos.x -= fX - 1.f;
	//					else
	//						_tInfo.vPos.x += fX - 1.f;

	//					//m_bScrollActiveX = false;
	//				}
	//			}
	//			if (TILE_TYPE::COLL_UP == tTile->eType)
	//			{
	//				if (fX > fY && 0 > m_tJumpInfo.fPower)
	//				{
	//					if (tTile->vPos.y > _tInfo.vPos.y + fScrollY)
	//					{
	//						m_bIsColl = true;
	//						_tInfo.vPos.y -= fY - 2.f;
	//					}
	//					else
	//					{
	//						m_bState[STATE::JUMP] = false;
	//						_tInfo.vPos.y += fY;
	//					}

	//				}
	//			}
	//		}
	//		// 점프상태 아닐때
	//		else if (false == m_bState[STATE::JUMP])
	//		{
	//			if (TILE_TYPE::COLL_ALL == tTile->eType)
	//			{
	//				if (fX > fY)
	//				{
	//					if (tTile->vPos.y > _tInfo.vPos.y + fScrollY)
	//					{
	//						m_bIsColl = true;
	//						_tInfo.vPos.y -= fY - 2.f;
	//					}
	//					else
	//					{
	//						m_bState[STATE::JUMP] = false;
	//						_tInfo.vPos.y += fY;
	//					}
	//				}
	//				else
	//				{
	//					if (tTile->vPos.x > _tInfo.vPos.x + fScrollX)
	//					{
	//						_tInfo.vPos.x -= fX - 1.f;

	//						/*if (STATE::JUMP_DOWN == m_eCurrentState  && CKeyMgr::GetInstance()->KeyPressing(KEY_A))
	//							m_bLeftScrollActive = true;*/
	//					}
	//					else
	//					{
	//						_tInfo.vPos.x += fX - 1.f;

	//						/*if (STATE::JUMP_DOWN == m_eCurrentState && CKeyMgr::GetInstance()->KeyPressing(KEY_D))
	//							m_bRightScrollActive = true;*/
	//					}
	//					//m_bScrollActiveX = false;
	//				}
	//			}
	//			if (TILE_TYPE::COLL_UP == tTile->eType/* && false == m_bTileDownActive*/)
	//			{
	//				if (fX > fY)
	//				{
	//					if (tTile->vPos.y > _tInfo.vPos.y + fScrollY)
	//					{
	//						m_bIsColl = true;
	//						_tInfo.vPos.y -= fY - 2.f;
	//					}
	//					else
	//					{
	//						m_bState[STATE::JUMP] = false;
	//						_tInfo.vPos.y += fY;
	//					}
	//				}
	//				else
	//				{
	//					if (0 > m_tJumpInfo.fPower && false == m_bState[STATE::JUMP] || STATE::JUMP_DOWN == m_eCurrentState)
	//					{
	//						if (tTile->vPos.x > _tInfo.vPos.x + fScrollX)
	//						{
	//							_tInfo.vPos.x -= fX - 1.f;

	//						/*	if (STATE::JUMP_DOWN == m_eCurrentState  && CKeyMgr::GetInstance()->KeyPressing(KEY_A))
	//								m_bLeftScrollActive = true;*/
	//						}
	//						else
	//						{
	//							_tInfo.vPos.x += fX - 1.f;

	//							/*if (STATE::JUMP_DOWN == m_eCurrentState && CKeyMgr::GetInstance()->KeyPressing(KEY_D))
	//								m_bRightScrollActive = true;*/
	//						}
	//					//m_bScrollActiveX = false;
	//					}
	//				}

	//			}
	//		}
	//	}
	//}

	//if (true == m_bIsColl)
	//{
	//	m_bState[STATE::JUMP] = false;
	//	m_tJumpInfo.fPower = 1000.f;
	//	m_tJumpInfo.fGravity = 0.f;
	//	//m_bJumpScrollActiveY = false;
	//}
}

// 몬스터용 (스크롤 관련 지울것)
//void CAnimation::InGravity(INFO& _tInfo, RECT_INFO& _tRectInfo)
//{
//	bool IsColl = false;
//
//	CGameObject* pMap = CObjectMgr::GetInstance()->Get_Obj(OBJECT::MAP).front();
//
//	if (false == m_bState[STATE::JUMP])
//	{
//		for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
//		{
//			if (TILE_TYPE::COLL_UP == tTile->eType && false == m_bTileDownActive)
//			{
//				RECT rc = {};
//				if (IntersectRect(&rc, &_tRectInfo.rcRect, &tTile->rcRect))
//				{
//
//					float fX = float(rc.right - rc.left);
//					float fY = float(rc.bottom - rc.top);
//
//					if (fX > fY)
//					{
//						if (tTile->rcRect.bottom - 5.f < _tInfo.vPos.y)
//							fY *= -1;
//
//						if (tTile->rcRect.bottom > _tInfo.vPos.y)
//						{
//							_tInfo.vPos.y -= fY - 1.f;
//							IsColl = true;
//						}
//					}
//					else
//					{
//						if (0 > m_tJumpInfo.fPower && false == m_bState[STATE::JUMP] || true == m_bState[STATE::JUMP_DOWN])
//						{
//							if (tTile->rcRect.right - 10.f < _tInfo.vPos.x)
//								fX *= -1;
//
//							_tInfo.vPos.x -= fX - 1.f;
//							m_bScrollActiveX = false;
//						}
//					}
//
//				}
//			}
//			else if (TILE_TYPE::COLL_ALL == tTile->eType)
//			{
//				RECT rc = {};
//				if (IntersectRect(&rc, &_tRectInfo.rcRect, &tTile->rcRect))
//				{
//					float fX = float(rc.right - rc.left);
//					float fY = float(rc.bottom - rc.top);
//
//					if (fX > fY)
//					{
//						if (tTile->rcRect.bottom - 5.f < _tInfo.vPos.y)
//							fY *= -1;
//
//						if (tTile->rcRect.bottom > _tInfo.vPos.y)
//						{
//							_tInfo.vPos.y -= fY - 1.f;
//							IsColl = true;
//						}
//					}
//					else
//					{
//						if (tTile->rcRect.right - 10.f < _tInfo.vPos.x)
//							fX *= -1;
//
//						_tInfo.vPos.x -= fX - 1.f;
//						m_bScrollActiveX = false;
//					}
//				}
//			}
//		}
//	}
//
//	if (true == m_bState[STATE::JUMP])
//	{
//		m_tJumpInfo.fPower -= m_pTimeMgrAni->GetDeltaTime() * 2000.f;
//		_tInfo.vPos.y -= m_pTimeMgrAni->GetDeltaTime() * m_tJumpInfo.fPower;
//
//		_tRectInfo.rcRect.left = LONG(_tInfo.vPos.x);
//		_tRectInfo.rcRect.top = LONG(_tInfo.vPos.y - (_tRectInfo.fcy));
//		_tRectInfo.rcRect.right = LONG(_tInfo.vPos.x + (_tRectInfo.fcx));
//		_tRectInfo.rcRect.bottom = LONG(_tInfo.vPos.y);
//
//		for (auto& tTile : SCAST<CMyMap*>(pMap)->Get_Tile())
//		{
//			if (TILE_TYPE::COLL_UP == tTile->eType)
//			{
//				RECT rc = {};
//				if (IntersectRect(&rc, &_tRectInfo.rcRect, &tTile->rcRect))
//				{
//					float fX = float(rc.right - rc.left);
//					float fY = float(rc.bottom - rc.top);
//
//					if (fX > fY)
//					{
//						if (0 > m_tJumpInfo.fPower)
//						{
//							if (tTile->rcRect.bottom - 1.f < _tInfo.vPos.y)
//								fY *= -1;
//
//							_tInfo.vPos.y -= fY - 1.f;
//							IsColl = true;
//							m_bState[STATE::JUMP] = false;
//						}
//					}
//				}
//			}
//			else if (TILE_TYPE::COLL_ALL == tTile->eType)
//			{
//				RECT rc = {};
//				if (IntersectRect(&rc, &_tRectInfo.rcRect, &tTile->rcRect))
//				{
//					float fX = float(rc.right - rc.left);
//					float fY = float(rc.bottom - rc.top);
//
//					if (fX > fY)
//					{
//						if (tTile->rcRect.bottom - 5.f < _tInfo.vPos.y)
//							fY *= -1;
//
//						_tInfo.vPos.y -= fY - 1.f;
//						IsColl = true;
//						m_bState[STATE::JUMP] = false;
//					}
//					else
//					{
//						if (tTile->rcRect.right - 10.f < _tInfo.vPos.x)
//							fX *= -1;
//
//						_tInfo.vPos.x -= fX - 1.f;
//						m_bScrollActiveX = false;
//					}
//				}
//
//			}
//		}
//	}
//	else if (IsColl)
//	{
//		m_tJumpInfo.fPower = 1000.f;
//		m_tJumpInfo.fGravity = 0.f;
//		m_bState[STATE::JUMP_DOWN] = false;
//	}
//	else if (false == m_bState[STATE::JUMP])
//	{
//		m_bState[STATE::JUMP_DOWN] = true;
//		m_tJumpInfo.fGravity += m_pTimeMgrAni->GetDeltaTime() * 800.f;
//		_tInfo.vPos.y += m_pTimeMgrAni->GetDeltaTime() * m_tJumpInfo.fGravity;
//	}
//}
