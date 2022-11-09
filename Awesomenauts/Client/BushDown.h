#pragma once
#include "GameObject.h"
class CBushDown :
	public CGameObject
{
public:
	enum TYPE { NONE, ONIN, TYPE_END };

private:
	explicit CBushDown();

public:
	virtual ~CBushDown();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBushDown* Create();

private:
	TEX_INFO* m_pTexInfoIn;
	RECT m_tRect;
	bool m_bIn;
	int m_iAlpha[TYPE_END];
};

