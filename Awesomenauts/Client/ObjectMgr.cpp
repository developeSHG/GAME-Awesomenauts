#include "stdafx.h"
#include "ObjectMgr.h"

#include "GameObject.h"
#include "Item.h"

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
	: m_pItemMgr(CItemMgr::GetInstance())
{
	m_fTimeCount = 0.f;
}

CObjectMgr::~CObjectMgr()
{
	Release();
}

HRESULT CObjectMgr::AddObject(OBJECT::TYPE eType, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_listObject[eType].push_back(pObject);

	return S_OK;
}

HRESULT CObjectMgr::AddObjectFront(OBJECT::TYPE eType, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_listObject[eType].push_front(pObject);

	return S_OK;
}

void CObjectMgr::DeleteID(OBJECT::TYPE eType)
{
	for (auto& pObj : m_listObject[eType])
		SafeDelete(pObj);

	m_listObject[eType].clear();
}

void CObjectMgr::Update()
{
	// ������ ����
	ItemCreate();

	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		auto iter_begin = m_listObject[i].begin();
		auto iter_end = m_listObject[i].end();

		for (; iter_begin != iter_end; )
		{
			int iEvent = (*iter_begin)->Update();

			if (OBJ_DEAD == iEvent)
			{
			/*	if (OBJECT::TYPE::ITEM == i)
					m_pItemMgr->SetObject(*iter_begin);
				else*/
					SafeDelete(*iter_begin);

				iter_begin = m_listObject[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
}

void CObjectMgr::LateUpdate()
{
	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		for (auto& pGameObject : m_listObject[i])
			pGameObject->LateUpdate();
	}

	// ����ν���
	CCollisionMgr::CollLightIn(m_listObject[OBJECT::TYPE::PLAYER], m_listObject[OBJECT::TYPE::LIGHT]);					

	// ����Ÿ��
	CCollisionMgr::CollMoveTile(m_listObject[OBJECT::TYPE::PLAYER], m_listObject[OBJECT::TYPE::MOVETILE]);				

	// �ǹ�������
	CCollisionMgr::CollSliver(m_listObject[OBJECT::TYPE::ITEM], m_listObject[OBJECT::TYPE::PLAYER]);			
	CCollisionMgr::CollSliver(m_listObject[OBJECT::TYPE::ITEM], m_listObject[OBJECT::TYPE::RED_HERO]);
	CCollisionMgr::CollSliver(m_listObject[OBJECT::TYPE::ITEM], m_listObject[OBJECT::TYPE::BLUE_HERO]);
	CCollisionMgr::CollSliver(m_listObject[OBJECT::TYPE::SPACEITEM], m_listObject[OBJECT::TYPE::INCUBATOR]);			

	// �÷��̾� ���� 
	CCollisionMgr::CollAttackHurt(m_listObject[OBJECT::TYPE::PLAYER], Get_BlueAllList());
	CCollisionMgr::CollAttackHurt(m_listObject[OBJECT::TYPE::PLAYER], Get_JungleAllList());
//	CCollisionMgr::CollAttackHurt(m_listObject[OBJECT::TYPE::PLAYER], m_listObject[OBJECT::TYPE::BLUE_TOWER]);			

	// Ÿ�� �� �浹
	//CCollisionMgr::CollPosWall(m_listObject[OBJECT::TYPE::PLAYER], m_listObject[OBJECT::TYPE::BLUE_TOWER]);			
	CCollisionMgr::CollPosWall(Get_RedUnitList(), m_listObject[OBJECT::TYPE::BLUE_TOWER]);
	CCollisionMgr::CollPosWall(Get_BlueUnitList(), m_listObject[OBJECT::TYPE::RED_TOWER]);

	// ������Ʈ ��������
	CCollisionMgr::CollElementCreate(m_listObject[OBJECT::TYPE::RED_TOWER], Get_BlueUnitList());
	CCollisionMgr::CollElementCreate(m_listObject[OBJECT::TYPE::BLUE_TOWER], Get_RedUnitList());
	CCollisionMgr::CollElementCreate(m_listObject[OBJECT::TYPE::RED_MINION_DIST], Get_BlueAllList());
	CCollisionMgr::CollElementCreate(m_listObject[OBJECT::TYPE::BLUE_MINION_DIST], Get_RedAllList());

	// Ÿ�� ������Ʈ ����	
	CCollisionMgr::CollElementHurt(m_listObject[OBJECT::TYPE::RED_ELEMENT], Get_BlueAllList());
	CCollisionMgr::CollElementHurt(m_listObject[OBJECT::TYPE::BLUE_ELEMENT], Get_RedAllList());

	// �̴Ͼ� ���� ����
	CCollisionMgr::CollMinionAttack(m_listObject[OBJECT::TYPE::RED_MINION_NEAR], Get_BlueAllList());
	CCollisionMgr::CollMinionAttack(m_listObject[OBJECT::TYPE::BLUE_MINION_NEAR], Get_RedAllList());
	CCollisionMgr::CollMinionAttack(m_listObject[OBJECT::TYPE::JUNGLE_MINION_NEAR], m_listObject[OBJECT::TYPE::PLAYER]);

	// ����� ���� ����
	CCollisionMgr::CollHeroAttack(m_listObject[OBJECT::TYPE::RED_HERO], Get_BlueAllList());
	CCollisionMgr::CollHeroAttack(m_listObject[OBJECT::TYPE::BLUE_HERO], Get_RedAllList());
}

void CObjectMgr::Render()
{
	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		for (auto& pGameObject : m_listObject[i])
			pGameObject->Render();
	}
}

list<CGameObject*> CObjectMgr::Get_RedUnitList()
{
	list<CGameObject*> listRedUnit;
	listRedUnit.clear();

	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		if (i == OBJECT::TYPE::RED_MINION_NEAR || i == OBJECT::TYPE::RED_MINION_DIST|| i == OBJECT::TYPE::RED_HERO || i == OBJECT::TYPE::PLAYER)
		{
			for (auto& pGameObject : m_listObject[i])
			{
				listRedUnit.emplace_back(pGameObject);
			}
		}
	}
	return listRedUnit;
}

