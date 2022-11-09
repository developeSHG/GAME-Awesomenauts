#pragma once
#include "GameObject.h"
class CMouse :
	public CGameObject
{
private:
	CMouse();

public:
	virtual ~CMouse();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMouse* Create();

};

