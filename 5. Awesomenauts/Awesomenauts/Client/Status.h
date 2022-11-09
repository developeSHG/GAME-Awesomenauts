#pragma once
#include "GameObject.h"
class CStatus :
	public CGameObject
{
private:
	explicit CStatus();

public:
	virtual ~CStatus();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	void SkillCollTimeRender();

public:
	static CStatus* Create();

private:
	bool m_bGameStart;
	float m_fTimeCount;
	bool m_bGameRender;
	int m_iAlpha;
};

