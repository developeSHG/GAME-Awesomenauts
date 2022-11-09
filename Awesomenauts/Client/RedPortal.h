#pragma once
#include "GameObject.h"
#include "Animation.h"

class CRedPortal :
	public CGameObject, public CAnimation
{
private:
	explicit CRedPortal();

public:
	virtual ~CRedPortal();

	// CGameObject��(��) ���� ��ӵ�
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CRedPortal* Create();

private:
	bool m_bActive;
	bool m_bOpen;
};

