#pragma once
#include "GameObject.h"
class CSpace :
	public CGameObject
{
public:
	enum SUMMONS
	{
		S1, S2, HOLE, END
	};
	enum INCUBATOR
	{
		START, IN_END
	};

private:
	CSpace();

public:
	virtual ~CSpace();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual HRESULT LateInit() override;

public:
	void SpaceRender();
	void SummonsRender();
	void LoadingRender();

public:
	static CSpace* Create();

private:
	bool m_bShipGo;
	int m_iAlphaLoading;
	int m_iAlphaLoadingComplete;
	int m_iAlphaTitle;
	int m_iAlphaPlay;
	int m_iAlphaSpeed;
	int m_iAlphaShip1;
	int m_iAlphaShip2;
	bool m_bPlayStart;
	float m_fTitleScroll;
	FRAME m_tSummonsFrame[SUMMONS::END];
	bool m_bHole[SUMMONS::END];
	bool m_bSummons;
	int m_iIncubator;
};

