#pragma once
#include "GameObject.h"
class CSpaceEnding :
	public CGameObject
{
public:
	enum INCUBATOR
	{
		START, EGG, IN_END
	};

private:
	CSpaceEnding();

public:
	virtual ~CSpaceEnding();

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
	void EggRender();
	void LogoRender();
	HRESULT CreateBooster();

public:
	static CSpaceEnding* Create();

private:
	FRAME m_tIncubatorFrame[INCUBATOR::IN_END];
	bool m_bEgg[INCUBATOR::IN_END];
	bool m_bScroll;
	bool m_bGo;
	float m_fTimeCount;
	bool m_bTimeCount;
	float m_fLogoCount;
	int m_iAlpha;
	float m_fY;
};

