#include "stdafx.h"
#include "Stage.h"

#include "Space.h"
#include "MyMap.h"
#include "Mouse.h"
#include "Player.h"
#include "Status.h"
#include "Minimap.h"
#include "Gear.h"
#include "RedPortal.h"
#include "BluePortal.h"
#include "JunglePortal.h"
#include "HealPortal.h"
#include "Right.h"
#include "Door.h"
#include "JungleDeco.h"
#include "RedLight.h"
#include "BlueLight.h"
#include "CenterLight.h"
#include "Bar.h"
#include "BushDown.h"
#include "BushCenter.h"
#include "MoveTile.h"
#include "Item.h"
#include "Tower.h"
#include "Hurt.h"
#include "Goblin.h"
#include "Shark.h"
#include "Yeti.h"
#include "Saver.h"
#include "StormBreaker.h"
#include "Lion.h"
#include "MyTable.h"
#include "SpaceEnding.h"

CStage::CStage()
	: m_pTimeMgr(CTimeMgr::GetInstance()),
	m_bGameActive(false),
	m_fMinionCreateTimeCount(1.2f),
	m_fJungleCreateTimeCount(0.f),
	m_iMinionWave(MINION_WAVE::ONE),
	m_bUpCreate(false)
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Update()
{
	HRESULT hr;

	if (1950.f < CScrollMgr::GetScrollPos().x)
		m_bGameActive = true;

	if (true == m_bGameActive)
	{
		m_fMinionCreateTimeCount += m_pTimeMgr->GetDeltaTime();
		if (5.f < m_fMinionCreateTimeCount)
		{
			m_fMinionWaveCreateCount += m_pTimeMgr->GetDeltaTime();

			// 饭靛固聪攫 积己
			hr = RedMinionCreate();
			FAILED_CHECK_MSG_RETURN(hr, L"RedMinion Create Failed");

			// 喉风固聪攫 积己
			hr = BlueMinionCreate();
			FAILED_CHECK_MSG_RETURN(hr, L"BlueMinion Create Failed");

			if (MINION_WAVE::WAVE_END == m_iMinionWave)
				m_iMinionWave = MINION_WAVE::ONE;
		}

		m_fJungleCreateTimeCount += m_pTimeMgr->GetDeltaTime();
		if (10.f < m_fJungleCreateTimeCount)
		{
			// 沥臂固聪攫 积己
			hr = JungleMinionCreate();
			FAILED_CHECK_MSG_RETURN(hr, L"JungleMinion Create Failed");

			m_fJungleCreateTimeCount = 0.f;
		}
	}

	m_pObjectMgr->Update();
}

