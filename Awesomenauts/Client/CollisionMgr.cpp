#include "stdafx.h"
#include "CollisionMgr.h"

#include "GameObject.h"
#include "Unit.h"
#include "Hero.h"
#include "Minion.h"
#include "Player.h"
#include "Item.h"
#include "Element.h"
#include "Hurt.h"

CObjectMgr* CCollisionMgr::m_pObjectMgr = CObjectMgr::GetInstance();

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionRect(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	//RECT rc = {};
	//for (auto& pDest : rDst)
	//{
	//	for (auto& pSource : rSrc)
	//	{
	//		if (IntersectRect(&rc, pDest->Get_Rect(), pSource->Get_Rect()))
	//		{
	//			pDest->Set_Dead();
	//			pSource->Set_Dead();
	//		}
	//	}
	//}
}

void CCollisionMgr::CollisionRectEX(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	//float fMoveX = 0.f, fMoveY = 0.f;
	//for (auto& pDest : rDst)
	//{
	//	for (auto& pSource : rSrc)
	//	{
	//		if (CheckRect(pDest, pSource, &fMoveX, &fMoveY))
	//		{
	//			float x = pSource->Get_Info().fx;
	//			float y = pSource->Get_Info().fy;

	//			if (fMoveX > fMoveY)
	//			{
	//				if (y < pDest->Get_Info().fy)
	//					fMoveY *= -1.f;

	//				pSource->Set_Pos(x, y + fMoveY);
	//			}
	//			else
	//			{
	//				if (x < pDest->Get_Info().fx)
	//					fMoveX *= -1.f;

	//				pSource->Set_Pos(x + fMoveX, y);
	//			}

	//		}
	//	}
	//}

}

void CCollisionMgr::CollisionSphere(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	//for (auto& pDest : rDst)
	//{
	//	for (auto& pSource : rSrc)
	//	{
	//		if (CheckSphere(pDest, pSource))
	//		{
	//			pDest->Set_Dead();
	//			pSource->Set_Dead();
	//		}
	//	}
	//}
}

bool CCollisionMgr::CheckSphere(CGameObject * pDst, CGameObject * pSrc)
{
	//float fRadiusSum = (pDst->Get_Info().fcx * 0.5f) + (pSrc->Get_Info().fcx * 0.5f);
	//float x = pDst->Get_Info().fx - pSrc->Get_Info().fx;
	//float y = pDst->Get_Info().fy - pSrc->Get_Info().fy;

	//float fDist = sqrtf(x * x + y * y);

	//return fRadiusSum > fDist;
	return false;
}

bool CCollisionMgr::CheckRect(CGameObject * pDst, CGameObject * pSrc, float * pMoveX, float * pMoveY)
{
	//float fRadiusSumX = (pDst->Get_Info().fcx * 0.5f) + (pSrc->Get_Info().fcx * 0.5f);
	//float fRadiusSumY = (pDst->Get_Info().fcy * 0.5f) + (pSrc->Get_Info().fcy * 0.5f);

	//// fabs 절대값 구해주는 함수. 
	//float fDistX = fabs(pDst->Get_Info().fx - pSrc->Get_Info().fx);
	//float fDistY = fabs(pDst->Get_Info().fy - pSrc->Get_Info().fy);

	//if (fRadiusSumX > fDistX && fRadiusSumY > fDistY)
	//{
	//	*pMoveX = fRadiusSumX - fDistX;
	//	*pMoveY = fRadiusSumY - fDistY;
	//	return true;
	//}
	//return false;

	//float fRadiusSumX = (pDst->Get_Info().fcx * 0.5f) + (pSrc->Get_Info().fcx * 0.5f);
	//float fRadiusSumY = (pDst->Get_Info().fcy * 0.5f) + (pSrc->Get_Info().fcy * 0.5f);

	//// fabs 절대값 구해주는 함수. 
	//float fDistX = fabs(pDst->Get_Info().fx - pSrc->Get_Info().fx);
	//float fDistY = fabs(pDst->Get_Info().fy - pSrc->Get_Info().fy);

	//if (fRadiusSumX > fDistX && fRadiusSumY > fDistY)
	//{
	//	*pMoveX = fRadiusSumX - fDistX;
	//	*pMoveY = fRadiusSumY - fDistY;
	//	return true;
	//}
	return false;
}

