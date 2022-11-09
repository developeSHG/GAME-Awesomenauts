#include "stdafx.h"
#include "Logo.h"

bool CLogo::m_bLoadingComplete = false;

CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Update()
{
	if (false == m_bLoadingComplete)
		return;
}

void CLogo::LateUpdate()
{
	if (false == m_bLoadingComplete)
		return;

	WaitForSingleObject(m_hLoadingThread, INFINITE);

	HRESULT hr = m_pSceneMgr->SceneChange(CSceneMgr::STAGE);
	FAILED_CHECK_MSG(hr, L"Stage Scene Change Failed");

	return;
	
}

void CLogo::Render()
{
	m_wstrLoadingMessage = m_pTextureMgr->GetLoadingMessage();

	D3DXMATRIX matTrans;

	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Loading");

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&matTrans, 0, 0.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Load1");
	D3DXMatrixTranslation(&matTrans, WINCX/2.f - 160.f, WINCY -260.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Load2");
	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matWorld, matRotate;
	m_fAngle += 10;
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f - 127.f, WINCY - 226.f, 0.f);
	matWorld = matRotate * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CLogo::Initialize()
{
	m_hLoadingThread = (HANDLE)_beginthreadex(
		nullptr, 0, LoadingTextureByThread, this, 0, nullptr);

	if (INVALID_HANDLE_VALUE == m_hLoadingThread)
		return E_FAIL;

	InitializeCriticalSection(&m_CriticalSection);

	HRESULT hr = TextureLoad();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_fAngle = 0.f;

	return S_OK;
}

void CLogo::Release()
{
	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_CriticalSection);
}

CLogo* CLogo::Create()
{
	CLogo* pInstance = new CLogo;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

unsigned CLogo::LoadingTextureByThread(void* pParam)
{
	CLogo* pLogo = reinterpret_cast<CLogo*>(pParam);

	EnterCriticalSection(&pLogo->m_CriticalSection);

	CTextureMgr::GetInstance()->LoadTextureFromImgPathFile(L"../Data/ImgPath.txt");
	//CTextureMgr::GetInstance()->SetLoadingMessage(L"로딩완료!");
	m_bLoadingComplete = true;

	LeaveCriticalSection(&pLogo->m_CriticalSection);

	return 0;
}

HRESULT CLogo::TextureLoad()
{
	HRESULT hr = m_pTextureMgr->LoadTexture(
		CTextureMgr::SINGLE_TEXTURE, L"../Texture/Logo/Loading.png",
		L"Loading");
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Texture Load Failed", E_FAIL);

	hr = m_pTextureMgr->LoadTexture(
		CTextureMgr::SINGLE_TEXTURE, L"../Texture/Logo/Load1.png",
		L"Load1");
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Texture Load Failed", E_FAIL);

	hr = m_pTextureMgr->LoadTexture(
		CTextureMgr::SINGLE_TEXTURE, L"../Texture/Logo/Load2.png",
		L"Load2");
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Texture Load Failed", E_FAIL);

	return S_OK;
}
