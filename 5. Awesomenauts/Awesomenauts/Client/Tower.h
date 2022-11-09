#pragma once
#include "Hero.h"
class CTower :
	public CHero
{
public:
	enum TOWER_POS
	{
		UP, DOWN, TYPE_END
	};

private:
	explicit CTower();

public:
	virtual ~CTower();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT LateInit() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual void FrameCheck() override;

public:
	// Set
	void Set_TowerPos(TOWER_POS _ePos) { m_eTowerPos = _ePos; }

public:
	HRESULT CreateElement();

public:
	static CTower* Create(D3DXVECTOR3& _vPos, TEAM_TYPE _eType, TOWER_POS _ePos);

private:
	TOWER_POS m_eTowerPos;
};