void CCollisionMgr::CollLightIn(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};

	for (auto& pDest : rDst)
	{
		for (auto& pSource : rSrc)
		{
			if (IntersectRect(&rc, &pDest->Get_RectBodyInfo(), &pSource->Get_RectBodyInfo()))
			{
				CHero::LIGHT_TYPE eType = SCAST<CHero*>(pSource)->Get_LightType();
				DCAST<CHero*>(pDest)->Set_LightIn();
				DCAST<CHero*>(pDest)->Set_LightType(eType);
			}
		}
	}
}

void CCollisionMgr::CollMoveTile(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};

	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	for (auto& pDest : rDst)
	{
		for (auto& pSource : rSrc)
		{
			if (IntersectRect(&rc, &pDest->Get_RectBodyInfo(), &pSource->Get_RectBodyInfo()))
			{
				float fX = float(rc.right - rc.left);
				float fY = float(rc.bottom - rc.top);

				if (false == SCAST<CHero*>(pDest)->Get_MoveTileDownActive())
				{
					// 점프상태 일때
					if (true == SCAST<CUnit*>(pDest)->Get_boolState(CUnit::STATE::JUMP))
					{
						if (fX > fY && 0 > SCAST<CUnit*>(pDest)->Get_JumpInfo().fPower)
						{
							if (pSource->Get_Pos().y > pDest->Get_Pos().y + fScrollY)
							{
								SCAST<CUnit*>(pDest)->Set_IsColl(true);
								SCAST<CHero*>(pDest)->IsColl();
								SCAST<CHero*>(pDest)->Set_IsCollMoveTile(true);
								D3DXVECTOR3 vPos = pDest->Get_Pos();
								vPos.y -= fY - 5.f;
								pDest->Set_Pos(vPos);
							}
						}
					}
					// 점프상태 아닐때
					else if (false == SCAST<CUnit*>(pDest)->Get_boolState(CUnit::STATE::JUMP))
					{
						if (fX > fY)
						{
							if (pSource->Get_Pos().y > pDest->Get_Pos().y + fScrollY)
							{
								SCAST<CUnit*>(pDest)->Set_IsColl(true);
								SCAST<CHero*>(pDest)->IsColl();
								SCAST<CHero*>(pDest)->Set_IsCollMoveTile(true);
								D3DXVECTOR3 vPos = pDest->Get_Pos();
								vPos.y -= fY - 5.f;
								pDest->Set_Pos(vPos);
							}
						}
					}
				}
			}
		}
	}
}

void CCollisionMgr::CollSliver(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};

	for (auto& pDest : rDst)
	{
		for (auto& pSource : rSrc)
		{
			if (IntersectRect(&rc, &pDest->Get_RectBodyInfo(), &pSource->Get_RectBodyInfo()))
			{
				if (false == SCAST<CItem*>(pDest)->Get_CoolTime())
				{
					if (false == SCAST<CItem*>(pDest)->Get_Acting())
					{
						if (m_pObjectMgr->Get_Obj(OBJECT::INCUBATOR).size() && pSource == m_pObjectMgr->Get_Obj(OBJECT::INCUBATOR).front())
						{
							if (ITEM_TYPE::SLIVER == SCAST<CItem*>(pDest)->Get_ItemType())
							{
								SCAST<CUnit*>(m_pObjectMgr->Get_Player())->Set_StatusExpUp(1.f);
								SCAST<CItem*>(pDest)->Set_ActingTrue();
							}
							continue;
						}

						if (ITEM_TYPE::SLIVER == SCAST<CItem*>(pDest)->Get_ItemType())
							SCAST<CUnit*>(pSource)->Set_StatusExpUp(1.f);
						else if(ITEM_TYPE::MEDICIN == SCAST<CItem*>(pDest)->Get_ItemType())
							SCAST<CUnit*>(pSource)->Set_Medicin();
					}
					SCAST<CItem*>(pDest)->Set_ActingTrue();
					
				}
			}	
		}
	}
}

