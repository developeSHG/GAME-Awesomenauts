#include "stdafx.h"
#include "Status.h"

#include "Player.h"
#include "Hurt.h"

CStatus::CStatus()
{
}

CStatus::~CStatus()
{
}

int CStatus::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	if (false == m_bGameStart)
	{
		if (1950.f < CScrollMgr::GetScrollPos().x)
		{
			m_bGameStart = true;
			m_bGameRender = true;
			SCAST<CHurt*>(m_pObjectMgr->Get_Hurt())->Set_TextActive();
			CSoundMgr::GetInstance()->MyPlaySound(L"GameStart.mp3", CSoundMgr::CHANNELID::BGM);
		}
	}
	else if(true == m_bGameStart)
	{
		if (5 > m_iAlpha)
			m_bGameRender = false;

		if (true == m_bGameRender)
		{
			m_fTimeCount += m_pTimeMgr->GetDeltaTime();

			if (7.f < m_fTimeCount)
				m_iAlpha -= m_pTimeMgr->GetDeltaTime() * 200;	
		}
	}

	return NO_EVENT;
}

void CStatus::LateUpdate()
{
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
}

void CStatus::Render()
{
	if (true == CScrollMgr::GetSpaceScroll())
		return;
	else
	{
		if(800 < CScrollMgr::GetScrollPos().x)
			CGameObject::LateInit();
	}

	m_wstrObjectKey = L"StatusHpBack";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	float fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_wstrObjectKey = L"StatusExpBack";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_wstrObjectKey = L"StatusHp";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);

	fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(m_pTexInfo->tImgInfo.Height);

	float fPasent = float(SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Get_Status().fHp) / float(SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Get_Status().fMaxHp);
	float fWidth = m_pTexInfo->tImgInfo.Width * fPasent;

	RECT rc = {};
	rc.left = 0.f;
	rc.right = fWidth;
	rc.top = 0.f;
	rc.bottom = m_pTexInfo->tImgInfo.Height;

	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, &rc,
		&D3DXVECTOR3(fCenterX, fCenterY + 15.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_wstrObjectKey = L"StatusExp";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);

	fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(m_pTexInfo->tImgInfo.Height);

	fPasent = float(SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Get_Status().fExp) / float(SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Get_Status().fMaxExp);
	fWidth = m_pTexInfo->tImgInfo.Width * fPasent;

	rc.left = 0.f;
	rc.right = fWidth;
	rc.top = 0.f;
	rc.bottom = m_pTexInfo->tImgInfo.Height;

	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, &rc,
		&D3DXVECTOR3(fCenterX, fCenterY + 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_wstrObjectKey = L"Status";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);

	fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (true == m_bGameRender)
	{
		TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"GameStart");
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMATRIX matWorld;
		D3DXMatrixScaling(&matWorld, 1.f, 1.f, 0.f);
		matWorld._41 += WINCX/2.f;
		matWorld._42 += 100.f;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}

	SkillCollTimeRender();
}

HRESULT CStatus::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { WINCX / 2.f - 15.f, WINCY, 0.f };
	m_tInfo.vSize = { 0.95f, 0.95f, 0.f };

	m_bGameStart = false;
	m_bGameRender = false;
	m_iAlpha = 255;

	return S_OK;
}

HRESULT CStatus::LateInit()
{
	m_pObjectMgr->DeleteID(OBJECT::SPACE);
	m_pObjectMgr->DeleteID(OBJECT::SPACEITEM);
	m_pObjectMgr->DeleteID(OBJECT::BOOSTER);
	m_pObjectMgr->DeleteID(OBJECT::INCUBATOR);
	return S_OK;
}

void CStatus::Release()
{
}

void CStatus::SkillCollTimeRender()
{
	D3DXMATRIX matWorld, matScale, matTrans;
	float fCenterX;
	float fCenterY;
	float fPasent;
	float fHeight;

	CHero* pPlayer = SCAST<CHero*>(m_pObjectMgr->Get_Player());
	RECT rc = {};
	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"SkillCoolTime");
	NULL_CHECK(pTexInfo);

	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height * 0.5f);
	
	// 1
	D3DXMatrixScaling(&matScale, 0.95f, 0.95f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f - 122.f, WINCY - 67.f, 0.f);
	matWorld = matScale * matTrans;

	fPasent = pPlayer->Get_SkillCoolTime(SKILL_TYPE::ONE).fCurrentTime / pPlayer->Get_SkillCoolTime(SKILL_TYPE::ONE).fMaxTime;
	fHeight = pTexInfo->tImgInfo.Height * fPasent;

	rc.left = 0.f;
	rc.right = pTexInfo->tImgInfo.Width;
	rc.top = 0.f;
	rc.bottom = fHeight;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 2
	D3DXMatrixScaling(&matScale, 0.98f, 0.98f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f - 51.f, WINCY - 67.f, 0.f);
	matWorld = matScale * matTrans;

	fPasent = pPlayer->Get_SkillCoolTime(SKILL_TYPE::TWO).fCurrentTime / pPlayer->Get_SkillCoolTime(SKILL_TYPE::TWO).fMaxTime;
	fHeight = pTexInfo->tImgInfo.Height * fPasent;

	rc.left = 0.f;
	rc.right = pTexInfo->tImgInfo.Width;
	rc.top = 0.f;
	rc.bottom = fHeight;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 3
	D3DXMatrixScaling(&matScale, 0.98f, 0.98f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f + 21.5f, WINCY - 67.f, 0.f);
	matWorld = matScale * matTrans;

	fPasent = pPlayer->Get_SkillCoolTime(SKILL_TYPE::THREE).fCurrentTime / pPlayer->Get_SkillCoolTime(SKILL_TYPE::THREE).fMaxTime;
	fHeight = pTexInfo->tImgInfo.Height * fPasent;

	rc.left = 0.f;
	rc.right = pTexInfo->tImgInfo.Width;
	rc.top = 0.f;
	rc.bottom = fHeight;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 4
	D3DXMatrixScaling(&matScale, 0.98f, 0.98f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f + 92.f, WINCY - 67.f, 0.f);
	matWorld = matScale * matTrans;

	fPasent = pPlayer->Get_SkillCoolTime(SKILL_TYPE::FOUR).fCurrentTime / pPlayer->Get_SkillCoolTime(SKILL_TYPE::FOUR).fMaxTime;
	fHeight = pTexInfo->tImgInfo.Height * fPasent;

	rc.left = 0.f;
	rc.right = pTexInfo->tImgInfo.Width;
	rc.top = 0.f;
	rc.bottom = fHeight;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CStatus * CStatus::Create()
{
	CStatus* pInstance = new CStatus;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
