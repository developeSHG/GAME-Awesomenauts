#pragma once
class CItemMgr
{
	DECLARE_SINGLETON(CItemMgr)

private:
	CItemMgr();
	~CItemMgr();

public:
	CGameObject* GetObject();

public:
	void SetObject(CGameObject* pObject);

private:
	void Release();

private:
	list<CGameObject*>	m_ItemPool; // ������Ʈ Ǯ
};