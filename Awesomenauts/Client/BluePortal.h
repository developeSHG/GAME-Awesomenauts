#pragma once
#include "GameObject.h"
#include "Animation.h"

class CBluePortal :
	public CGameObject, public CAnimation
{
private:
	explicit CBluePortal();

public:
	virtual ~CBluePortal();

	// CGameObject��(��) ���� ��ӵ�
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBluePortal* Create();
};

