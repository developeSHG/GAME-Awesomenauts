#pragma once
#include "Hero.h"
class CBlueLight :
	public CHero
{
private:
	explicit CBlueLight();

public:
	virtual ~CBlueLight();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBlueLight* Create();

private:
	int m_iAlpha;
};

