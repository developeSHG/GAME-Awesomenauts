#pragma once
#include "GameObject.h"
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

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// Get
	const list<TILE*>& Get_Tile() const { return m_listTile; }

public:
	HRESULT CreateTerrain();
	void UpdateTileRect();

public:
	static CMyMap* Create();

private:
	list<TILE*> m_listTile;

};

