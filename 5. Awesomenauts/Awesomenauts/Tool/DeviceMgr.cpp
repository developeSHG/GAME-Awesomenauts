#include "stdafx.h"
#include "DeviceMgr.h"

IMPLEMENT_SINGLETON(CDeviceMgr)

CDeviceMgr::CDeviceMgr()
{
}


CDeviceMgr::~CDeviceMgr()
{
	Release();
}

LPDIRECT3DDEVICE9 CDeviceMgr::GetDevice() const
{
	return m_pGraphicDev;
}

LPD3DXSPRITE CDeviceMgr::GetSprite() const
{
	return m_pSprite;
}

LPD3DXFONT CDeviceMgr::GetFont() const
{
	return m_pFont;
}

LPD3DXLINE CDeviceMgr::GetLine() const
{
	return m_pLine;
}

HRESULT CDeviceMgr::InitDevice(
	HWND hWnd, const DWORD& dwWinCX,
	const DWORD& dwWinCY, MODE eMode)
{
	HRESULT hr = 0;

	// 장치초기화
	// 1. IDirect3D9 생성	
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. 장치 조사 (현재 그래픽 카드의 사양 및 지원여부 조사)

	// HAL (Hardware Abstraction Layer)
	// 하드웨어 추상 계층

	D3DCAPS9 Caps;
	ZeroMemory(&Caps, sizeof(D3DCAPS9));

	// 현재 그래픽 카드의 정보를 알아내고 있다.
	hr = m_pSDK->GetDeviceCaps(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);
	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);

	// 버텍스 프로세싱 유무 조사.
	// - 현재 그래픽 카드에서 버텍스 프로세싱을 지원하는가
	// - 버텍스 프로세싱: 정점변환 + 조명연산
	DWORD vp = 0;

	if (Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//// 플래그 연산
	//DWORD 공속 = 0x0001;
	//DWORD 이속 = 0x0002;
	//DWORD 물방 = 0x0004;
	//DWORD 마방 = 0x0008;

	//DWORD 현재버프 = 공속 | 물방;

	//if (현재버프 & 공속)
	//	공격력 증가;
	//if (현재버프 & 이속)
	//	이동력 증가;
	//if (현재버프 & 물방)
	//	물리방어 증가;
	//if (현재버프 & 마방)
	//	마법방어 증가;


	// 3. IDirect3DDevice9 생성

	// 렌더링 환경 설정.
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = dwWinCX;	// 후면버퍼 가로크기
	d3dpp.BackBufferHeight = dwWinCY;	// 후면버퍼 세로크기
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // 후면버퍼 픽셀포맷
	d3dpp.BackBufferCount = 1;	// 후면버퍼 개수

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // 멀티샘플링 안씀.
	d3dpp.MultiSampleQuality = 0;

	// 렌더링 방식
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 스왑체인 방식.
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;	// TRUE 창모드, FALSE 전체화면모드.
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = m_pSDK->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, vp, &d3dpp, &m_pGraphicDev);
	FAILED_CHECK_MSG_RETURN(hr, L"CreateDevice Failed", E_FAIL);

	// ID3DXSprite 생성
	hr = D3DXCreateSprite(m_pGraphicDev, &m_pSprite);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	// ID3DXFont 생성
	D3DXFONT_DESCW tFontDesc;
	ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));

	tFontDesc.Width = 10;
	tFontDesc.Height = 20;
	tFontDesc.Weight = FW_HEAVY;
	tFontDesc.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontDesc.FaceName, L"궁서");

	hr = D3DXCreateFontIndirect(m_pGraphicDev, &tFontDesc, &m_pFont);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateFontIndirect Failed", E_FAIL);

	// Line 생성
	hr = D3DXCreateLine(m_pGraphicDev, &m_pLine);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateLine Failed", E_FAIL);
	m_pLine->SetWidth(2); // 라인의 굵기를 2로 설정

	return S_OK;
}

void CDeviceMgr::Render_Begin()
{
	// 렌더링 과정.
	// 1. 후면버퍼를 비운다.
	m_pGraphicDev->Clear(0, nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);

	// 2. 후면버퍼에 그리기 시작한다.
	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDeviceMgr::Render_End(HWND hWnd/* = nullptr*/)
{
	// 3. 후면버퍼에 그리기를 끝마친다.
	m_pSprite->End();
	m_pGraphicDev->EndScene();

	// 4. 후면버퍼를 전면버퍼로 교체하여 렌더링한다.
	m_pGraphicDev->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDeviceMgr::Release()
{
	// Release 순서 주의!
	if (SafeRelease(m_pLine))
		ERR_MSG(L"m_pLine Release Failed");

	if (SafeRelease(m_pFont))
		ERR_MSG(L"m_pFont Release Failed");

	if (SafeRelease(m_pSprite))
		ERR_MSG(L"m_pSprite Release Failed");

	if (SafeRelease(m_pGraphicDev))
		ERR_MSG(L"m_pGraphicDev Release Failed");

	if (SafeRelease(m_pSDK))
		ERR_MSG(L"m_pSDK Release Failed");
}
