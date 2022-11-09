#include "stdafx.h"
#include "ItemMgr.h"

#include "Item.h"

IMPLEMENT_SINGLETON(CItemMgr)

CItemMgr::CItemMgr()
{
}


CItemMgr::~CItemMgr()
{
}

CGameObject * CItemMgr::GetObject()
{
	if (m_ItemPool.empty())
	{
		return CItem::Create(D3DXVECTOR3(-9900.f, -999.f, 0.f), ITEM_TYPE::MEDICIN);
	}
	else
	{
		// Ǯ�� ��Ȱ�� ������ ������Ʈ�� �����Ѵٸ� ��Ȱ��!
		CGameObject* pObject = m_ItemPool.front();
		m_ItemPool.pop_front();
		return pObject;
	}

	return nullptr;
}

void CItemMgr::SetObject(CGameObject * pObject)
{
	// ����� ��ģ ������Ʈ�� Ǯ�� ��´�.
	m_ItemPool.push_back(pObject);
}

void CItemMgr::Release()
{
	for_each(m_ItemPool.begin(), m_ItemPool.end(), SafeDelete<CGameObject*>);
	m_ItemPool.clear();
}
