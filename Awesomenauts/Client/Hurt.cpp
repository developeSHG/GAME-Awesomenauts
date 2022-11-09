#include "stdafx.h"
#include "Hurt.h"

#include "Player.h"

CHurt::CHurt()
{
}


CHurt::~CHurt()
{
}

int CHurt::Update()
{
	//
	if (0 == m_pObjectMgr->Get_Obj(OBJECT::BLUE_TOWER).size() && false == m_bVictory)
	{
		m_bVictory = true;
		Set_TextActive();
		m_iVictoryAlpha = 255;
		m_pSoundMgr->MyPlaySound(L"Clear.wav", CSoundMgr::CHANNELID::EFFECT);
	}

	if (true == m_bVictory)
	{
		m_fVictoryCount += m_pTimeMgr->GetDeltaTime();

		if (3.5f < m_fVictoryCount)
			m_iVictoryAlpha -= int(m_pTimeMgr->GetDeltaTime() * 250.f);

		if (5 > m_iVictoryAlpha)
		{
			m_iVictoryAlpha = 0;
			m_fVictoryCount = 0.f;
		}
	}
	
	if (true == m_bHurtActive)
	{
		m_iAlpha -= int(m_pTimeMgr->GetDeltaTime() * 200.f);

		if (5 > m_iAlpha)
		{
			m_iAlpha = 0;
			m_bHurtActive = false;
		}
	}

	//
	if (true == m_bSkillTextActive)
	{
		m_fSkillTextCount += m_pTimeMgr->GetDeltaTime();

		if (1.3f < m_fSkillTextCount)
			m_iSkillTextAlpha -= int(m_pTimeMgr->GetDeltaTime() * 300.f);

		if (5 > m_iSkillTextAlpha)
		{
			m_iSkillTextAlpha = 0;
			m_bSkillTextActive = false;
			m_fSkillTextCount = 0.f;
		}
	}
	if (true == m_bSkillActive)
	{
		m_iSkillAlpha -= int(m_pTimeMgr->GetDeltaTime() * 150.f);

		if (5 > m_iSkillAlpha)
		{
			m_iSkillAlpha = 0;
			m_bSkillActive = false;
		}
	}

	//
	if (true == m_bHealActive)
	{
		if (250 < m_fHealAlpha)
		{
			m_fHealAlpha = 251.f;
		}
		else
		{
			m_fHealAlpha += (m_pTimeMgr->GetDeltaTime() * 100.f);
		}
	}
	else
	{
		if (5 > m_fHealAlpha)
		{
			m_fHealAlpha = 0.f;
		}
		else
		{
			m_fHealAlpha += (m_pTimeMgr->GetDeltaTime() * -200.f);
		}
	}

	//
	if (true == m_bLevelUpActive)
	{
		m_iLevelUpAlpha -= int(m_pTimeMgr->GetDeltaTime() * 150.f);
		m_fLevelUpTextCount += m_pTimeMgr->GetDeltaTime();

		if (5 > m_iLevelUpAlpha)
			m_iLevelUpAlpha = 0;

		if(3.5f < m_fLevelUpTextCount)
			m_iLevelUpTextAlpha -= int(m_pTimeMgr->GetDeltaTime() * 250.f);

		if (5 > m_iLevelUpTextAlpha)
		{
			m_iLevelUpTextAlpha = 0;
			m_fLevelUpTextCount = 0.f;
			m_bLevelUpActive = false;
			//CSoundMgr::GetInstance()->MyPlaySound(L"Summons.wav", CSoundMgr::CHANNELID::EFFECT);
		}
	}

	//
	if (true == m_bTowerPos)
	{
		m_fTowerPosTextCount += m_pTimeMgr->GetDeltaTime();

		if (3.5f < m_fTowerPosTextCount)
			m_iTowerPosTextAlpha -= int(m_pTimeMgr->GetDeltaTime() * 250.f);

		if (5 > m_iTowerPosTextAlpha)
		{
			m_iTowerPosTextAlpha = 0;
			m_fTowerPosTextCount = 0.f;
			m_bTowerPos = false;
		}
	}

	//
	if (true == m_bTextActive)
	{
		m_iTextAlpha -= int(m_pTimeMgr->GetDeltaTime() * 150.f);

		if (5 > m_iTextAlpha)
		{
			m_iTextAlpha = 0;
			m_bTextActive = false;
		}
	}

	//
	if (true == m_bDeadCoolTime)
	{
		m_fDeadCoolTimeCount += m_pTimeMgr->GetDeltaTime();

		if (1.f <= m_fDeadCoolTimeCount)
		{
			m_fDeadCoolTimeCount = 0.f;
			m_iDeadCoolTimeFrame += 1;

			if (10 <= m_iDeadCoolTimeFrame)
			{
				m_bDeadCoolTime = false;
				m_iDeadCoolTimeFrame = 0;
			}
		}
	}

	return NO_EVENT;
}

void CHurt::LateUpdate()
{
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
}

