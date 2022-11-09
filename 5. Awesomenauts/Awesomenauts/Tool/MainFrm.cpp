
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"
#include "ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("���� ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	cs.cx = WINCX + 400;
	cs.cy = WINCY;

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// ����
	/* ������ ���� ���� �ؿ���. �̴ϸʵ� �����Ͷ�!
	----------------------------- 
	|			|				|
	|	mini	|				|
	|			|				|
	-------------	tool		|
	|			|				|
	|	form	|				|
	|			|				|
	-----------------------------
	*/
	m_MainSplitt.CreateStatic(this, 1, 2);
	m_SecondSplitt.CreateStatic(&m_MainSplitt, 2, 1,
		WS_CHILD | WS_VISIBLE, m_MainSplitt.IdFromRowCol(0, 1));

	// ���� �ʺ� �����ϴ� �Լ�.
	// SetColumnInfo(�� ��ȣ, ������ ũ��, �ּ� ũ��)	
	m_MainSplitt.SetColumnInfo(0, WINCX, 0);
	m_MainSplitt.SetColumnInfo(1, 300, 0);

	m_MainSplitt.CreateView(0, 0, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);
	m_SecondSplitt.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(400, 200), pContext);
	m_SecondSplitt.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(400, 300), pContext);

	
	
	//// â����
	//m_MainSplitt.CreateStatic(this, 1, 2);

	//// ���ҵ� ������ View�� ��ġ	
	////m_MainSplitt.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, WINCY), pContext);
	//m_MainSplitt.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY), pContext);
	//m_MainSplitt.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	return TRUE/*CFrameWnd::OnCreateClient(lpcs, pContext)*/;
}