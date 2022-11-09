#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "Stage.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pCurrentScene(nullptr),
	m_ePreSceneType(TYPE_END),
	m_eCurSceneType(TYPE_END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

HRESULT CSceneMgr::SceneChange(SCENE_TYPE eType)
{
	m_eCurSceneType = eType;

	if (m_ePreSceneType != m_eCurSceneType)
	{
		SafeDelete(m_pCurrentScene);

		switch (m_eCurSceneType)
		{
		case LOGO:
			m_pCurrentScene = CLogo::Create();
			break;
		case STAGE:
			m_pCurrentScene = CStage::Create();
			break;
		}

		m_ePreSceneType = m_eCurSceneType;
	}

	NULL_CHECK_RETURN(m_pCurrentScene, E_FAIL);

	return S_OK;
}

void CSceneMgr::Update()
{
	NULL_CHECK_MSG(m_pCurrentScene, L"Current Scene is null");
	m_pCurrentScene->Update();
}

void CSceneMgr::LateUpdate()
{
	NULL_CHECK_MSG(m_pCurrentScene, L"Current Scene is null");
	m_pCurrentScene->LateUpdate();
}

void CSceneMgr::Render()
{
	NULL_CHECK_MSG(m_pCurrentScene, L"Current Scene is null");
	m_pCurrentScene->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pCurrentScene);
}
