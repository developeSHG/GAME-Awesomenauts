
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"
#include "MyMap.h"
#include "MapTool.h"
#include "Gear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pMap(nullptr),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pMapTool = nullptr;
	m_pMiniView = nullptr;
	m_pPickTile = nullptr;
	m_pPickItem = nullptr;
	m_pGear = nullptr;
	m_bPickMove = false;
}

CToolView::~CToolView()
{
	for (auto& pItem : m_listItem)
	{
		SAFE_DELETE(pItem);
	}
	m_listItem.clear();
	SafeDelete(m_pMap);
	SafeDelete(m_pGear);
	CTextureMgr::GetInstance()->DestroyInstance();
	CDeviceMgr::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	//pDC->Rectangle(0, 0, 200, 200);

	CDeviceMgr::GetInstance()->Render_Begin();

	NULL_CHECK_MSG(m_pMap, L"Map is nullptr");
	m_pMap->Render();

	//SpaceRender();

	NULL_CHECK_MSG(m_pGear, L"Gear is nullptr");
	m_pGear->Render();

	TEX_INFO* pTexInfo = nullptr;
	D3DXMATRIX matWorld, matScale, matTrans;
	for (auto& pItem : m_listItem)
	{
		 if (ITEM_TYPE::MEDICIN == pItem->eType)
			pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"Medison");
		else if (ITEM_TYPE::SLIVER == pItem->eType)
			pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"SliverTool");

		NULL_CHECK(pTexInfo);
		
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);		
		D3DXMatrixTranslation(&matTrans, pItem->vPos.x -GetScrollPos(0), pItem->vPos.y - GetScrollPos(1), 0.f);
		matWorld = matScale * matTrans;

		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
		if (pItem == m_pPickItem)
			CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(130, 255, 255, 255));
		else
			CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	CDeviceMgr::GetInstance()->Render_End();
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 스크롤 크기 지정
	//CScrollView::SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY / 2 * TILEY));
	CScrollView::SetScrollSizes(MM_TEXT, CSize(int(7000.f * 1.4f), int(1800.f * 1.4f)));
	//CScrollView::SetScrollSizes(MM_TEXT, CSize(int(7000.f * 1.4f), int(6551.f * 3.f)));

	// View의 크기와 후면버퍼의 크기 비율을 1:1로 맞춤.
	// View의 크기를 800, 600으로 맞춰보자!
	// View의 크기를 조정하기 위해선 FrameWnd의 크기를 조정하면 된다.

	// AfxGetApp함수: WinApp의 포인터를 반환하는 MFC 전역함수.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is nullptr");

	// GetWindowRect함수: 테두리를 포함한 전체 윈도우 렉트를 얻어옴. 이 때 좌표는 스크린 좌표 기준.
	// GetClientRect함수: 테두리를 미포함한 클라이언트 영역을 얻어옴. 이 때 좌표는 클라이언트 좌표 기준.
	RECT rcFrame = {};
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	// 현재 프레임윈도우와 뷰의 x, y 간격.
	int iCX = rcFrame.right - rcView.right;
	int iCY = rcFrame.bottom - rcView.bottom;

	// FrameWnd 위치, 크기 재조정.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iCX, WINCY + iCY, SWP_NOZORDER);

	HRESULT hr = 0;

	hr = CDeviceMgr::GetInstance()->InitDevice(
		m_hWnd, WINCX, WINCY, CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	hr = CTextureMgr::GetInstance()->LoadTextureFromImgPathFile(L"../Data/ImgPath.txt");
	FAILED_CHECK_MSG(hr, L"LoadTextureFromImgPathFile Failed");

	m_pMap = CMyMap::Create(this);
	NULL_CHECK_MSG(m_pMap, L"MyMap Create Failed");

	m_pGear = CGear::Create(this);
	NULL_CHECK_MSG(m_pMap, L"MyMap Create Failed");

	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));
	NULL_CHECK(pFormView);

	// MapTool 갱신
	m_pMapTool = &pFormView->m_MapTool;

	// MiniView 갱신
	m_pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnLButtonDown(nFlags, point);

	D3DXVECTOR3 vMouse = 
	{ 
		float(point.x) + CScrollView::GetScrollPos(0), 
		float(point.y) + CScrollView::GetScrollPos(1),
		0.f 
	};

	if (true == m_bPickMove)
	{
		if (m_pPickTile)
		{
			m_pPickTile->vPos = { vMouse.x, vMouse.y, 0.f };
			m_bPickMove = false;
			m_pPickTile = nullptr;

		}
		else if (m_pPickGear)
		{
			m_pPickGear->vPos = { vMouse.x, vMouse.y, 0.f };
			m_bPickMove = false;
			m_pPickGear = nullptr;
		}
		else if (m_pPickItem)
		{
			m_pPickItem->vPos = { vMouse.x, vMouse.y, 0.f };
			m_bPickMove = false;
			m_pPickItem = nullptr;
		}
	}
	else if (false == m_bPickMove)
	{
		for (auto& pTile : m_pMap->m_listTile)
		{
			if (pTile->rcRect.left + CScrollView::GetScrollPos(0) <= vMouse.x && pTile->rcRect.right + CScrollView::GetScrollPos(0) >= vMouse.x
				&& pTile->rcRect.top + CScrollView::GetScrollPos(1) <= vMouse.y && pTile->rcRect.bottom + CScrollView::GetScrollPos(1) >= vMouse.y)
			{
				m_pMapTool->DisableXY(false);
				m_pPickTile = pTile;
				m_pMapTool->UpdateData(TRUE);
				m_pMapTool->m_fX = pTile->vPos.x;
				m_pMapTool->m_fY = pTile->vPos.y;
				m_pMapTool->m_fSizeX = pTile->vSize.x;
				m_pMapTool->m_fSizeY = pTile->vSize.y;
				m_pMapTool->UpdateData(FALSE);
			}
		}
		for (auto& pItem : m_listItem)
		{
			float fLeft = pItem->vPos.x - 47.f;
			float fRight = pItem->vPos.x + 47.f;
			float fTop = pItem->vPos.y - 52.5f;
			float fBottom = pItem->vPos.y + 52.5f;
			if (fLeft <= vMouse.x && fRight >= vMouse.x
				&& fTop <= vMouse.y && fBottom >= vMouse.y)
			{
				m_pMapTool->DisableXY(false);
				m_pMapTool->m_fX = pItem->vPos.x;
				m_pMapTool->m_fY = pItem->vPos.y;
				m_pMapTool->UpdateData(FALSE);

				m_pPickItem = pItem;
			}
		}
	}


	// MapTool 업데이트
	NULL_CHECK(m_pMapTool);
	m_pMapTool->Invalidate(FALSE);

	// MiniView 업데이트
	NULL_CHECK(m_pMiniView);
	m_pMiniView->Invalidate(FALSE);

	Invalidate(FALSE);
}

