#pragma once
#include "GameObject.h"

class CToolView;

class CMyMap :
	public CGameObject
{

private:
	CMyMap();

public:
	virtual ~CMyMap();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void MiniRender() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// Get
	const list<TILE*>& Get_Tile() const { return m_listTile; }

public:
	virtual void SetToolView(CToolView* pView);

public:
	HRESULT CreateTerrain();
	void UpdateTileRect();
	void UpdateTileRectMini();

public:
	static CMyMap* Create(CToolView* pView);

public:
	CToolView*			m_pToolView;
	list<TILE*> m_listTile;
	bool m_bPickMove;

};

