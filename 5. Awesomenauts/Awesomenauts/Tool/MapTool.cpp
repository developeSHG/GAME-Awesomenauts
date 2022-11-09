// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "FileInfo.h"
#include "MyMap.h"
#include "Gear.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_fX(0.f)
	, m_fY(0.f)
	, m_fSizeX(0.f)
	, m_fSizeY(0.f)
{
	
}

CMapTool::~CMapTool()
{
	auto& iter_begin = m_listTile.begin();

	for (; iter_begin != m_listTile.end(); ++iter_begin)
		SAFE_DELETE(*iter_begin);

	m_listTile.clear();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// Control Value
	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxType);
	DDX_Control(pDX, IDC_EDIT3, m_EditX);
	DDX_Control(pDX, IDC_EDIT4, m_EditY);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl);
	// User Value
	DDX_Text(pDX, IDC_EDIT3, m_fX);
	DDX_Text(pDX, IDC_EDIT4, m_fY);
	DDX_Text(pDX, IDC_EDIT7, m_fSizeX);
	DDX_Text(pDX, IDC_EDIT5, m_fSizeY);
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_RETURN(pFrameWnd, FALSE);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK_RETURN(pToolView, FALSE);

	m_pToolView = pToolView;

	m_ComboBoxType.ResetContent();

	m_ComboBoxType.AddString(L"00.Land");
	m_ComboBoxType.AddString(L"01.Tile");
	m_ComboBoxType.AddString(L"02.Item");
	m_ComboBoxType.AddString(L"03.Gear");
	m_ComboBoxType.AddString(L"04.Red_STOP");
	m_ComboBoxType.AddString(L"05.Red_JUMP");
	m_ComboBoxType.AddString(L"06.Blue_STOP");
	m_ComboBoxType.AddString(L"07.Blue_JUMP");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedButtonApply)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMapTool::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.

void CMapTool::OnCbnSelchangeComboType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ListBox.ResetContent();

	m_pToolView->m_pPickTile = nullptr;
	m_pToolView->m_pPickGear = nullptr;
	m_pToolView->m_pPickItem = nullptr;
	m_pToolView->m_bPickMove = false;

	int iIndex = m_ComboBoxType.GetCurSel();
	CDeviceMgr::GetInstance()->Render_Begin();
	CDeviceMgr::GetInstance()->Render_End(m_PictureCtrl.m_hWnd);
	switch (iIndex)
	{
	case 0:
		break;
	case 1:
		{
			// 대화상자에 배치한 픽처컨트롤에 타일이미지 렌더링.
			CDeviceMgr::GetInstance()->Render_Begin();
			//CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
			const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
				L"Tile");
			NULL_CHECK(pTexInfo);

			D3DXMATRIX matScale, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, WINCX/ pTexInfo->tImgInfo.Width / 2.f, WINCY / pTexInfo->tImgInfo.Height / 3.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);
			matWorld = matScale * matTrans;

			float fCenterX = pTexInfo->tImgInfo.Width / 2.f;
			float fCenterY = pTexInfo->tImgInfo.Height / 2.f;

			CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//CDeviceMgr::GetInstance()->GetSprite()->End();
			CDeviceMgr::GetInstance()->Render_End(m_PictureCtrl.m_hWnd);
		}
		break;
	case 2:
		m_ListBox.AddString(L"SliverS");
		m_ListBox.AddString(L"Medison");
		m_ListBox.AddString(L"GoldS");
		break;
	case 3:
		m_ListBox.AddString(L"One");
		m_ListBox.AddString(L"Two");
		m_ListBox.AddString(L"Three");
		m_ListBox.AddString(L"Four");
		m_ListBox.AddString(L"Five");
		m_ListBox.AddString(L"Six");
		break;
	}
}