// 마우스가 움직일 때마다 호출될 함수.
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	D3DXVECTOR3 vMouse =
	{
		float(point.x) + CScrollView::GetScrollPos(0),
		float(point.y) + CScrollView::GetScrollPos(1),
		0.f
	};

	if (true == m_bPickMove)
	{
		if (m_pPickTile)
		{
			m_pPickTile->vPos = { vMouse.x, vMouse.y, 0.f };
			m_pMap->UpdateTileRect();
			m_pMapTool->UpdateData(TRUE);
			m_pMapTool->m_fX = m_pPickTile->vPos.x;
			m_pMapTool->m_fY = m_pPickTile->vPos.y;
			m_pMapTool->UpdateData(FALSE);
		}
		else if (m_pPickGear)
		{
			m_pPickGear->vPos = { vMouse.x, vMouse.y, 0.f };
			m_pMapTool->UpdateData(TRUE);
			m_pMapTool->m_fX = m_pPickGear->vPos.x;
			m_pMapTool->m_fY = m_pPickGear->vPos.y;
			m_pMapTool->UpdateData(FALSE);
		}
		else if (m_pPickItem)
		{
			m_pPickItem->vPos = { vMouse.x, vMouse.y, 0.f };
			m_pMapTool->UpdateData(TRUE);
			m_pMapTool->m_fX = m_pPickItem->vPos.x;
			m_pMapTool->m_fY = m_pPickItem->vPos.y;
			m_pMapTool->UpdateData(FALSE);
		}
	}


	// MapTool 업데이트
	NULL_CHECK(m_pMapTool);
	m_pMapTool->Invalidate(FALSE);

	// MiniView 업데이트
	NULL_CHECK(m_pMiniView);
	m_pMiniView->Invalidate(FALSE);

	Invalidate(FALSE);
}