void CStage::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CStage::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CStage::Initialize()	
{
	CGameObject* pGameObject = nullptr;
	HRESULT hr = 0;

	//CSoundMgr::GetInstance()->PlayBGM(L"Bgm.mp3");
	CSoundMgr::GetInstance()->MyPlaySound(L"bgm43.wav", CSoundMgr::CHANNELID::BGM);

	// Space 积己
	pGameObject = CSpace::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Space Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::SPACE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Space AddObject Failed", E_FAIL);

	// SpaceItem 积己
	hr = SpaceItemCreate();
	FAILED_CHECK_MSG_RETURN(hr, L"SpaceItem Create Failed", E_FAIL);

	// Hurt 积己
	pGameObject = CHurt::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Hurt Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::HURT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Hurt AddObject Failed", E_FAIL);

	// Map 积己
	pGameObject = CMyMap::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Map Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MAP, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Map AddObject Failed", E_FAIL);

	// MoveTile 积己
	hr = MoveTileCreate();
	FAILED_CHECK_MSG_RETURN(hr, L"MoveTile Create Failed", E_FAIL);

	// Item 积己
	hr = ItemCreate();
	FAILED_CHECK_MSG_RETURN(hr, L"Item Create Failed", E_FAIL);

	// Gear 积己
	pGameObject = CGear::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Gear Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UNITFRONT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Gear AddObject Failed", E_FAIL);

	// Right 积己
	pGameObject = CRight::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Right Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UIBACK, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Right AddObject Failed", E_FAIL);

	// Door 积己
	pGameObject = CDoor::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Door Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UIBACK, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Door AddObject Failed", E_FAIL);

	// RedPortal 积己
	pGameObject = CRedPortal::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"RedPortal Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::PORTAL, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"RedPortal AddObject Failed", E_FAIL);

	// BluePortal 积己
	pGameObject = CBluePortal::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"BluePortal Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::PORTAL, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"BluePortal AddObject Failed", E_FAIL);

	// JunglePortal 积己
	pGameObject = CJunglePortal::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"JunglePortal Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::PORTAL, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"JunglePortal AddObject Failed", E_FAIL);

	// RedHealPortal 积己
	pGameObject = CHealPortal::Create(D3DXVECTOR3(2082.f, 1490.f, 0.f));
	NULL_CHECK_MSG_RETURN(pGameObject, L"HealPortal Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UNITFRONT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"HealPortal AddObject Failed", E_FAIL);

	// BlueHealPortal 积己
	pGameObject = CHealPortal::Create(D3DXVECTOR3(7842.f, 1490.f, 0.f));
	NULL_CHECK_MSG_RETURN(pGameObject, L"HealPortal Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UNITFRONT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"HealPortal AddObject Failed", E_FAIL);

	// RedLight 积己
	pGameObject = CRedLight::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"RedLight Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::LIGHT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"RedLight AddObject Failed", E_FAIL);

	// BlueLight 积己
	pGameObject = CBlueLight::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"BlueLight Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::LIGHT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"BlueLight AddObject Failed", E_FAIL);

	// CenterLight 积己
	pGameObject = CCenterLight::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"CenterLight Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::LIGHT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"CenterLight AddObject Failed", E_FAIL);

	// CenterLight 积己
	pGameObject = CBar::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Bar Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UIBACK, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Bar AddObject Failed", E_FAIL);

	// JungleDeco 积己
	pGameObject = CJungleDeco::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"JungleDeco Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UNITFRONT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"JungleDeco AddObject Failed", E_FAIL);

	// BushDown 积己
	pGameObject = CBushDown::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"BushDown Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UNITFRONT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"BushDown AddObject Failed", E_FAIL);

	// BushCenter 积己
	pGameObject = CBushCenter::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"BushCenter Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UNITFRONT, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"BushCenter AddObject Failed", E_FAIL);

	// Mouse 积己
	pGameObject = CMouse::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Mouse Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MOUSE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MOUSE AddObject Failed", E_FAIL);

	// Player 积己
	pGameObject = CPlayer::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Player Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::PLAYER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Player AddObject Failed", E_FAIL);

	// Saver 积己 (喉风评)
	pGameObject = CSaver::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Saver Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BLUE_HERO, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Saver AddObject Failed", E_FAIL);

	// Status 积己
	pGameObject = CStatus::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Status Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UI, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Status AddObject Failed", E_FAIL);

	// Minimap 积己
	pGameObject = CMinimap::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Minimap Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::UI, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Minimap AddObject Failed", E_FAIL);

	// Tower 积己
	hr = TowerCreate();
	FAILED_CHECK_MSG_RETURN(hr, L"Tower Create Failed", E_FAIL);

	// MyTable 积己
	pGameObject = CMyTable::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"MyTable Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MYTABLE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MyTable AddObject Failed", E_FAIL);

	// SpaceEnding 积己
	pGameObject = CSpaceEnding::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"SpaceEnding Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::SPACEENDING, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"SpaceEnding AddObject Failed", E_FAIL);

	return S_OK;
}

void CStage::Release()
{
}

HRESULT CStage::MoveTileCreate()
{
	CGameObject* pGameObject = nullptr;
	HRESULT hr = 0;

	// 饭靛1
	pGameObject = CMoveTile::Create(D3DXVECTOR3(3160.f, WINCY + 617.f, 0.f), 1780.f, 3300.f, 225.f);
	NULL_CHECK_MSG_RETURN(pGameObject, L"MoveTile Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MOVETILE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MoveTile AddObject Failed", E_FAIL);

	// 饭靛2
	pGameObject = CMoveTile::Create(D3DXVECTOR3(4274.f, WINCY + 397.f, 0.f), 2850.f, 4350.f, 150.f, 100.f);
	NULL_CHECK_MSG_RETURN(pGameObject, L"MoveTile Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MOVETILE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MoveTile AddObject Failed", E_FAIL);

	// 吝居哭率
	pGameObject = CMoveTile::Create(D3DXVECTOR3(4410.f, WINCY + 925.f, 0.f), 3000.f, 4500.f, 225.f);
	NULL_CHECK_MSG_RETURN(pGameObject, L"MoveTile Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MOVETILE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MoveTile AddObject Failed", E_FAIL);

	// 吝居坷弗率
	pGameObject = CMoveTile::Create(D3DXVECTOR3(5520.f, WINCY + 925.f, 0.f), 4100.f, 5600.f, 225.f);
	NULL_CHECK_MSG_RETURN(pGameObject, L"MoveTile Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MOVETILE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MoveTile AddObject Failed", E_FAIL);

	// 喉风2
	pGameObject = CMoveTile::Create(D3DXVECTOR3(5656.f, WINCY + 397.f, 0.f), 4250.f, 5765.f, 150.f, 100.f);
	NULL_CHECK_MSG_RETURN(pGameObject, L"MoveTile Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MOVETILE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MoveTile AddObject Failed", E_FAIL);

	// 喉风1
	pGameObject = CMoveTile::Create(D3DXVECTOR3(6767.f, WINCY + 617.f, 0.f), 5400.f, 6850.f, 225.f);
	NULL_CHECK_MSG_RETURN(pGameObject, L"MoveTile Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::MOVETILE, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"MoveTile AddObject Failed", E_FAIL);


	return S_OK;
}

HRESULT CStage::ItemCreate()
{
	HANDLE hFile = CreateFile(L"../Data/Item.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Load Failed");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	TCHAR szTitle[MIN_STR] = L"";

	while (true)
	{
		ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (!lstrcmp(szTitle, L"ItemStart"))
		{
			ITEM_INFO* pItem = nullptr;
			CGameObject* pGameObject = nullptr;
			HRESULT hr = 0;

			TCHAR szStateKey[MIN_STR] = L"";
			while (true)
			{
				pItem = new ITEM_INFO;
				ReadFile(hFile, pItem, sizeof(ITEM_INFO), &dwByte, nullptr);

				pGameObject = CItem::Create(pItem->vPos, pItem->eType);
				NULL_CHECK_MSG_RETURN(pGameObject, L"Sliver Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObject(OBJECT::TYPE::ITEM, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"Sliver AddObject Failed", E_FAIL);
				SAFE_DELETE(pItem);
				
				ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				if (!lstrcmp(szTitle, L"ItemEnd"))
					break;
			}
		}
		else
			continue;

		break;
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CStage::SpaceItemCreate()
{
	HANDLE hFile = CreateFile(L"../Data/SpaceItem.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Load Failed");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	TCHAR szTitle[MIN_STR] = L"";

	while (true)
	{
		ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (!lstrcmp(szTitle, L"ItemStart"))
		{
			ITEM_INFO* pItem = nullptr;
			CGameObject* pGameObject = nullptr;
			HRESULT hr = 0;

			TCHAR szStateKey[MIN_STR] = L"";
			while (true)
			{
				pItem = new ITEM_INFO;
				ReadFile(hFile, pItem, sizeof(ITEM_INFO), &dwByte, nullptr);

				//pItem->vPos.x -= SPACE_X;
				pItem->vPos.y -= 6551.f * 3.f;
				pGameObject = CItem::Create(pItem->vPos, pItem->eType);
				NULL_CHECK_MSG_RETURN(pGameObject, L"Sliver Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObject(OBJECT::TYPE::SPACEITEM, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"Sliver AddObject Failed", E_FAIL);
				SAFE_DELETE(pItem);
				
				ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				if (!lstrcmp(szTitle, L"ItemEnd"))
					break;
			}
		}
		else
			continue;

		break;
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CStage::TowerCreate()
{
	CGameObject* pGameObject = nullptr;
	HRESULT hr;

	// 饭靛1
	pGameObject = CTower::Create(D3DXVECTOR3(3580.f, 1898.f, 0.f), TEAM_TYPE::RED, CTower::TOWER_POS::DOWN);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::RED_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	// 饭靛2
	pGameObject = CTower::Create(D3DXVECTOR3(4265.f, 1984.f, 0.f), TEAM_TYPE::RED, CTower::TOWER_POS::DOWN);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::RED_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	// 饭靛3
	pGameObject = CTower::Create(D3DXVECTOR3(3800.f, 1017.f, 0.f), TEAM_TYPE::RED, CTower::TOWER_POS::UP);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::RED_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	// 饭靛4
	pGameObject = CTower::Create(D3DXVECTOR3(4445.f, 1017.f, 0.f), TEAM_TYPE::RED, CTower::TOWER_POS::UP);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::RED_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	// 喉风1
	pGameObject = CTower::Create(D3DXVECTOR3(6350.f, 1898.f, 0.f), TEAM_TYPE::BLUE, CTower::TOWER_POS::DOWN);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BLUE_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	// 喉风2
	pGameObject = CTower::Create(D3DXVECTOR3(5650.f, 1984.f, 0.f), TEAM_TYPE::BLUE, CTower::TOWER_POS::DOWN);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BLUE_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	// 喉风3
	pGameObject = CTower::Create(D3DXVECTOR3(6110.f, 1017.f, 0.f), TEAM_TYPE::BLUE, CTower::TOWER_POS::UP);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BLUE_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	// 喉风4
	pGameObject = CTower::Create(D3DXVECTOR3(5465.f, 1017.f, 0.f), TEAM_TYPE::BLUE, CTower::TOWER_POS::UP);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Tower Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BLUE_TOWER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Tower AddObject Failed", E_FAIL);

	return S_OK;
}

HRESULT CStage::RedMinionCreate()
{
	CGameObject* pGameObject = nullptr;
	HRESULT hr;

	if (0 != m_pObjectMgr->Get_Obj(OBJECT::BLUE_TOWER).size())
	{
		switch (m_iMinionWave)
		{
		case MINION_WAVE::ONE:
			// 饭靛绊喉赴
			if (false == m_bUpCreate)
			{
				pGameObject = CGoblin::Create(TEAM_TYPE::RED, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"RedGoblin Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_NEAR, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"RedGoblin AddObject Failed", E_FAIL);
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CGoblin::Create(TEAM_TYPE::RED, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"RedGoblin Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_NEAR, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"RedGoblin AddObject Failed", E_FAIL);
			break;
		case MINION_WAVE::TWO:
			// 饭靛箕农
			if (false == m_bUpCreate)
			{
				pGameObject = CShark::Create(TEAM_TYPE::RED, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"RedShark Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_DIST, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"RedShark AddObject Failed", E_FAIL);
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CShark::Create(TEAM_TYPE::RED, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"RedShark Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_DIST, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"RedShark AddObject Failed", E_FAIL);
			break;
		case MINION_WAVE::THREE:
			// 饭靛抗萍
			if (false == m_bUpCreate)
			{
				pGameObject = CYeti::Create(TEAM_TYPE::RED, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"RedYeti Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_NEAR, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"RedYeti AddObject Failed", E_FAIL);
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CYeti::Create(TEAM_TYPE::RED, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"RedYeti Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_NEAR, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"RedYeti AddObject Failed", E_FAIL);
			break;
		case MINION_WAVE::FOUR:
			// 饭靛胶盆
			if (false == m_bUpCreate)
			{
				pGameObject = CStormBreaker::Create(TEAM_TYPE::RED, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"RedStorm Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_NEAR, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"RedStorm AddObject Failed", E_FAIL);
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CStormBreaker::Create(TEAM_TYPE::RED, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"RedStorm Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::RED_MINION_NEAR, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"RedStorm AddObject Failed", E_FAIL);
			break;
		}
	}

	return S_OK;
}

HRESULT CStage::BlueMinionCreate()
{
	CGameObject* pGameObject = nullptr;
	HRESULT hr;

	if (0 != m_pObjectMgr->Get_Obj(OBJECT::BLUE_TOWER).size())
	{
		switch (m_iMinionWave)
		{
		case MINION_WAVE::ONE:
			// 喉风绊喉赴
			if (false == m_bUpCreate)
			{
				pGameObject = CGoblin::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"BlueGoblin Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_NEAR, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"BlueGoblin AddObject Failed", E_FAIL);

				m_bUpCreate = true;
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CGoblin::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"RedGoblin Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_NEAR, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"BlueGoblin AddObject Failed", E_FAIL);

			m_fMinionWaveCreateCount = 0.f;
			m_fMinionCreateTimeCount = 0.f;
			m_iMinionWave += 1;
			m_bUpCreate = false;
			break;
		case MINION_WAVE::TWO:
			// 喉风箕农
			if (false == m_bUpCreate)
			{
				pGameObject = CShark::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"BlueShark Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_DIST, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"BlueShark AddObject Failed", E_FAIL);

				m_bUpCreate = true;
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CShark::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"BlueShark Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_DIST, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"BlueShark AddObject Failed", E_FAIL);

			m_fMinionWaveCreateCount = 0.f;
			m_fMinionCreateTimeCount = 0.f;
			m_iMinionWave += 1;
			m_bUpCreate = false;
			break;
		case MINION_WAVE::THREE:
			// 喉风抗萍
			if (false == m_bUpCreate)
			{
				pGameObject = CYeti::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"BlueYeti Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_NEAR, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"BlueYeti AddObject Failed", E_FAIL);

				m_bUpCreate = true;
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CYeti::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"BlueYeti Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_NEAR, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"BlueYeti AddObject Failed", E_FAIL);

			m_fMinionWaveCreateCount = 0.f;
			m_fMinionCreateTimeCount = 0.f;
			m_iMinionWave += 1;
			m_bUpCreate = false;
			break;
		case MINION_WAVE::FOUR:
			// 喉风胶盆
			if (false == m_bUpCreate)
			{
				pGameObject = CStormBreaker::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::UP);
				NULL_CHECK_MSG_RETURN(pGameObject, L"BlueStorm Create Failed", E_FAIL);
				hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_NEAR, pGameObject);
				FAILED_CHECK_MSG_RETURN(hr, L"BlueStorm AddObject Failed", E_FAIL);

				m_bUpCreate = true;
			}

			if (1.f > m_fMinionWaveCreateCount)
				return S_OK;

			pGameObject = CStormBreaker::Create(TEAM_TYPE::BLUE, MINION_AI_TYPE::DOWN);
			NULL_CHECK_MSG_RETURN(pGameObject, L"BlueStorm Create Failed", E_FAIL);
			hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::BLUE_MINION_NEAR, pGameObject);
			FAILED_CHECK_MSG_RETURN(hr, L"BlueStorm AddObject Failed", E_FAIL);

			m_fMinionWaveCreateCount = 0.f;
			m_fMinionCreateTimeCount = 0.f;
			m_iMinionWave += 1;
			m_bUpCreate = false;
			break;
		}
	}

	return S_OK;
}

HRESULT CStage::JungleMinionCreate()
{
	CGameObject* pGameObject = nullptr;
	HRESULT hr;
	
	if (3 == m_pObjectMgr->Get_JungleAllList().size())
		return S_OK;

	// 荤磊
	pGameObject = CLion::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Lion Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObjectFront(OBJECT::TYPE::JUNGLE_MINION_NEAR, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Lion AddObject Failed", E_FAIL);

	return S_OK;
}

CStage* CStage::Create()
{
	CStage* pInstance = new CStage;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
