#pragma once
#include "afxwin.h"


// CPathExtractor ��ȭ �����Դϴ�.

class CPathExtractor : public CDialog
{
	DECLARE_DYNAMIC(CPathExtractor)

public:
	CPathExtractor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathExtractor();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
