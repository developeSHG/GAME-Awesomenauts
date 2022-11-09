#pragma once
#include "GameObject.h"
#include "Animation.h"
class CBar :
	public CGameObject, public CAnimation
{
public:
	enum TYPE { RED1, RED2, RED3, BLUE1, BLUE2, BLUE3, TYPE_END };

private:
	explicit CBar();

public:
	virtual ~CBar();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBar* Create();

private:
	INFO m_tBarInfo[TYPE_END];
	FRAME m_tBarFrame[TYPE_END];
	bool m_bBarCulling[TYPE_END];
};

