#pragma once
#include "GameObject.h"
class CRight :
	public CGameObject
{
private:
	explicit CRight();

public:
	virtual ~CRight();

	// CGameObject��(��) ���� ��ӵ�
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CRight* Create();

private:
	int m_iAlpha[3];
	float m_fSpeed[3];
};