void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnRButtonDown(nFlags, point);

	D3DXVECTOR3 vMouse =
	{
		float(point.x) + CScrollView::GetScrollPos(0),
		float(point.y) + CScrollView::GetScrollPos(1),
		0.f
	};

	for (auto& pTile : m_pMap->m_listTile)
	{
		if (pTile->rcRect.left + CScrollView::GetScrollPos(0) <= vMouse.x && pTile->rcRect.right + CScrollView::GetScrollPos(0) >= vMouse.x
			&& pTile->rcRect.top + CScrollView::GetScrollPos(1) <= vMouse.y && pTile->rcRect.bottom + CScrollView::GetScrollPos(1) >= vMouse.y)
		{
			m_pMapTool->DisableXY(true);
			m_pPickTile = pTile;
			m_pMapTool->UpdateData(TRUE);
			m_pMapTool->m_fX = pTile->vPos.x;
			m_pMapTool->m_fY = pTile->vPos.y;
			m_pMapTool->m_fSizeX = pTile->vSize.x;
			m_pMapTool->m_fSizeY = pTile->vSize.y;
			m_pMapTool->UpdateData(FALSE);
			m_bPickMove = true;
		}
	}

	//for (auto& map : m_pGear->m_mapInfo)
	//{
	//	for (auto& pInfo : map.second)
	//	{
	//		m_pPickGear = pInfo;
	//		m_pMapTool->UpdateData(TRUE);
	//		m_pMapTool->m_fX = pInfo->vPos.x;
	//		m_pMapTool->m_fY = pInfo->vPos.y;
	//		m_pMapTool->UpdateData(FALSE);
	//		m_bPickMove = true;
	//	}
	//}

	
	for (auto& pItem : m_listItem)
	{
		float fLeft = pItem->vPos.x - 47.f;
		float fRight = pItem->vPos.x + 47.f;
		float fTop = pItem->vPos.y - 52.5f;
		float fBottom = pItem->vPos.y + 52.5f;
		if (fLeft <= vMouse.x && fRight >= vMouse.x
			&& fTop <= vMouse.y && fBottom >= vMouse.y)
		{
			m_pMapTool->DisableXY(false);
			m_pMapTool->m_fX = pItem->vPos.x;
			m_pMapTool->m_fY = pItem->vPos.y;
			m_pMapTool->UpdateData(FALSE);

			m_pPickItem = pItem;
			m_bPickMove = true;
		}
	}

	// MapTool 업데이트
	NULL_CHECK(m_pMapTool);
	m_pMapTool->Invalidate(FALSE);

	// MiniView 업데이트
	NULL_CHECK(m_pMiniView);
	m_pMiniView->Invalidate(FALSE);

	Invalidate(FALSE);
}

void CToolView::SpaceRender()
{
	D3DXMATRIX matWorld, matScale, matTrans;

	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Space1");
	float fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollView::GetScrollPos(0), 0 - CScrollView::GetScrollPos(1), 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Space2");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollView::GetScrollPos(0), 0 - CScrollView::GetScrollPos(1), 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, -fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Space3");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollView::GetScrollPos(0), 0 - CScrollView::GetScrollPos(1), 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, -fCenterY * 2, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Star1");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollView::GetScrollPos(0), 0 - CScrollView::GetScrollPos(1), 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Star2");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, -fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Star3");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, -fCenterY * 2, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Ship1");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.15f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f - CScrollView::GetScrollPos(0), 0.f - CScrollView::GetScrollPos(1), 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X + 345.5f, 0 - 10.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"GreenLine");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 905.f - CScrollView::GetScrollPos(0), 0.f - CScrollView::GetScrollPos(1), 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, -fCenterY * 2 - 500.f, 0.f), nullptr, D3DCOLOR_ARGB(225, 255, 255, 255));

	//pTexInfo = m_pTextureMgr->GetTexInfo(L"Map");
	//fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	//fCenterY = float(pTexInfo->tImgInfo.Height * 0.5f);

	//D3DXMatrixScaling(&matScale, 1.4f, 1.f, 0.f);
	//D3DXMatrixTranslation(&matTrans, 0.f - CScrollView::GetScrollPos(0), 0.f - CScrollView::GetScrollPos(1), 0.f);
	//matWorld = matScale * matTrans;
	//m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	//m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
	//	&D3DXVECTOR3(0.f, -6551.f * 3, 0.f), nullptr, D3DCOLOR_ARGB(225, 255, 255, 255));
	
}
