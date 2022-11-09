#include "stdafx.h"
#include "JungleDeco.h"


CJungleDeco::CJungleDeco()
{
}


CJungleDeco::~CJungleDeco()
{
}

int CJungleDeco::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// 컬링 시작
	if (3400.f < CScrollMgr::GetScrollPos().x && 5300.f > CScrollMgr::GetScrollPos().x 
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	return NO_EVENT;
}

void CJungleDeco::LateUpdate()
{
	if (false == m_bCulling)
		return;

	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CJungleDeco::Render()
{
	if (false == m_bCulling)
		return;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	fCenterX = m_pTexInfo2->tImgInfo.Width * 0.5f;
	fCenterY = m_pTexInfo2->tImgInfo.Height * 0.5f;
	D3DXMATRIX matWorld = m_tInfo.matWorld;
	matWorld._41 += 296.f;
	matWorld._42 += 10.f;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo2->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CJungleDeco::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 4820.f, WINCY + 834.f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
	m_wstrObjectKey = L"JungleDeco1";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// 고유변수
	m_pTexInfo2 = m_pTextureMgr->GetTexInfo(L"JungleDeco2");
	NULL_CHECK_RETURN(m_pTexInfo2, E_FAIL);

	return S_OK;
}

void CJungleDeco::Release()
{
}

CJungleDeco * CJungleDeco::Create()
{
	CJungleDeco* pInstance = new CJungleDeco;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
