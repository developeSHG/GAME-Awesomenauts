#pragma once

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

public:
	enum SCENE_TYPE { LOGO, STAGE, TYPE_END };

private:
	CSceneMgr();
	~CSceneMgr();

public:
	HRESULT SceneChange(SCENE_TYPE eType);
	void Update();
	void LateUpdate();
	void Render();

private:
	void Release();

private:
	CScene*		m_pCurrentScene;
	SCENE_TYPE	m_ePreSceneType;
	SCENE_TYPE	m_eCurSceneType;
};

