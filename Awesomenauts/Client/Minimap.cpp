#include "stdafx.h"
#include "Minimap.h"

#include "Player.h"

CMinimap::CMinimap()
{
}


CMinimap::~CMinimap()
{
}

int CMinimap::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	return NO_EVENT;
}

void CMinimap::LateUpdate()
{
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
}

void CMinimap::Render()
{
	if (true == CScrollMgr::GetSpaceScroll())
		return;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	BlueHeroRender();
	RedHeroRender();
	PlayerRender();
}

HRESULT CMinimap::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { WINCX - 135.f, WINCY - 55.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Minimap";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// 고유변수
	m_pTexInfoHero = m_pTextureMgr->GetTexInfo(L"MiniPlayer");
	m_pTexInfoRedHero = m_pTextureMgr->GetTexInfo(L"MiniPlayer");
	m_pTexInfoBlueHero = m_pTextureMgr->GetTexInfo(L"MiniBlue");

	return S_OK;
}

HRESULT CMinimap::LateInit()
{
	return S_OK;
}

void CMinimap::Release()
{
}

void CMinimap::PlayerRender()
{
	CGameObject* pPlayer = m_pObjectMgr->Get_Player();

	if (true == SCAST<CUnit*>(pPlayer)->Get_boolState(CUnit::STATE::DEAD))
		return;

	float fStartX = 2375.f;
	float fEndX = 7500.f;
	float fStartY = -200.f;
	float fEndY = 1635.f;

	float fMiniX = fEndX - fStartX;
	float fMiniY = fEndY - fStartY;

	float fPlayerX = CScrollMgr::GetScrollPos().x - fStartX + pPlayer->Get_Pos().x; 
	float fPlayerY = CScrollMgr::GetScrollPos().y - fStartY + pPlayer->Get_Pos().y;

	fPlayerX = fPlayerX * m_pTexInfo->tImgInfo.Width / fMiniX + (WINCX - 135.f - m_pTexInfo->tImgInfo.Width / 2.f);
	fPlayerY = fPlayerY * m_pTexInfo->tImgInfo.Height / fMiniY + (WINCY - 85.f - m_pTexInfo->tImgInfo.Height / 2.25f);

	if (1025 > fPlayerX || 1265 < fPlayerX)
		return;

	float fCenterX = m_pTexInfoHero->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfoHero->tImgInfo.Height * 0.5f;

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, fPlayerX, fPlayerY, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfoHero->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMinimap::RedHeroRender()
{
	float fStartX = 2375.f;
	float fEndX = 7500.f;
	float fStartY = -200.f;
	float fEndY = 1635.f;

	float fMiniX = fEndX - fStartX;
	float fMiniY = fEndY - fStartY;

	float fPlayerX;
	float fPlayerY;

	float fCenterX;
	float fCenterY;

	for (auto& pDest : m_pObjectMgr->Get_Obj(OBJECT::RED_HERO))
	{
		if (true == SCAST<CUnit*>(pDest)->Get_boolState(CUnit::STATE::DEAD))
			return;

		fPlayerX = -fStartX + pDest->Get_Pos().x;
		fPlayerY = -fStartY + pDest->Get_Pos().y;

		fPlayerX = fPlayerX * m_pTexInfo->tImgInfo.Width / fMiniX + (WINCX - 135.f - m_pTexInfo->tImgInfo.Width / 2.f);
		fPlayerY = fPlayerY * m_pTexInfo->tImgInfo.Height / fMiniY + (WINCY - 85.f - m_pTexInfo->tImgInfo.Height / 2.25f);

		if (1025 > fPlayerX || 1265 < fPlayerX)
			return;

		fCenterX = m_pTexInfoHero->tImgInfo.Width * 0.5f;
		fCenterY = m_pTexInfoHero->tImgInfo.Height * 0.5f;

		D3DXMATRIX matWorld;
		D3DXMatrixTranslation(&matWorld, fPlayerX, fPlayerY, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfoRedHero->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMinimap::BlueHeroRender()
{
	float fStartX = 2375.f;
	float fEndX = 7500.f;
	float fStartY = -200.f;
	float fEndY = 1635.f;

	float fMiniX = fEndX - fStartX;
	float fMiniY = fEndY - fStartY;

	float fPlayerX;
	float fPlayerY;

	float fCenterX;
	float fCenterY;

	for (auto& pDest : m_pObjectMgr->Get_Obj(OBJECT::BLUE_HERO))
	{
		if (true == SCAST<CUnit*>(pDest)->Get_boolState(CUnit::STATE::DEAD))
			return;

		fPlayerX = -fStartX + pDest->Get_Pos().x;
		fPlayerY = -fStartY + pDest->Get_Pos().y;

		fPlayerX = fPlayerX * m_pTexInfo->tImgInfo.Width / fMiniX + (WINCX - 135.f - m_pTexInfo->tImgInfo.Width / 2.f);
		fPlayerY = fPlayerY * m_pTexInfo->tImgInfo.Height / fMiniY + (WINCY - 85.f - m_pTexInfo->tImgInfo.Height / 2.25f);

		if (1025 > fPlayerX || 1265 < fPlayerX)
			return;

		fCenterX = m_pTexInfoHero->tImgInfo.Width * 0.5f;
		fCenterY = m_pTexInfoHero->tImgInfo.Height * 0.5f;

		D3DXMATRIX matWorld;
		D3DXMatrixTranslation(&matWorld, fPlayerX, fPlayerY, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfoBlueHero->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

CMinimap * CMinimap::Create()
{
	CMinimap* pInstance = new CMinimap;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
