#pragma once
#include "afxwin.h"


// CMapTool 대화 상자입니다.

class CToolView;
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:	
	// virtual function
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

public:	
	// message function
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:	
	// user function
	void DisableXY(bool _b);

public:		
	// Control
	CComboBox m_ComboBoxType;
	CListBox m_ListBox;	
	CStatic m_PictureCtrl;
	CEdit m_EditX;
	CEdit m_EditY;

public:		
	// User
	float m_fX;
	float m_fY;
	float m_fSizeX;
	float m_fSizeY;

public:
	list<TILE*> m_listTile;
	CToolView* m_pToolView;

	

};
