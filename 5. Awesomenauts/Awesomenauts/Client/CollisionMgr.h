#pragma once
class CGameObject;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void CollisionRect(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);
	static void CollisionRectEX(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);
	static void CollisionSphere(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);

public:
	static bool CheckSphere(CGameObject* pDst, CGameObject* pSrc);
	static bool CheckRect(CGameObject* pDst, CGameObject* pSrc, float* pMoveX, float* pMoveY);

public:
	static void CollLightIn(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);				// 조명 충돌
	static void CollMoveTile(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);				// 무브타일 충돌
	static void CollSliver(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);					// 실버아이템 충돌
	static void CollPosWall(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);				// 실버아이템 충돌
	static void CollAttackHurt(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);				// 
	static void CollElementCreate(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);			// 
	static void CollElementHurt(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);			// 
	static void CollMinionAttack(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);			// 
	static void CollHeroAttack(list<CGameObject*>& rDst, list<CGameObject*>& rSrc);			// 

public:
	static CObjectMgr* m_pObjectMgr;
};

