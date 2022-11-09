#pragma once
#include "GameObject.h"
class CMinimap :
	public CGameObject
{
private:
	explicit CMinimap();

public:
	virtual ~CMinimap();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	void PlayerRender();
	void RedHeroRender();
	void BlueHeroRender();

public:
	static CMinimap* Create();

private:
	TEX_INFO* m_pTexInfoHero;
	TEX_INFO* m_pTexInfoRedHero;
	TEX_INFO* m_pTexInfoBlueHero;
};

