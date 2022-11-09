#include "stdafx.h"
#include "MyMap.h"


CMyMap::CMyMap()
{
}


CMyMap::~CMyMap()
{
	Release();
}

int CMyMap::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	float fTimeDelta = m_pTimeMgr->GetDeltaTime();

	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		CScrollMgr::SetScrollPos(D3DXVECTOR3(-1000.f * fTimeDelta, 0.f, 0.f));
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		CScrollMgr::SetScrollPos(D3DXVECTOR3(1000.f * fTimeDelta, 0.f, 0.f));
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -1000.f * fTimeDelta, 0.f));
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 1000.f * fTimeDelta, 0.f));

	return NO_EVENT;
}

void CMyMap::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
	UpdateTileRect();
}

void CMyMap::Render()
{
	UpdateTileRect();
	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

#ifdef _DEBUG
	m_pDeviceMgr->GetSprite()->End();
	for (auto& pTile : m_listTile)
	{
		D3DXVECTOR2 vList[] = {
			D3DXVECTOR2(float(pTile->rcRect.left), float(pTile->rcRect.top)),
			D3DXVECTOR2(float(pTile->rcRect.right), float(pTile->rcRect.top)),
			D3DXVECTOR2(float(pTile->rcRect.right), float(pTile->rcRect.bottom)),
			D3DXVECTOR2(float(pTile->rcRect.left), float(pTile->rcRect.bottom)),
			D3DXVECTOR2(float(pTile->rcRect.left), float(pTile->rcRect.top))
		};
		if (TILE_TYPE::COLL_ALL == pTile->eType)
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 127, 127, 127));
		else if (TILE_TYPE::COLL_UP == pTile->eType)
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
		else if (TILE_TYPE::RED_STOP == pTile->eType)
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 100, 0));
		else if (TILE_TYPE::RED_JUMP == pTile->eType)
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
		else if (TILE_TYPE::BLUE_STOP == pTile->eType)
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 100, 255));
		else if (TILE_TYPE::BLUE_JUMP == pTile->eType)
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 0, 255));
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	}
#endif
}

HRESULT CMyMap::Initialize()
{
	// 타일생성
	HRESULT hr = CreateTerrain();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
	m_wstrObjectKey = L"Map";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// CAnimation, CMyMap 고유변수 (초기화 변경 필요시 코딩)
	
	return S_OK;
}

void CMyMap::Release()
{
	auto& iter_begin = m_listTile.begin();

	for (; iter_begin != m_listTile.end(); ++iter_begin)
		SAFE_DELETE(*iter_begin);

	m_listTile.clear();
}

HRESULT CMyMap::CreateTerrain()
{
	TILE* m_tTile = nullptr;

	HANDLE hFile = CreateFile(L"../Data/Map.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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

		if (!lstrcmp(szTitle, L"TileStart"))
		{
			TILE* pTile = nullptr;
			while (true)
			{
				pTile = new TILE;
				ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
				m_listTile.push_back(pTile);
				ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				if (!lstrcmp(szTitle, L"TileEnd"))
					break;
			}
		}
		else
			continue;

		break;
	}


	CloseHandle(hFile);
	//ERR_MSG(L"Load Complete", S_OK);

	//m_tTile = new TILE;
	//NULL_CHECK_RETURN(m_tTile, E_FAIL);
	//m_tTile->eType = TILE_TYPE::COLL_ALL;
	//m_tTile->vPos = { 550.f, 600.f, 0.f };
	//m_tTile->vSize = { 900.f, 200.f, 0.f };
	//m_listTile.push_back(m_tTile);

	return S_OK;
}

void CMyMap::UpdateTileRect()
{
	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	for (auto& pTile : m_listTile)
	{
		pTile->rcRect.left = LONG(pTile->vPos.x - pTile->vSize.x / 2.f - fScrollX);
		pTile->rcRect.top = LONG(pTile->vPos.y - pTile->vSize.y / 2.f - fScrollY);
		pTile->rcRect.right = LONG(pTile->vPos.x + pTile->vSize.x / 2.f - fScrollX);
		pTile->rcRect.bottom = LONG(pTile->vPos.y + pTile->vSize.y / 2.f - fScrollY);
	}
}

CMyMap * CMyMap::Create()
{
	CMyMap* pInstance = new CMyMap;
	
	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
