#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pDeviceMgr(CDeviceMgr::GetInstance())
{
}

CScene::~CScene()
{
}
