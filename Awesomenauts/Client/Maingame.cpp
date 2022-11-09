#include "stdafx.h"
#include "Maingame.h"


CMaingame::CMaingame()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pFrameMgr(CFrameMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance())
{
	srand((unsigned int)time(NULL));
}

CMaingame::~CMaingame()
{
	Release();
}

void CMaingame::Update()
{
	m_pTimeMgr->UpdateTime();
	m_pKeyMgr->Update();

	m_pSceneMgr->Update();
}

void CMaingame::LateUpdate()
{
	m_pSceneMgr->LateUpdate();
	CScrollMgr::ScrollLock();
}

void CMaingame::Render()
{
	m_pDeviceMgr->Render_Begin();

	m_pSceneMgr->Render();
	m_pFrameMgr->RenderFPS();

	m_pDeviceMgr->Render_End();
}

HRESULT CMaingame::Initialize()
{
	m_pTimeMgr->InitTime();
	m_pFrameMgr->InitTime();
	m_pSoundMgr->Initialize();

	HRESULT hr = m_pDeviceMgr->InitDevice(
		g_hWnd, WINCX, WINCY, CDeviceMgr::MODE_FULL);
	FAILED_CHECK_MSG_RETURN(hr, L"Init Device Failed", E_FAIL);

	//hr = m_pTextureMgr->LoadTextureFromImgPathFile(L"../Data/ImgPath.txt");
	//FAILED_CHECK_MSG_RETURN(hr, L"LoadTextureFromImgPathFile Failed", E_FAIL);	

	//hr = m_pTextureMgr->LoadTexture(
	//	CTextureMgr::SINGLE_TEXTURE, L"../Texture/Stage/Map/Map.png",
	//	L"Map");
	//FAILED_CHECK_MSG_RETURN(hr, L"Map Texture Load Failed", E_FAIL);

	//hr = m_pTextureMgr->LoadTexture(
	//	CTextureMgr::SINGLE_TEXTURE, L"../Texture/Stage/UI/Mouse/Mouse.png",
	//	L"Mouse");
	//FAILED_CHECK_MSG_RETURN(hr, L"Mouse Texture Load Failed", E_FAIL);

	//hr = m_pTextureMgr->LoadTexture(
	//	CTextureMgr::MULTI_TEXTURE, L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png",
	//	L"Player", L"Dash", 11);
	//FAILED_CHECK_MSG_RETURN(hr, L"Player Dash Texture Load Failed", E_FAIL);

	hr = m_pSceneMgr->SceneChange(CSceneMgr::LOGO);
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Scene Change Failed", E_FAIL);

	return S_OK;
}

void CMaingame::Release()
{
	m_pFrameMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
	m_pSceneMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pSoundMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

CMaingame* CMaingame::Create()
{
	CMaingame* pInstance = new CMaingame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
