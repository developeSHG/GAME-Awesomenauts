#pragma once

class CGameObject;
class CItemMgr;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	CObjectMgr();
	~CObjectMgr();

public:
	HRESULT AddObject(OBJECT::TYPE eType, CGameObject* pObject);
	HRESULT AddObjectFront(OBJECT::TYPE eType, CGameObject* pObject);
	void DeleteID(OBJECT::TYPE eType);
	void Update();
	void LateUpdate();
	void Render();

public:
	// Get
	list<CGameObject*> Get_RedUnitList();
	list<CGameObject*> Get_BlueUnitList();
	list<CGameObject*> Get_RedAllList();
	list<CGameObject*> Get_BlueAllList();
	list<CGameObject*> Get_JungleAllList();


private:
	void Release();

public:
	void ItemCreate();

public:
	CGameObject* Get_Player() const { return m_listObject[OBJECT::TYPE::PLAYER].front(); }
	CGameObject* Get_Hurt() const { return m_listObject[OBJECT::TYPE::HURT].front(); }
	const list<CGameObject*>* Get_listObj() const { return m_listObject; }
	const list<CGameObject*> Get_Obj(OBJECT::TYPE _eType) const { return m_listObject[_eType]; }

public:
	list<CGameObject*>	m_listObject[OBJECT::TYPE::TYPE_END];
	CItemMgr* m_pItemMgr;
	float m_fTimeCount;
};

