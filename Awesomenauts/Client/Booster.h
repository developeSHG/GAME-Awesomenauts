#pragma once
#include "GameObject.h"
#include "Animation.h"
class CBooster :
	public CGameObject, public CAnimation
{
private:
	explicit CBooster();

public:
	virtual ~CBooster();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual HRESULT LateInit() override;

public:
	void Set_Type(int _i) { m_iType = _i; }
public:
	static CBooster* Create(D3DXVECTOR3& _vPos, int _i = 0);

private:
	int m_iType;

};

