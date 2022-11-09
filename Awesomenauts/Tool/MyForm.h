#pragma once



// CMyForm �� ���Դϴ�.
#include "UnitTool.h"
#include "MapTool.h"
#include "PathExtractor.h"
#include "afxcmn.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:	
	// virtual Function
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate();

	
	
public:	
	// Message Function
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTcnSelchangeTabToolManage(NMHDR *pNMHDR, LRESULT *pResult);

public:
	// MFC���� �����ϴ� Font ��ü.
	CFont			m_Font;	
	CUnitTool		m_UnitTool;
	CMapTool		m_MapTool;	
	CPathExtractor	m_Extractor;
	

public:
	// Control Value
	CTabCtrl m_TabToolManage;

public:
	// User Value
};


