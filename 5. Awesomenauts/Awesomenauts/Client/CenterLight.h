#pragma once
#include "Hero.h"
class CCenterLight :
	public CHero
{
private:
	explicit CCenterLight();

public:
	virtual ~CCenterLight();

	// CGameObject��(��) ���� ��ӵ�
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual void LakeRender() override;

public:
	static CCenterLight* Create();

private:
	int m_iAlpha;
	int m_iLakeAlpha;
};