void CCollisionMgr::CollPosWall(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};

	for (auto& pDest : rDst)
	{
		for (auto& pSource : rSrc)
		{
			if (IntersectRect(&rc, &pDest->Get_RectBodyInfo(), &pSource->Get_RectBodyInfo())
				&& true != SCAST<CUnit*>(pSource)->Get_boolState(CUnit::STATE::TAXI)
				&& CUnit::STATE::DEAD != SCAST<CUnit*>(pSource)->Get_CurrentState())
			{
				if (CUnit::STATE::DEAD == SCAST<CUnit*>(pSource)->Get_CurrentState())
					return;

				float fX = float(rc.right - rc.left);

				if (pDest == m_pObjectMgr->Get_Player())
				{
					if (true == SCAST<CPlayer*>(pDest)->Get_boolState(CUnit::STATE::JUMP)
						|| true == SCAST<CPlayer*>(pDest)->Get_boolState(CUnit::STATE::JUMP_DOWN))
					{
						if (pDest->Get_RectBodyInfo().left < pSource->Get_RectBodyInfo().left)
							pDest->Set_PosPlus(D3DXVECTOR3(-fX, 0.f, 0.f));
						else
							pDest->Set_PosPlus(D3DXVECTOR3(fX, 0.f, 0.f));
						return;
					}
					SCAST<CPlayer*>(pDest)->Set_ScrollActiveX(false);
					return;
				}
				if (pDest->Get_RectBodyInfo().left < pSource->Get_RectBodyInfo().left)
					pDest->Set_PosPlus(D3DXVECTOR3(-fX, 0.f, 0.f));
				else
					pDest->Set_PosPlus(D3DXVECTOR3(fX, 0.f, 0.f));
			}
		}
	}
}

void CCollisionMgr::CollAttackHurt(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};

	if (!rSrc.size())
		return;
	auto& iter = rSrc.rbegin();

	for (auto& pDest : rDst)
	{
		for (; iter != rSrc.rend(); ++iter)
		{
			/*if (IntersectRect(&rc, &pDest->Get_RectSkillInfo(), &(*iter)->Get_RectBodyInfo()))
			{
				if (true == SCAST<CUnit*>(pDest)->Get_AttackRectActive())
				{
					SCAST<CUnit*>(pDest)->Set_AttackOnlyTarget(false);

					if (CUnit::STATE::DEAD != SCAST<CUnit*>(*iter)->Get_CurrentState())
						SCAST<CUnit*>(*iter)->Set_Hurt();
					else
						continue;

					SCAST<CUnit*>(*iter)->Set_StatusHpHurt(SCAST<CUnit*>(pDest)->Get_StatusAttack());
					return;
				}
			}*/

			if (IntersectRect(&rc, &pDest->Get_RectAttackInfo(), &(*iter)->Get_RectBodyInfo() )
				&& true != SCAST<CUnit*>(*iter)->Get_boolState(CUnit::STATE::TAXI)
				&& CUnit::STATE::DEAD != SCAST<CUnit*>(*iter)->Get_CurrentState())
			{
				// 스킬상태
				bool bTowerAttack = true;
				if (CUnit::STATE::ATTACK != SCAST<CUnit*>(pDest)->Get_CurrentState())
				{
					for (auto& pObj : m_pObjectMgr->Get_Obj(OBJECT::BLUE_TOWER))
					{
						if (*iter == pObj)
						{
							bTowerAttack = false;
							break;
						}
					}
				}
				if (false == bTowerAttack)
					continue;

				if (true == SCAST<CUnit*>(pDest)->Get_AttackRectActive())
				{
					if(CUnit::STATE::ATTACK == SCAST<CUnit*>(pDest)->Get_CurrentState())
						SCAST<CUnit*>(pDest)->Set_AttackOnlyTarget(false);

					if (CUnit::STATE::DEAD != SCAST<CUnit*>(*iter)->Get_CurrentState())
						SCAST<CUnit*>(*iter)->Set_Hurt();
					else
						continue;

					SCAST<CUnit*>(*iter)->Set_StatusHpHurt(SCAST<CUnit*>(pDest)->Get_StatusAttack());

					if (0 >= SCAST<CUnit*>(*iter)->Get_Status().fHp)
					{
						bool bCheck = false;
						for (auto& pObject : m_pObjectMgr->Get_Obj(OBJECT::BLUE_HERO))
						{
							if (pObject == (*iter))
							{
								SCAST<CHero*>(pDest)->Set_AbilityKillHero();
								bCheck = true;
								break;
							}
						}
						if (false == bCheck)
						{
							for (auto& pTower : m_pObjectMgr->Get_Obj(OBJECT::BLUE_TOWER))
							{
								if (!(pTower == (*iter)))
								{
									SCAST<CHero*>(pDest)->Set_AbilityKillMinion();
									break;
								}
							}
						}
						SCAST<CUnit*>(pDest)->Set_StatusExpUp(SCAST<CUnit*>(*iter)->Get_Status().fExp);
					}

					if (CUnit::STATE::ATTACK == SCAST<CUnit*>(pDest)->Get_CurrentState())
						return;
				}
			}
		}
	}
}

