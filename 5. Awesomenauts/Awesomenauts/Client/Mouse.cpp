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
	ShowCursor(false);					// ���콺 Ŀ���� �׸��� ������ �ִ� �Լ�. 
	GetCursorPos(&pt);					// ���콺�� ��ǥ�� ������ �Լ�. �ִ� ��ũ�� ���� ��ǥ�� ���´�. 
	ScreenToClient(g_hWnd, &pt);		//��ũ�� ��ǥ���� Ŭ���̾�Ʈ ��ǥ�� ��ȯ �����ش�. 

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
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// ���� ��Ʈ���� ����
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
	// CGameObject ��Ӻ��� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
	m_tInfo.vPos = { 400.f, 300.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Mouse";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// CMouse �������� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
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
