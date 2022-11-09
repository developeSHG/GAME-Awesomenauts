#include "stdafx.h"
#include "Mouse.h"

#include "Player.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
	Release();
}

int CMouse::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	//CGameObject::LateInit();
	
	POINT pt = {};
	ShowCursor(false);					// 마우스 커서를 그릴지 결정해 주는 함수. 
	GetCursorPos(&pt);					// 마우스의 좌표를 얻어오는 함수. 애는 스크린 기준 좌표를 얻어온다. 
	ScreenToClient(g_hWnd, &pt);		//스크린 좌표에서 클라이언트 좌표로 변환 시켜준다. 

	m_tInfo.vPos.x = float(pt.x);
	m_tInfo.vPos.y = float(pt.y);

	CGameObject* pObject = m_pObjectMgr->Get_Player();
	
	if (false == SCAST<CPlayer*>(pObject)->Get_AfterSpeed())
	{
		if (m_tInfo.vPos.x < pObject->Get_Pos().x)
			SCAST<CPlayer*>(pObject)->Set_Dir(DIR::LEFT);
		else
			SCAST<CPlayer*>(pObject)->Set_Dir(DIR::RIGHT);
	}
	

	return NO_EVENT;
}

void CMouse::LateUpdate()
{
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
}

void CMouse::Render()
{
	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CMouse::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 400.f, 300.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Mouse";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// CMouse 고유변수 (초기화 변경 필요시 코딩)
	//m_wstrStateKey[STATE::IDLE] = L"Dash";

	return S_OK;
}

void CMouse::Release()
{
}

CMouse * CMouse::Create()
{
	CMouse* pInstance = new CMouse;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
