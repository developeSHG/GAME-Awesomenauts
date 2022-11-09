#pragma once
#include "GameObject.h"
class CMouse :
	public CGameObject
{
private:
	CMouse();

public:
	virtual ~CMouse();

	// CGameObject��(��) ���� ��ӵ�
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