void CCollisionMgr::CollElementCreate(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};
	CGameObject* pTarget = nullptr;
	D3DXVECTOR3 vDist = {};

	for (auto& pDest : rDst)
	{
		for (auto& pSource : rSrc)
		{
			if (IntersectRect(&rc, &pDest->Get_RectObserveInfo(), &pSource->Get_RectBodyInfo())
				&& true != SCAST<CUnit*>(pSource)->Get_boolState(CUnit::STATE::TAXI)
				&& CUnit::STATE::DEAD != SCAST<CUnit*>(pSource)->Get_CurrentState())
			{
				if (CUnit::STATE::DEAD == SCAST<CUnit*>(pSource)->Get_CurrentState())
					continue;

				if (nullptr != SCAST<CUnit*>(pDest)->Get_Target())
					break;

				//if (pSource == m_pObjectMgr->Get_Player())
				//{
				//	//SCAST<CUnit*>(pDest)->Set_bStateActive(CUnit::STATE::ATTACK);
				//	pTarget = pSource;
				//	SCAST<CUnit*>(pDest)->Set_Target(pTarget);
				//	return;
//}

				pTarget = pSource;
				SCAST<CUnit*>(pDest)->Set_Target(pTarget);
				break;
			}
			else
			{
				if (pSource == SCAST<CUnit*>(pDest)->Get_Target() && 0.f == SCAST<CUnit*>(pDest)->Get_CurrentFrame())
					SCAST<CUnit*>(pDest)->Set_Target(nullptr);
			}
		}

		//if (true == bAttackState)
		//{
		//	//SCAST<CUnit*>(pDest)->Set_bStateActive(CUnit::STATE::ATTACK);
		//	SCAST<CUnit*>(pDest)->Set_Target(pTarget);
		//}
		//bAttackState = false;
		//pTarget = nullptr;
	}
}

void CCollisionMgr::CollElementHurt(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};

	for (auto& pDest : rDst)
	{
		for (auto& pSource : rSrc)
		{
			if (IntersectRect(&rc, &pDest->Get_RectBodyInfo(), &pSource->Get_RectBodyInfo())
				&& true != SCAST<CUnit*>(pSource)->Get_boolState(CUnit::STATE::TAXI)
				&& CUnit::STATE::DEAD != SCAST<CUnit*>(pSource)->Get_CurrentState())
			{
				if (true == SCAST<CUnit*>(pDest)->Get_AttackRectActive())
				{
					SCAST<CUnit*>(pDest)->Set_AttackRectActive(false);

					if (CUnit::STATE::DEAD != SCAST<CUnit*>(pSource)->Get_CurrentState())
						SCAST<CUnit*>(pSource)->Set_Hurt();
					SCAST<CElement*>(pDest)->Set_EffectActive();
					SCAST<CUnit*>(pSource)->Set_StatusHpHurt(SCAST<CUnit*>(pDest)->Get_StatusAttack());

					if (pSource == m_pObjectMgr->Get_Player())
						SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_HurtActive();
					return;
				}
			}
		}
	}

}

