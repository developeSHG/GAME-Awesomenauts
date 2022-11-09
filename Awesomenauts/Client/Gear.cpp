#include "stdafx.h"
#include "Gear.h"


CGear::CGear()
{
}

CGear::~CGear()
{
	Release();
}

int CGear::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	for (auto& value : m_mapFrame)
	{

		if (!lstrcmp(L"One", value.first.c_str()) || !lstrcmp(L"Two", value.first.c_str()))
		{
			value.second += 3.f * m_pTimeMgr->GetDeltaTime() * 3.f;
			if (value.second >= 3.f)
				value.second = 0.f;
		}
		else
		{
			value.second += 4.f * m_pTimeMgr->GetDeltaTime() * 3.f;
			if (value.second >= 4.f)
				value.second = 0.f;
		}
	}

	return NO_EVENT;
}

void CGear::LateUpdate()
{

	for (auto& map : m_mapInfo)
	{
		for (auto& pInfo : map.second)
		{
			SET_MATRIX_WORLD((*pInfo));
		}
	}
	//SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
}

void CGear::Render()
{
	for (auto& map : m_mapInfo)
	{
		for (auto& pInfo : map.second)
		{
			// 컬링 시작
			if (pInfo->vPos.x - 1485.f < CScrollMgr::GetScrollPos().x && pInfo->vPos.x + 120.f > CScrollMgr::GetScrollPos().x
				&& pInfo->vPos.y - 900.f < CScrollMgr::GetScrollPos().y && pInfo->vPos.y + 90.f > CScrollMgr::GetScrollPos().y)
				m_bCulling = true;
			else
				m_bCulling = false;

			if (false == m_bCulling)
				continue;
			// 컬링 끝

			SET_TEX_INFO_MULTI(m_wstrObjectKey, map.first, m_mapFrame[map.first]);
			NULL_CHECK(m_pTexInfo);

			float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

			m_pDeviceMgr->GetSprite()->SetTransform(&pInfo->matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

HRESULT CGear::Initialize()
{
	// 기어생성
	HRESULT hr = LoadFile();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Gear";

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	//m_wstrStateKey[STATE::IDLE];
	m_tFrame.fCurrentFrame = 0.f;
	m_tFrame.fMaxFrameCount = 4.f;

	return S_OK;
}

HRESULT CGear::LateInit()
{
	return S_OK;
}

void CGear::Release()
{
	for_each(m_mapInfo.begin(), m_mapInfo.end(), 
		[](auto& MyPair)
	{
		for (auto& pInfo : MyPair.second)
		{
			SAFE_DELETE(pInfo);
		}
		MyPair.second.clear();
	});
	m_mapInfo.clear();
}

HRESULT CGear::LoadFile()
{
	TILE* m_tTile = nullptr;

	HANDLE hFile = CreateFile(L"../Data/Gear.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Load Failed");
		return E_FAIL;
	}

	TILE* pTile = nullptr;
	DWORD dwByte = 0;

	TCHAR szTitle[MIN_STR] = L"";
	
	while (true)
	{
		ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (!lstrcmp(szTitle, L"GearStart"))
		{
			INFO* pInfo = nullptr;
			TCHAR szStateKey[MIN_STR] = L"";
			while (true)
			{
				pInfo = new INFO;
				ReadFile(hFile, szStateKey, sizeof(szStateKey), &dwByte, nullptr);
				ReadFile(hFile, pInfo, sizeof(INFO), &dwByte, nullptr);
				m_mapInfo[szStateKey].emplace_back(pInfo);
				ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				if (!lstrcmp(szTitle, L"GearEnd"))
					break;
			}
		}
		else
			continue;

		break;
	}

	for (auto& map : m_mapInfo)
	{
		for (auto& pInfo : map.second)
		{
			m_mapFrame.insert({ map.first, 0.f });
		}
	}

	CloseHandle(hFile);
	return S_OK;
}

CGear * CGear::Create()
{
	CGear* pInstance = new CGear;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
