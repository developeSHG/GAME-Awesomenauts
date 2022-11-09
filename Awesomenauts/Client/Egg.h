#pragma once
#include "GameObject.h"
#include "Animation.h"
class CEgg :
	public CGameObject, public CAnimation
{
public:
	enum INCUBATOR
	{
		START, EGG, ARRIVE, IN_END
	};

private:
	explicit CEgg();

public:
	virtual ~CEgg();

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
	void KeyInput();
	HRESULT CreateBooster();

public:
	static CEgg* Create();

private:
	FRAME m_tIncubatorFrame[INCUBATOR::IN_END];
	bool m_bEgg[INCUBATOR::IN_END];
	bool m_bScroll;
	bool m_bMap;
	float m_fTimeCount;
	bool m_bTimeCount;
};

