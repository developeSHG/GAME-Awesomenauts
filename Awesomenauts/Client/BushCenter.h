#pragma once
#include "GameObject.h"
class CBushCenter :
	public CGameObject
{
public:
	enum TYPE { NONE, ONIN, TYPE_END };

private:
	explicit CBushCenter();

public:
	virtual ~CBushCenter();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBushCenter* Create();

private:
	TEX_INFO* m_pTexInfoIn;
	RECT m_tRect;
	bool m_bIn;
	int m_iAlpha[TYPE_END];
};

