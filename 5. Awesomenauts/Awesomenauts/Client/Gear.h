#pragma once
#include "GameObject.h"
#include "Animation.h"

class CGear :
	public CGameObject, public CAnimation
{
private:
	explicit CGear();

public:
	virtual ~CGear();

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
	HRESULT LoadFile();

public:
	static CGear* Create();

private:
	map<wstring, list<INFO*>> m_mapInfo;
	map<wstring, float> m_mapFrame;
};
