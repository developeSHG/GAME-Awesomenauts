#pragma once
#include "afxwin.h"


// CPathExtractor 대화 상자입니다.

class CPathExtractor : public CDialog
{
	DECLARE_DYNAMIC(CPathExtractor)

public:
	CPathExtractor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathExtractor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

private:	// user function
	void CreateHorizontalScroll();

public:
	CListBox m_ListBox;

public:
	list<PATH_INFO*>	m_PathInfoLst;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
};
