#include "stdafx.h"
#include "MyTable.h"

#include "Hero.h"

CMyTable::CMyTable()
{
}

CMyTable::~CMyTable()
{
}

int CMyTable::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	m_bActive = false;
	if (m_pKeyMgr->KeyPressing(KEY_1))
		m_bActive = true;

	return NO_EVENT;
}

void CMyTable::LateUpdate()
{
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
}

void CMyTable::Render()
{
	if (true == CScrollMgr::GetSpaceScroll() || false == m_bActive)
		return;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	RedHeroRender();
	BlueHeroRender();
}

HRESULT CMyTable::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { WINCX / 2.f - 15.f, WINCY / 2.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"MyTable";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// 고유변수
	m_bActive = false;

	return S_OK;
}

HRESULT CMyTable::LateInit()
{
	return S_OK;
}

void CMyTable::Release()
{
}

void CMyTable::RedHeroRender()
{
	D3DXMATRIX matTrans;
	CHero* pObject = SCAST<CHero*>(m_pObjectMgr->Get_Player());

	swprintf_s(m_szBuf, L"%d", m_pObjectMgr->Get_Obj(OBJECT::RED_TOWER).size());
	D3DXMatrixTranslation(&matTrans, 442.f, 303.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();

	// 레벨
	swprintf_s(m_szBuf, L"%d", pObject->Get_Status().iLevel);
	D3DXMatrixTranslation(&matTrans, 210.f, 406.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();

	// HP
	swprintf_s(m_szBuf, L"%d", int(pObject->Get_Status().fHp));
	D3DXMatrixTranslation(&matTrans, 255.f, 406.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();

	// 공격력
	swprintf_s(m_szBuf, L"%d", int(pObject->Get_Status().fAttack));
	D3DXMatrixTranslation(&matTrans, 300.f, 406.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();

	// 데드
	swprintf_s(m_szBuf, L"%d", int(pObject->Get_Ability().iDead));
	D3DXMatrixTranslation(&matTrans, 360.f, 406.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();

	// 히어로킬
	swprintf_s(m_szBuf, L"%d", int(pObject->Get_Ability().iKillHero));
	D3DXMatrixTranslation(&matTrans, 425.f, 406.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();

	// 미니언킬
	swprintf_s(m_szBuf, L"%d", int(pObject->Get_Ability().iKillMinion));
	D3DXMatrixTranslation(&matTrans, 500.f, 406.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();
}

void CMyTable::BlueHeroRender()
{
	D3DXMATRIX matTrans;

	swprintf_s(m_szBuf, L"%d", m_pObjectMgr->Get_Obj(OBJECT::BLUE_TOWER).size());
	D3DXMatrixTranslation(&matTrans, 994.f, 303.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
		CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->GetSprite()->End();


	for (auto& listObject : m_pObjectMgr->Get_Obj(OBJECT::BLUE_HERO))
	{
		CHero* pObject = SCAST<CHero*>(listObject);

		// 레벨
		swprintf_s(m_szBuf, L"%d", pObject->Get_Status().iLevel);
		D3DXMatrixTranslation(&matTrans, 795.f, 406.f, 0.f);

		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
			CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		CDeviceMgr::GetInstance()->GetSprite()->End();

		// HP
		swprintf_s(m_szBuf, L"%d", int(pObject->Get_Status().fHp));
		D3DXMatrixTranslation(&matTrans, 835.f, 406.f, 0.f);

		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
			CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		CDeviceMgr::GetInstance()->GetSprite()->End();

		// 공격력
		swprintf_s(m_szBuf, L"%d", int(pObject->Get_Status().fAttack));
		D3DXMatrixTranslation(&matTrans, 880.f, 406.f, 0.f);

		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
			CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		CDeviceMgr::GetInstance()->GetSprite()->End();

		// 데드
		swprintf_s(m_szBuf, L"%d", int(pObject->Get_Ability().iDead));
		D3DXMatrixTranslation(&matTrans, 940.f, 406.f, 0.f);

		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
			CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		CDeviceMgr::GetInstance()->GetSprite()->End();

		// 히어로킬
		swprintf_s(m_szBuf, L"%d", int(pObject->Get_Ability().iKillHero));
		D3DXMatrixTranslation(&matTrans, 1005.f, 406.f, 0.f);

		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
			CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		CDeviceMgr::GetInstance()->GetSprite()->End();

		// 미니언킬
		swprintf_s(m_szBuf, L"%d", int(pObject->Get_Ability().iKillMinion));
		D3DXMatrixTranslation(&matTrans, 1080.f, 406.f, 0.f);

		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDeviceMgr::GetInstance()->GetFont()->DrawTextW(
			CDeviceMgr::GetInstance()->GetSprite(), m_szBuf, lstrlen(m_szBuf),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		CDeviceMgr::GetInstance()->GetSprite()->End();
	}
}

CMyTable * CMyTable::Create()
{
	CMyTable* pInstance = new CMyTable;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
