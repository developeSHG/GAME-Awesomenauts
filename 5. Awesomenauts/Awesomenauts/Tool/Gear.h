#pragma once
#include "GameObject.h"

class CToolView;

class CGear :
	public CGameObject
{
private:
	explicit CGear();

public:
	virtual ~CGear();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void MiniRender() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	virtual void SetToolView(CToolView* pView);

public:
	static CGear* Create(CToolView* pView);

public:
	CToolView*	m_pToolView;
	map<CString, list<INFO*>> m_mapInfo;
};