void CMapTool::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	TILE* tTile = nullptr;

	int iIndex = m_ComboBoxType.GetCurSel();

	NULL_CHECK(m_pToolView);
	
	if (nullptr == m_pToolView->m_pPickTile)
	{
		switch (iIndex)
		{
		case 0:
			tTile = new TILE;
			NULL_CHECK(tTile);
			tTile->eType = TILE_TYPE::COLL_ALL;
			tTile->vSize = { m_fSizeX, m_fSizeY, 0.f };

			SCAST<CMyMap*>(m_pToolView->m_pMap)->m_listTile.push_back(tTile);
			m_pToolView->m_pPickTile = tTile;
			m_pToolView->m_bPickMove = true;
			break;
		case 1:
			tTile = new TILE;
			NULL_CHECK(tTile);
			tTile->eType = TILE_TYPE::COLL_UP;
			tTile->vSize = { m_fSizeX, m_fSizeY, 0.f };

			SCAST<CMyMap*>(m_pToolView->m_pMap)->m_listTile.push_back(tTile);
			m_pToolView->m_pPickTile = tTile;
			m_pToolView->m_bPickMove = true;
			break;
		case 2:
			if (m_pToolView->m_pPickItem)
			{
				m_pToolView->m_pPickItem->vPos = { m_fX, m_fY, 0.f };
				m_pToolView->m_pPickItem = nullptr;
				m_pToolView->m_bPickMove = false;
			}
			break;
		case 3:
			break;
		case 4:
			tTile = new TILE;
			NULL_CHECK(tTile);
			tTile->eType = TILE_TYPE::RED_STOP;
			tTile->vSize = { m_fSizeX, m_fSizeY, 0.f };

			SCAST<CMyMap*>(m_pToolView->m_pMap)->m_listTile.push_back(tTile);
			m_pToolView->m_pPickTile = tTile;
			m_pToolView->m_bPickMove = true;
			break;
		case 5:
			tTile = new TILE;
			NULL_CHECK(tTile);
			tTile->eType = TILE_TYPE::RED_JUMP;
			tTile->vSize = { m_fSizeX, m_fSizeY, 0.f };

			SCAST<CMyMap*>(m_pToolView->m_pMap)->m_listTile.push_back(tTile);
			m_pToolView->m_pPickTile = tTile;
			m_pToolView->m_bPickMove = true;
			break;
		case 6:
			tTile = new TILE;
			NULL_CHECK(tTile);
			tTile->eType = TILE_TYPE::BLUE_STOP;
			tTile->vSize = { m_fSizeX, m_fSizeY, 0.f };

			SCAST<CMyMap*>(m_pToolView->m_pMap)->m_listTile.push_back(tTile);
			m_pToolView->m_pPickTile = tTile;
			m_pToolView->m_bPickMove = true;
			break;
		case 7:
			tTile = new TILE;
			NULL_CHECK(tTile);
			tTile->eType = TILE_TYPE::BLUE_JUMP;
			tTile->vSize = { m_fSizeX, m_fSizeY, 0.f };

			SCAST<CMyMap*>(m_pToolView->m_pMap)->m_listTile.push_back(tTile);
			m_pToolView->m_pPickTile = tTile;
			m_pToolView->m_bPickMove = true;
			break;
		}
	}
	else if (m_pToolView->m_pPickTile)
	{
		m_pToolView->m_pPickTile->vSize = { m_fSizeX, m_fSizeY, 0.f };
		m_pToolView->m_pPickTile->vPos = {m_fX , m_fY , 0.f };
		m_pToolView->m_pPickTile = nullptr;
		DisableXY(true);
	}

	m_pToolView->Invalidate(FALSE);
	m_pToolView->m_pMiniView->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CMapTool::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pToolView->m_pPickTile)
	{
		auto& iter_begin = m_pToolView->m_pMap->m_listTile.begin();

		for (auto& pTile : m_pToolView->m_pMap->m_listTile)
		{
			if (pTile == m_pToolView->m_pPickTile)
			{
				SAFE_DELETE(pTile);
				m_pToolView->m_pPickTile = nullptr;
				m_pToolView->m_pMap->m_listTile.erase(iter_begin);
				m_pToolView->m_bPickMove = false;
				m_pToolView->Invalidate(FALSE);
				m_pToolView->m_pMiniView->Invalidate(FALSE);
				return;
			}
			iter_begin++;
		}
	}
	int iIndex = m_ComboBoxType.GetCurSel();
	//auto& iter = m_pToolView->m_listItem.end();
	auto& iter = m_pToolView->m_listItem.begin();
	switch (iIndex)
	{
	case 2:
		for (auto& pItem : m_pToolView->m_listItem)
		{
			if (pItem == m_pToolView->m_pPickItem)
			{
				SAFE_DELETE(pItem);
				m_pToolView->m_listItem.erase(iter);
				m_pToolView->m_pPickItem = nullptr;
				m_pToolView->m_bPickMove = false;
				break;
			}
			++iter;
		}

		//--iter;
		//SAFE_DELETE(*iter);
		//m_pToolView->m_listItem.erase(iter);
		////m_pToolView->m_pPickItem = nullptr;
		////m_pToolView->m_bPickMove = false;
		m_pToolView->Invalidate(FALSE);
		break;
	case 3:
		int iIndex = m_ListBox.GetCurSel();

		if (-1 == iIndex)
			return;

		CString strSelectName = L"";
		m_ListBox.GetText(iIndex, strSelectName);

		if (m_pToolView->m_pGear->m_mapInfo[strSelectName].size())
		{
			auto& iter = m_pToolView->m_pGear->m_mapInfo[strSelectName].end();
			SAFE_DELETE(m_pToolView->m_pGear->m_mapInfo[strSelectName].back());
			m_pToolView->m_pGear->m_mapInfo[strSelectName].erase(--iter);

			m_pToolView->Invalidate(FALSE);
			m_pToolView->m_pMiniView->Invalidate(FALSE);
			return;
		}
		break;
	}
}