list<CGameObject*> CObjectMgr::Get_BlueUnitList()
{
	list<CGameObject*> listBlueUnit;
	listBlueUnit.clear();

	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		if (i == OBJECT::TYPE::BLUE_MINION_NEAR || i == OBJECT::TYPE::BLUE_MINION_DIST || i == OBJECT::TYPE::BLUE_HERO)
		{
			for (auto& pGameObject : m_listObject[i])
			{
				listBlueUnit.emplace_back(pGameObject);
			}
		}
	}
	return listBlueUnit;
}

list<CGameObject*> CObjectMgr::Get_RedAllList()
{
	list<CGameObject*> listRedUnit;
	listRedUnit.clear();

	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		if (i == OBJECT::TYPE::RED_MINION_NEAR || i == OBJECT::TYPE::RED_MINION_DIST || i == OBJECT::TYPE::RED_TOWER || i == OBJECT::TYPE::RED_HERO || i == OBJECT::TYPE::PLAYER)
		{
			for (auto& pGameObject : m_listObject[i])
			{
				listRedUnit.emplace_back(pGameObject);
			}
		}
	}
	return listRedUnit;
}

list<CGameObject*> CObjectMgr::Get_BlueAllList()
{
	list<CGameObject*> listBlueUnit;
	listBlueUnit.clear();

	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		if (i == OBJECT::TYPE::BLUE_MINION_NEAR || i == OBJECT::TYPE::BLUE_MINION_DIST || i == OBJECT::TYPE::BLUE_TOWER || i == OBJECT::TYPE::BLUE_HERO)
		{
			for (auto& pGameObject : m_listObject[i])
			{
				listBlueUnit.emplace_back(pGameObject);
			}
		}
	}
	return listBlueUnit;
}

list<CGameObject*> CObjectMgr::Get_JungleAllList()
{
	list<CGameObject*> listJungleUnit;
	listJungleUnit.clear();

	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		if (i == OBJECT::TYPE::JUNGLE_MINION_DIST || i == OBJECT::TYPE::JUNGLE_MINION_NEAR)
		{
			for (auto& pGameObject : m_listObject[i])
			{
				listJungleUnit.emplace_back(pGameObject);
			}
		}
	}
	return listJungleUnit;
}

void CObjectMgr::Release()
{
	for (int i = 0; i < OBJECT::TYPE::TYPE_END; ++i)
	{
		for_each(
			m_listObject[i].begin(),
			m_listObject[i].end(),
			SafeDelete<CGameObject*>);

		m_listObject[i].clear();
	}
}

void CObjectMgr::ItemCreate()
{
	//m_fTimeCount += CTimeMgr::GetInstance()->GetDeltaTime();

	//if (1.5f < m_fTimeCount)
	//{
	//	CItem* pItem = dynamic_cast<CItem*>(m_pItemMgr->GetObject());

	//	if (nullptr == pItem)
	//		return;

	//	pItem->Set_DeadFalse();
	//	pItem->Set_ActingTrue();
	//	pItem->Set_Create(true);
	//	pItem->Set_Hide(false);
	//	AddObject(OBJECT::ITEM, pItem);

	//	m_fTimeCount = 0.f;
	//}
}
