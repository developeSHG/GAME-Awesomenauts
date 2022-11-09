#pragma once
#include "Hero.h"
class CSaver :
	public CHero
{
private:
	explicit CSaver();

public:
	virtual ~CSaver();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	virtual void FrameCheck() override;
	virtual void AttackStateActive() override;
	virtual void DirCheck() override;

public:
	static CSaver* Create(/*D3DXVECTOR3 _vPos = { WINCX / 2 - 45.f, 480.f, 0.f }*/);

};

