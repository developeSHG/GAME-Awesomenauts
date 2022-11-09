// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "ToolView.h"
#include "MainFrm.h"

#include "MyMap.h"
#include "Gear.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	// CSplitterWnd::GetPane(row, col) row, col�� ��ġ�� CWnd*�� ��ȯ.
	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);

	CDeviceMgr::GetInstance()->Render_Begin();

	pToolView->m_pMap->MiniRender();
	pToolView->m_pGear->MiniRender();

	CDeviceMgr::GetInstance()->Render_End(m_hWnd);
}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.