void CHurt::Render()
{
	if (true == m_bDeadCoolTime)
	{
		D3DXMATRIX matWorld;
		TEX_INFO* pTexInfo = nullptr;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"DeadCoolTime", L"Time", m_iDeadCoolTimeFrame);

		// 데드쿨타임 텍스트
		D3DXMatrixTranslation(&matWorld, WINCX / 2.f, 100.f, 0.f);
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	if (true == m_bHurtActive)
	{
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 0, 0));
	}

	if (0.f != m_fHealAlpha)
	{
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(int(m_fHealAlpha), 255, 253, 93));
	}


	if (true == m_bSkillTextActive)
	{
		D3DXMATRIX matWorld;
		TEX_INFO* pTexInfo = nullptr;
		switch (m_eSkillType)
		{
		case SKILL_TYPE::ONE:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"SkillText1");
			break;
		case SKILL_TYPE::TWO:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"SkillText2");
			break;
		case SKILL_TYPE::THREE:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"SkillText3");
			break;
		case SKILL_TYPE::FOUR:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"SkillText4");
			break;
		case SKILL_TYPE::TAXI:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"SkillTextTaxi");
			break;
		case SKILL_TYPE::COOLTIME:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"SkillCoolTimeText");
			break;
		}

		// 스킬타입 텍스트
		D3DXMatrixTranslation(&matWorld, WINCX / 2.f, 100.f, 0.f);
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iSkillTextAlpha, 255, 255, 255));
	}
	if (true == m_bSkillActive)
	{
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo_SKill->pTexture, nullptr,
			&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iSkillAlpha, 0, 0, 0));
	}

	if (true == m_bTowerPos)
	{
		D3DXMATRIX matWorld;
		TEX_INFO* pTexInfo = nullptr;
		switch (m_iTowerPos)
		{
		case 0:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"RedTowerUpBreak");
			break;
		case 1:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"RedTowerDownBreak");
			break;
		case 2:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"BlueTowerUpBreak");
			break;
		case 3:
			pTexInfo = m_pTextureMgr->GetTexInfo(L"BlueTowerDownBreak");
			break;
		}

		// 스킬타입 텍스트
		D3DXMatrixTranslation(&matWorld, WINCX / 2.f, 100.f, 0.f);
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iTowerPosTextAlpha, 255, 255, 255));
	}
	if (true == m_bTextActive)
	{
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iTextAlpha, 255, 255, 255));
	}

	if (true == m_bLevelUpActive)
	{
		D3DXMATRIX matWorld;

		// 레벨업 텍스트
		D3DXMatrixTranslation(&matWorld, WINCX / 2.f, 100.f, 0.f);
		float fCenterX = m_pTexInfo_LevelUpText->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo_LevelUpText->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo_LevelUpText->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iLevelUpTextAlpha, 255, 255, 255));

		// 스킬활성화 텍스트
		int iLevel = SCAST<CUnit*>(m_pObjectMgr->Get_Player())->Get_Status().iLevel;
		if (3 == iLevel || 5 == iLevel || 7 == iLevel)
		{
			D3DXMatrixTranslation(&matWorld, WINCX / 2.f, 140.f, 0.f);
			fCenterX = m_pTexInfo_SkillText->tImgInfo.Width * 0.5f;
			fCenterY = m_pTexInfo_SkillText->tImgInfo.Height * 0.5f;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo_SkillText->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iLevelUpTextAlpha, 255, 255, 255));
		}

		// 레벨업 헐트효과
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iLevelUpAlpha, 255, 255, 255));
	}

	if (true == m_bVictory)
	{
		D3DXMATRIX matWorld;
		TEX_INFO* pTexInfo = nullptr;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"RedVictory");

		// 텍스트
		D3DXMatrixTranslation(&matWorld, WINCX / 2.f, 100.f, 0.f);
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iVictoryAlpha, 255, 255, 255));
	}
}

HRESULT CHurt::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Hurt";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	m_pTexInfo_SKill = m_pTextureMgr->GetTexInfo(L"SkillActive");
	NULL_CHECK_RETURN(m_pTexInfo_SKill, E_FAIL);

	m_pTexInfo_LevelUpText = m_pTextureMgr->GetTexInfo(L"LevelUpText");
	NULL_CHECK_RETURN(m_pTexInfo_LevelUpText, E_FAIL);

	m_pTexInfo_SkillText = m_pTextureMgr->GetTexInfo(L"SkillLearn");
	NULL_CHECK_RETURN(m_pTexInfo_SkillText, E_FAIL);

	// 고유변수
	m_bHurtActive = false;
	m_iAlpha = 0;

	m_bSkillActive = false;
	m_iSkillAlpha = 0;

	m_bHealActive = false;
	m_fHealAlpha = 0.f;

	m_bLevelUpActive = false;
	m_iLevelUpAlpha = 0;
	m_iLevelUpTextAlpha = 0;
	m_fLevelUpTextCount = 0.f;

	m_bTextActive = false;
	m_iTextAlpha = 0;

	m_bSkillTextActive = false;
	m_iSkillTextAlpha = 0;
	m_fSkillTextCount = 0.f;

	m_bDeadCoolTime = false;
	m_iDeadCoolTimeFrame = 0.f;
	m_fDeadCoolTimeCount = 0;

	m_bTowerPos = false;
	m_iTowerPos = 0;
	m_iTowerPosTextAlpha = 0;
	m_fTowerPosTextCount = 0.f;

	m_iVictoryAlpha = 0;
	m_fVictoryCount = 0.f;
	m_bVictory = false;

	return S_OK;
}

void CHurt::Release()
{
}

CHurt * CHurt::Create()
{
	CHurt* pInstance = new CHurt;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
