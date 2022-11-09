#pragma once



// CMyForm 폼 뷰입니다.
#include "UnitTool.h"
#include "MapTool.h"
#include "PathExtractor.h"
#include "afxcmn.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate();

	
	
public:	
	// Message Function
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTcnSelchangeTabToolManage(NMHDR *pNMHDR, LRESULT *pResult);

public:
	// MFC에서 제공하는 Font 객체.
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