void CCollisionMgr::CollMinionAttack(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};
	CGameObject* pTarget = nullptr;
	D3DXVECTOR3 vTargetDist = { 0.f, 0.f, 0.f };

	if (!rSrc.size())
		return;
	auto& iter = rSrc.rbegin();

	for (auto& pDest : rDst)
	{
		for (; iter != rSrc.rend(); ++iter)
		{
			if (IntersectRect(&rc, &pDest->Get_RectObserveInfo(), &(*iter)->Get_RectBodyInfo())
				&& true != SCAST<CUnit*>(*iter)->Get_boolState(CUnit::STATE::TAXI)
				&& CUnit::STATE::DEAD != SCAST<CUnit*>(*iter)->Get_CurrentState())
			{
				D3DXVECTOR3 vDist = pDest->Get_Pos() - (*iter)->Get_Pos();
				if ((*iter) == m_pObjectMgr->Get_Player())
				{
					D3DXVECTOR3 vPos = {
						(*iter)->Get_Pos().x + CScrollMgr::GetScrollPos().x,
						(*iter)->Get_Pos().y + CScrollMgr::GetScrollPos().y,
						0.f
					};
					vDist = pDest->Get_Pos() - vPos;
				}

				if (1 > D3DXVec3Length(&vTargetDist) || D3DXVec3Length(&vDist) < D3DXVec3Length(&vTargetDist))
				{
					pTarget = (*iter);
					vTargetDist = vDist;
					SCAST<CUnit*>(pDest)->Set_Target(*iter);
				}
			}
			else
			{
				if ((*iter) == SCAST<CUnit*>(pDest)->Get_Target() && 0.f == SCAST<CUnit*>(pDest)->Get_CurrentFrame())
					SCAST<CUnit*>(pDest)->Set_Target(nullptr);
			}

		}
		iter = rSrc.rbegin();

		if (!pTarget)
			continue;

		if (IntersectRect(&rc, &pDest->Get_RectAttackInfo(), & pTarget->Get_RectBodyInfo()))
		{
			// SCAST<CUnit*>(pDest)->Set_Target(pTarget);
			SCAST<CUnit*>(pDest)->Set_bStateActive(CUnit::STATE::ATTACK);

			if (true == SCAST<CUnit*>(pDest)->Get_AttackRectActive())
			{
				SCAST<CUnit*>(pDest)->Set_AttackOnlyTarget(false);

				if (CUnit::STATE::DEAD != SCAST<CUnit*>(pTarget)->Get_CurrentState())
					SCAST<CUnit*>(pTarget)->Set_Hurt();
				SCAST<CUnit*>(pTarget)->Set_StatusHpHurt(SCAST<CUnit*>(pDest)->Get_StatusAttack());

				if ((pTarget) == m_pObjectMgr->Get_Player())
					SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_HurtActive();
			}
		}
	}
}

