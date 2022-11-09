#pragma once
#include "Hero.h"
class CRedLight :
	public CHero
{
private:
	explicit CRedLight();

public:
	virtual ~CRedLight();

	// CGameObject��(��) ���� ��ӵ�
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CRedLight* Create();

private:
	int m_iAlpha;
};

