#pragma once
class CScene
{
protected:
	explicit CScene();

public:
	virtual ~CScene();

public:
	virtual void Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

protected:
	CObjectMgr* m_pObjectMgr;
	CSceneMgr*	m_pSceneMgr;
	CTextureMgr*	m_pTextureMgr;
	CDeviceMgr*		m_pDeviceMgr;
};

