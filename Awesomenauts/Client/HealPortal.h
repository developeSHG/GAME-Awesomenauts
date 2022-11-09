#pragma once
#include "GameObject.h"
#include "Animation.h"

class CHealPortal :
	public CGameObject, public CAnimation
{
public:
	enum TEAM { RED, BLUE };

private:
	explicit CHealPortal();

public:
	virtual ~CHealPortal();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	void Set_Pos(D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos; }

public:
	static CHealPortal* Create(D3DXVECTOR3& vPos);

private:
	TEAM m_eTeam;
	float m_fTimeCount;
};

