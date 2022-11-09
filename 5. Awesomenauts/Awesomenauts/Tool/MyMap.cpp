#include "stdafx.h"
#include "MyMap.h"

#include "ToolView.h"

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

	//m_pToolView->GetScrollPos(0)->
	//if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(-500.f, 0.f, 0.f));
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(500.f, 0.f, 0.f));
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -500.f , 0.f));
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 500.f, 0.f));

	return NO_EVENT;
}

void CMyMap::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CMyMap::Render()																																	
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	UpdateTileRect();

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
		if (pTile != m_pToolView->m_pPickTile)
		{
			if(TILE_TYPE::COLL_ALL == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
			else if (TILE_TYPE::COLL_UP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 0, 255));
			else if (TILE_TYPE::RED_STOP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 100, 0));
			else if (TILE_TYPE::RED_JUMP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
			else if (TILE_TYPE::BLUE_STOP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 100, 255));
			else if (TILE_TYPE::BLUE_JUMP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 0, 255));
		}
		else
		{
			//if (TILE_TYPE::COLL_ALL == pTile->eType)
			//	m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(120, 0, 255, 0));
			//else if (TILE_TYPE::COLL_UP == pTile->eType)
			//	m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(120, 0, 0, 255));
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
	}
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CMyMap::MiniRender()
{
	D3DXMATRIX matScale, matTrans;		
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x/7.65f, m_tInfo.vSize.y/3.45f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pDeviceMgr->GetSprite()->End();

	m_pDeviceMgr->GetLine()->Begin();
	UpdateTileRectMini();

	for (auto& pTile : m_listTile)
	{
		D3DXVECTOR2 vList[] = {
			D3DXVECTOR2(float(pTile->rcRect.left), float(pTile->rcRect.top)),
			D3DXVECTOR2(float(pTile->rcRect.right), float(pTile->rcRect.top)),
			D3DXVECTOR2(float(pTile->rcRect.right), float(pTile->rcRect.bottom)),
			D3DXVECTOR2(float(pTile->rcRect.left), float(pTile->rcRect.bottom)),
			D3DXVECTOR2(float(pTile->rcRect.left), float(pTile->rcRect.top))
		};
		if (pTile != m_pToolView->m_pPickTile)
		{
			if (TILE_TYPE::COLL_ALL == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
			else if (TILE_TYPE::COLL_UP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 0, 255));
			else if (TILE_TYPE::RED_STOP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 100, 0));
			else if (TILE_TYPE::RED_JUMP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
			else if (TILE_TYPE::BLUE_STOP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 100, 255));
			else if (TILE_TYPE::BLUE_JUMP == pTile->eType)
				m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 0, 0, 255));
		}
		else
		{
			//if (TILE_TYPE::COLL_ALL == pTile->eType)
			//	m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(120, 0, 255, 0));
			//else if (TILE_TYPE::COLL_UP == pTile->eType)
			//	m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(120, 0, 0, 255));
			m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
	}
	m_pDeviceMgr->GetLine()->End();
}

HRESULT CMyMap::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = {0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
	m_wstrObjectKey = L"Map";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// CAnimation, CMyMap 고유변수 (초기화 변경 필요시 코딩)
	

	HRESULT hr = CreateTerrain();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}

void CMyMap::Release()
{
	//auto& iter_begin = m_listTile.begin();

	//for (; iter_begin != m_listTile.end(); ++iter_begin)
	//	SAFE_DELETE(*iter_begin);

	for (auto& pTile : m_listTile)
		SAFE_DELETE(pTile);
	m_listTile.clear();
}

void CMyMap::SetToolView(CToolView * pView)
{
	m_pToolView = pView;
}

HRESULT CMyMap::CreateTerrain()
{
	//TILE* m_tTile = nullptr;

	//m_tTile = new TILE;
	//NULL_CHECK_RETURN(m_tTile, E_FAIL);
	//m_tTile->eType = TILE_TYPE::COLL_ALL;
	//m_tTile->rcRect = {100, 500, 1000, 700};
	//m_listTile.push_back(m_tTile);

	//m_tTile = new TILE;
	//m_tTile->eType = TILE_TYPE::COLL_ALL;
	//m_tTile->rcRect = { 500, 300, 800, 450 };
	//m_listTile.push_back(m_tTile);

	//m_tTile = new TILE;
	//m_tTile->eType = TILE_TYPE::COLL_UP;
	//m_tTile->rcRect = { 200, 100, 300, 130 };
	//m_listTile.push_back(m_tTile);

	//m_tTile = new TILE;
	//m_tTile->eType = TILE_TYPE::COLL_ALL;
	//m_tTile->rcRect = { 500, 100, 800, 130 };
	//m_listTile.push_back(m_tTile);

	return S_OK;
}

void CMyMap::UpdateTileRect()
{
	for (auto& pTile : m_listTile)
	{
		pTile->rcRect.left = LONG(pTile->vPos.x - pTile->vSize.x / 2.f - m_pToolView->GetScrollPos(0));
		pTile->rcRect.top = LONG(pTile->vPos.y - pTile->vSize.y / 2.f - m_pToolView->GetScrollPos(1));
		pTile->rcRect.right = LONG(pTile->vPos.x + pTile->vSize.x / 2.f - m_pToolView->GetScrollPos(0));
		pTile->rcRect.bottom = LONG(pTile->vPos.y + pTile->vSize.y / 2.f - m_pToolView->GetScrollPos(1));
	}
}

void CMyMap::UpdateTileRectMini()
{
	for (auto& pTile : m_listTile)
	{
		pTile->rcRect.left = LONG((pTile->vPos.x - pTile->vSize.x / 2.f) / 7.65f);
		pTile->rcRect.top = LONG((pTile->vPos.y - pTile->vSize.y / 2.f) / 3.45f);
		pTile->rcRect.right = LONG((pTile->vPos.x + pTile->vSize.x / 2.f) / 7.65f);
		pTile->rcRect.bottom = LONG((pTile->vPos.y + pTile->vSize.y / 2.f) / 3.45f);
	}
}

CMyMap * CMyMap::Create(CToolView* pView)
{
	CMyMap* pInstance = new CMyMap;
	
	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetToolView(pView);

	return pInstance;
}
