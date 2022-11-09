#pragma once
class CMaingame
{
private:
	explicit CMaingame();

public:
	~CMaingame();

public:
	void Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();

public:
	static CMaingame* Create();

private:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CObjectMgr*		m_pObjectMgr;
	CSceneMgr*		m_pSceneMgr;
	CTimeMgr*		m_pTimeMgr;
	CFrameMgr*		m_pFrameMgr;
	CSoundMgr*		m_pSoundMgr;
	CKeyMgr*		m_pKeyMgr;
};

