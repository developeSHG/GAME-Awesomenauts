#include "stdafx.h"
#include "Right.h"


CRight::CRight()
{
}


CRight::~CRight()
{
}

int CRight::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// 컬링 시작
	if (1200.f < CScrollMgr::GetScrollPos().x)
		m_bCulling = false;
	else
		m_bCulling = true;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	for (int i = 0; i < 3; ++i)
	{
		if (m_iAlpha[i] >= 255)
			m_fSpeed[i] = -5.f;
		else if (m_iAlpha[i] <= 0)
			m_fSpeed[i] = 5.f;
		m_iAlpha[i] += int(m_fSpeed[i]);
	}

	return NO_EVENT;
}

void CRight::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CRight::Render()
{
	if (false == m_bCulling)
		return;

	float fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha[0], 255, 255, 255));

	D3DXMATRIX matWorld = m_tInfo.matWorld;
	matWorld._41 += 150.f;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha[1], 255, 255, 255));

	matWorld._41 += 150.f;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha[2], 255, 255, 255));
}

HRESULT CRight::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = {700.f, WINCY + 400.f, 0.f };
	m_tInfo.vSize = { 0.9f, 0.9f, 0.f };
	m_wstrObjectKey = L"Right";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// 고유변수
	m_iAlpha[0] = 85;
	m_iAlpha[1] = 195;
	m_iAlpha[2] = 255;
	m_fSpeed[0] = -5.f;
	m_fSpeed[1] = -5.f;
	m_fSpeed[2] = -5.f;

	return S_OK;
}

void CRight::Release()
{
}

CRight * CRight::Create()
{
	CRight* pInstance = new CRight;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