void CCollisionMgr::CollHeroAttack(list<CGameObject*>& rDst, list<CGameObject*>& rSrc)
{
	RECT rc = {};
	CGameObject* pTarget = nullptr;
	D3DXVECTOR3 vTargetDist = { 0.f, 0.f, 0.f };

	if (!rSrc.size())
		return;
	auto& iter = rSrc.rbegin();

	for (auto& pDest : rDst)
	{
		for (; iter != rSrc.rend(); ++iter)
		{
			if (IntersectRect(&rc, &pDest->Get_RectObserveInfo(), &(*iter)->Get_RectBodyInfo())
				&& true != SCAST<CUnit*>(*iter)->Get_boolState(CUnit::STATE::TAXI)
				&& CUnit::STATE::DEAD != SCAST<CUnit*>(*iter)->Get_CurrentState())
			{
				D3DXVECTOR3 vDist = pDest->Get_Pos() - (*iter)->Get_Pos();
				if ((*iter) == m_pObjectMgr->Get_Player())
				{
					D3DXVECTOR3 vPos = {
						(*iter)->Get_Pos().x + CScrollMgr::GetScrollPos().x,
						(*iter)->Get_Pos().y + CScrollMgr::GetScrollPos().y,
						0.f
					};
					vDist = pDest->Get_Pos() - vPos;
				}

				if (1 > D3DXVec3Length(&vTargetDist) || D3DXVec3Length(&vDist) < D3DXVec3Length(&vTargetDist))
				{
					pTarget = (*iter);
					vTargetDist = vDist;
					SCAST<CUnit*>(pDest)->Set_Target(*iter);
				}
			}
			else
			{
				if ((*iter) == SCAST<CUnit*>(pDest)->Get_Target() && 0.f == SCAST<CUnit*>(pDest)->Get_CurrentFrame())
					SCAST<CUnit*>(pDest)->Set_Target(nullptr);
			}

		}
		iter = rSrc.rbegin();

		if (!pTarget)
			continue;

		if (IntersectRect(&rc, &pDest->Get_RectAttackInfo(), &pTarget->Get_RectBodyInfo()))
		{
			SCAST<CUnit*>(pDest)->Set_bStateActive(CUnit::STATE::ATTACK);

			if (true == SCAST<CUnit*>(pDest)->Get_AttackRectActive())
			{
				SCAST<CUnit*>(pDest)->Set_AttackOnlyTarget(false);

				if (CUnit::STATE::DEAD != SCAST<CUnit*>(pTarget)->Get_CurrentState())
					SCAST<CUnit*>(pTarget)->Set_Hurt();
				SCAST<CUnit*>(pTarget)->Set_StatusHpHurt(SCAST<CUnit*>(pDest)->Get_StatusAttack());

				if ((pTarget) == m_pObjectMgr->Get_Player())
					SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_HurtActive();

				if (0 >= SCAST<CUnit*>(pTarget)->Get_Status().fHp)
				{
					bool bCheck = false;

					if (TEAM_TYPE::RED == SCAST<CUnit*>(pDest)->Get_TeamType())
					{
						for (auto& pObject : m_pObjectMgr->Get_Obj(OBJECT::BLUE_HERO))
						{
							if (pObject == (pTarget))
							{
								SCAST<CHero*>(pDest)->Set_AbilityKillHero();
								bCheck = true;
								break;
							}
						}
						if (false == bCheck)
						{
							for (auto& pTower : m_pObjectMgr->Get_Obj(OBJECT::BLUE_TOWER))
							{
								if (!(pTower == (pTarget)))
								{
									SCAST<CHero*>(pDest)->Set_AbilityKillMinion();
									break;
								}
							}
						}
						SCAST<CUnit*>(pDest)->Set_StatusExpUp(SCAST<CUnit*>(pTarget)->Get_Status().fExp);
					}
					else if(TEAM_TYPE::BLUE == SCAST<CUnit*>(pDest)->Get_TeamType())
					{
						for (auto& pObject : m_pObjectMgr->Get_Obj(OBJECT::RED_HERO))
						{
							if (pObject == (pTarget))
							{
								SCAST<CHero*>(pDest)->Set_AbilityKillHero();
								bCheck = true;
								break;
							}
						}
						if (false == bCheck)
						{
							for (auto& pTower : m_pObjectMgr->Get_Obj(OBJECT::RED_TOWER))
							{
								if (!(pTower == (pTarget)))
								{
									SCAST<CHero*>(pDest)->Set_AbilityKillMinion();
									break;
								}
							}
						}
						SCAST<CUnit*>(pDest)->Set_StatusExpUp(SCAST<CUnit*>(pTarget)->Get_Status().fExp);
					}
				}
			}
		}
	}
}