void CMapTool::OnLbnSelchangeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_pToolView->m_pPickTile = nullptr;
	m_pToolView->m_pPickGear = nullptr;
	m_pToolView->m_pPickItem = nullptr;
	m_pToolView->m_bPickMove = false;

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strSelectName = L"";
	m_ListBox.GetText(iIndex, strSelectName);

	iIndex = m_ComboBoxType.GetCurSel();

	switch (iIndex)
	{
	case 2:
		{
			// 대화상자에 배치한 픽처컨트롤에 타일이미지 렌더링.
			CDeviceMgr::GetInstance()->Render_Begin();
			const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
				strSelectName.GetString());
			NULL_CHECK(pTexInfo);

			D3DXMATRIX matScale, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, WINCX / pTexInfo->tImgInfo.Width / 1.5f, WINCY / pTexInfo->tImgInfo.Height / 1.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);
			matWorld = matScale * matTrans;

			float fCenterX = pTexInfo->tImgInfo.Width / 2.f;
			float fCenterY = pTexInfo->tImgInfo.Height / 2.f;

			CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			CDeviceMgr::GetInstance()->Render_End(m_PictureCtrl.m_hWnd);

			ITEM_INFO* pItem = nullptr;

			pItem = new ITEM_INFO;
			NULL_CHECK(pItem);

			ZeroMemory(pItem, sizeof(ITEM_INFO));
			iIndex = m_ListBox.GetCurSel();
			if (0 == iIndex)
				pItem->eType = ITEM_TYPE::MEDICIN;
			else if(1 == iIndex)
				pItem->eType = ITEM_TYPE::MEDICIN;
			else if (2 == iIndex)
				pItem->eType = ITEM_TYPE::SLIVER;
			m_pToolView->m_listItem.emplace_back(pItem);
			m_pToolView->m_pPickItem = pItem;
			m_pToolView->m_bPickMove = true;
		}
		break;
	case 3:
		{
			if(0.f >= m_fSizeX || 5.f < m_fSizeX)
				m_fSizeX = 1.f;
			if (0.f >= m_fSizeY || 5.f < m_fSizeY)
				m_fSizeY = 1.f;

			// 대화상자에 배치한 픽처컨트롤에 타일이미지 렌더링.
			CDeviceMgr::GetInstance()->Render_Begin();
			const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
				L"Gear", strSelectName.GetString(), 0);
			NULL_CHECK(pTexInfo);

			D3DXMATRIX matScale, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, WINCX / pTexInfo->tImgInfo.Width / 1.f, WINCY / pTexInfo->tImgInfo.Height / 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);
			matWorld = matScale * matTrans;

			float fCenterX = pTexInfo->tImgInfo.Width / 2.f;
			float fCenterY = pTexInfo->tImgInfo.Height / 2.f;

			CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			CDeviceMgr::GetInstance()->Render_End(m_PictureCtrl.m_hWnd);

			INFO* pInfo = nullptr;

			pInfo = new INFO;
			NULL_CHECK(pInfo);
			
			ZeroMemory(pInfo, sizeof(INFO));
			pInfo->vSize = { m_fSizeX, m_fSizeY, 0.f };
			SCAST<CGear*>(m_pToolView->m_pGear)->m_mapInfo[strSelectName.GetString()].emplace_back(pInfo);
			m_pToolView->m_pPickGear = pInfo;
			m_pToolView->m_bPickMove = true;
		}
		break;
	}

	m_pToolView->Invalidate(FALSE);
	m_pToolView->m_pMiniView->Invalidate(FALSE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}

void CMapTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,	/* 열기 TRUE, 저장 FALSE */
		L"dat", /* 디폴트 확장자 */
		L"제목없음.dat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[MAX_STR] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Save Failed");
			return;
		}

		DWORD dwByte = 0;
		bool bTitleCheck = false;
		TCHAR szTitle[MIN_STR] = L"";
		for (auto& pTile : m_pToolView->m_pMap->m_listTile)
		{
			lstrcpy(szTitle, L"TileStart");
			WriteFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
			bTitleCheck = true;
		}
		if (true == bTitleCheck)
		{
			lstrcpy(szTitle, L"TileEnd");
			WriteFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
		}

		bTitleCheck = false;
		TCHAR szName[MIN_STR] = L"";
		for (auto& Gear : m_pToolView->m_pGear->m_mapInfo)
		{
			for (auto& pInfo : Gear.second)
			{
				lstrcpy(szTitle, L"GearStart");
				WriteFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				lstrcpy(szName, Gear.first.GetString());
				WriteFile(hFile, szName, sizeof(szName), &dwByte, nullptr);
				WriteFile(hFile, pInfo, sizeof(INFO), &dwByte, nullptr);
				bTitleCheck = true;
			}
		}
		if (true == bTitleCheck)
		{
			lstrcpy(szTitle, L"GearEnd");
			WriteFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
		}

		for (auto& pItem : m_pToolView->m_listItem)
		{
			lstrcpy(szTitle, L"ItemStart");
			WriteFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
			WriteFile(hFile, pItem, sizeof(ITEM_INFO), &dwByte, nullptr);
			bTitleCheck = true;
		}
		if (true == bTitleCheck)
		{
			lstrcpy(szTitle, L"ItemEnd");
			WriteFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
		}

		CloseHandle(hFile);
		ERR_MSG(L"Save Complete");
	}
}


void CMapTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		TRUE,	/* 열기 TRUE, 저장 FALSE */
		L"dat", /* 디폴트 확장자 */
		L"제목없음.dat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[MAX_STR] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Load Failed");
			return;
		}

		for (auto& pTile : m_pToolView->m_pMap->m_listTile)
			SAFE_DELETE(pTile)
		m_pToolView->m_pMap->m_listTile.clear();

		for (auto& map : m_pToolView->m_pGear->m_mapInfo)
		{
			for (auto& pInfo : map.second)
			{
				SAFE_DELETE(pInfo);
			}
			map.second.clear();
		}
		m_pToolView->m_pGear->m_mapInfo.clear();

		for (auto& pItem : m_pToolView->m_listItem)
		{
			SAFE_DELETE(pItem);
		}
		m_pToolView->m_listItem.clear();

		DWORD dwByte = 0;

		TCHAR szTitle[MIN_STR] = L"";

		ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
		if (!lstrcmp(szTitle, L"TileStart"))
		{
			TILE* pTile = nullptr;
			while (true)
			{
				pTile = new TILE;
				ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
				m_pToolView->m_pMap->m_listTile.push_back(pTile);
				ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				if (!lstrcmp(szTitle, L"TileEnd"))
				{
					ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
					break;
				}
			}
		}
		if (!lstrcmp(szTitle, L"GearStart"))
		{
			INFO* pInfo = nullptr;
			TCHAR szName[MIN_STR] = L"";
			while (true)
			{
				pInfo = new INFO;
				ReadFile(hFile, szName, sizeof(szName), &dwByte, nullptr);
				ReadFile(hFile, pInfo, sizeof(INFO), &dwByte, nullptr);
				SCAST<CGear*>(m_pToolView->m_pGear)->m_mapInfo[szName].emplace_back(pInfo);
				ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				if (!lstrcmp(szTitle, L"GearEnd"))
				{
					ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
					break;
				}
			}
		}
		if (!lstrcmp(szTitle, L"ItemStart"))
		{
			ITEM_INFO* pItem = nullptr;
			while (true)
			{
				pItem = new ITEM_INFO;
				ReadFile(hFile, pItem, sizeof(ITEM_INFO), &dwByte, nullptr);
				m_pToolView->m_listItem.emplace_back(pItem);
				ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);

				if (!lstrcmp(szTitle, L"ItemEnd"))
				{
					ReadFile(hFile, szTitle, sizeof(szTitle), &dwByte, nullptr);
					break;
				}
			}
		}

		//if (0 == dwByte)
		//	break;

		//m_pToolView->m_pMap->m_listTile.push_back(pTile);
		CloseHandle(hFile);
	}
	m_pToolView->Invalidate(FALSE);
	m_pToolView->m_pMiniView->Invalidate(FALSE);

	ERR_MSG(L"Load Complete");
	
	Invalidate(FALSE);
}

void CMapTool::DisableXY(bool _b)
{
	if (true == _b)
	{
		m_EditX.EnableWindow(false);
		m_EditY.EnableWindow(false);
	}
	else
	{
		m_EditX.EnableWindow(true);
		m_EditY.EnableWindow(true);
	}
}