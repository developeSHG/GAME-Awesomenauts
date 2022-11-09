#include "stdafx.h"
#include "CenterLight.h"


CCenterLight::CCenterLight()
{
}


CCenterLight::~CCenterLight()
{
}

int CCenterLight::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// 컬링 시작
	if (3500.f < CScrollMgr::GetScrollPos().x && 5100.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

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

void CCenterLight::LateUpdate()
{
	CGameObject::UpdateMyRect(m_tRectInfo[BODY].rcRect, 100.f, 5.f);
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CCenterLight::Render()
{
	if (false == m_bCulling)
		return;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// 호수 랜더 시작
	LakeRender();
	// 호수 랜더 끝

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 255, 255, 0))
#endif
}

HRESULT CCenterLight::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 4961.5f, WINCY + 1281.5f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
	m_wstrObjectKey = L"CenterLight";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	m_fSpeed = 400.f;

	// 고유변수
	m_iAlpha = 0;
	m_iLakeAlpha = 0;
	m_eLightType = LIGHT_TYPE::CENTER;

	return S_OK;
}

void CCenterLight::Release()
{
}

void CCenterLight::LakeRender()
{
	if (!CScrollMgr::GetLakeRender())
		return;

	m_vLake = { m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, LAKE_Y - CScrollMgr::GetScrollPos().y, 0.f };
	D3DXVECTOR3 vPos = { m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f };
	D3DXVECTOR3 vDist = vPos - m_vLake;

	D3DXMATRIX matWorld, matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y * -1, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x - vDist.x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y - vDist.y / 0.5f, 0.f);
	matWorld = matScale * matTrans;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;
	
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha/3, 255, 255, 255));
}

CCenterLight * CCenterLight::Create()
{
	CCenterLight* pInstance = new CCenterLight;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}