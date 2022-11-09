// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabToolManage);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTabToolManage)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_Font.CreatePointFont(85, L"Verdana");

	// 현재 대화상자에 배치된 오브젝트를 얻어오는 함수.
	GetDlgItem(IDC_TAB1)->SetFont(&m_Font);

	CRect rect;
	m_TabToolManage.GetWindowRect(&rect);

	CString tab1 = L"Map";
	CString tab2 = L"Unit";
	CString tab3 = L"Pathextractor";

	m_TabToolManage.InsertItem(1, tab1);
	m_TabToolManage.InsertItem(2, tab2);
	m_TabToolManage.InsertItem(3, tab3);

	if (nullptr == m_MapTool.GetSafeHwnd())
	{
		m_MapTool.Create(IDD_MAPTOOL, &m_TabToolManage);
		m_MapTool.MoveWindow(0, 20, rect.Width(), rect.Height() - 20);
		m_MapTool.ShowWindow(SW_SHOW);
	}

	if (nullptr == m_UnitTool.GetSafeHwnd())
	{
		m_UnitTool.Create(IDD_UNITTOOL, &m_TabToolManage);
		m_UnitTool.MoveWindow(0, 20, rect.Width(), rect.Height() - 20);
		m_UnitTool.ShowWindow(SW_HIDE);
	}

	if (nullptr == m_Extractor.GetSafeHwnd())
	{
		m_Extractor.Create(IDD_PATHEXTRACTOR, &m_TabToolManage);
		m_Extractor.MoveWindow(0, 20, rect.Width(), rect.Height() - 20);
		m_Extractor.ShowWindow(SW_HIDE);
	}

	CSize size;
	size = GetTotalSize();
	size.cx = 0;
	size.cy = 0;
	SetScrollSizes(MM_TEXT, size);
}

void CMyForm::OnTcnSelchangeTabToolManage(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int iTabIndex = m_TabToolManage.GetCurSel();

	switch (iTabIndex)
	{
	case 0:
		m_MapTool.ShowWindow(SW_SHOW);
		m_UnitTool.ShowWindow(SW_HIDE);
		m_Extractor.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_MapTool.ShowWindow(SW_HIDE);
		m_UnitTool.ShowWindow(SW_SHOW);
		m_Extractor.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_UnitTool.ShowWindow(SW_HIDE);
		m_MapTool.ShowWindow(SW_HIDE);
		m_Extractor.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}
