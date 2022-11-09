#include "stdafx.h"
#include "RedLight.h"

CRedLight::CRedLight()
{
}


CRedLight::~CRedLight()
{
}

int CRedLight::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// 컬링 시작
	if (1400.f > CScrollMgr::GetScrollPos().x || 3000.f < CScrollMgr::GetScrollPos().x)
		m_bCulling = false;
	else
		m_bCulling = true;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	if (m_iAlpha >= 240)
		m_fSpeed = -400.f;
	else if (m_iAlpha <= 15)
		m_fSpeed = 400.f;
	m_iAlpha += int(m_pTimeMgr->GetDeltaTime() * m_fSpeed);

	return NO_EVENT;
}

void CRedLight::LateUpdate()
{
	CGameObject::UpdateMyRect(m_tRectInfo[BODY].rcRect, 75.f, 50.f);
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CRedLight::Render()
{
	if (false == m_bCulling)
		return;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 255, 255, 0))
#endif
}

HRESULT CRedLight::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 2824.f, WINCY + 307.f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
	m_wstrObjectKey = L"RedLight";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	m_fSpeed = 400.f;

	// 고유변수
	m_iAlpha = 0;
	m_eLightType = LIGHT_TYPE::RED;

	return S_OK;
}

void CRedLight::Release()
{
}

CRedLight * CRedLight::Create()
{
	CRedLight* pInstance = new CRedLight;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
