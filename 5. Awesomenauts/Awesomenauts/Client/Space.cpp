#include "stdafx.h"
#include "Space.h"

#include "Egg.h"

CSpace::CSpace()
{
}


CSpace::~CSpace()
{
}

int CSpace::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	if (true == m_bPlayStart)
	{
		m_iAlphaLoading = 0;
		m_iAlphaTitle = 255;
		if (1 < m_iAlphaLoadingComplete)
			m_iAlphaLoadingComplete -= 3;

		if (3 <= m_iAlphaPlay)
			m_iAlphaPlay -= 3;

		if (1 > m_iAlphaLoadingComplete)
		{
			if (-SPACE_Y < CScrollMgr::GetScrollPos().y && true == m_bShipGo)
			{
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -300.f * m_pTimeMgr->GetDeltaTime(), 0.f));
				m_fTitleScroll += -300.f * m_pTimeMgr->GetDeltaTime();
			}
			else
			{
				if (0 == m_iAlphaShip2)
					m_pSoundMgr->MyPlaySound(L"Skill.wav", CSoundMgr::CHANNELID::EFFECT);
				m_fTitleScroll = 9999.f;
				if (254 >= m_iAlphaShip2)
					m_iAlphaShip2 += 3;
				else
					m_iAlphaShip1 = 0;

				if (150 <= m_iAlphaShip2)
					m_bSummons = true;

				m_bShipGo = false; 
			}
		}

		////////////////////////////////////////

		if (true == m_bSummons)
		{
			if (2 <= m_iIncubator)
				CGameObject::LateInit();
			if (5 < m_iIncubator)
				m_bSummons = false;

			if (true == m_bHole[SUMMONS::HOLE])
			{
				

				m_tSummonsFrame[SUMMONS::HOLE].fCurrentFrame += m_tSummonsFrame[SUMMONS::HOLE].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tSummonsFrame[SUMMONS::HOLE].fMoveSpeed;

				if (m_tSummonsFrame[SUMMONS::HOLE].fCurrentFrame >= m_tSummonsFrame[SUMMONS::HOLE].fMaxFrameCount)
				{
					m_tSummonsFrame[SUMMONS::HOLE].fCurrentFrame = 0.f;
					m_iIncubator++;
				}
			}
			if (true == m_bHole[SUMMONS::S2])
			{
				if (m_tSummonsFrame[SUMMONS::S2].fMaxFrameCount - 3.8f >= m_tSummonsFrame[SUMMONS::S2].fCurrentFrame
					&& m_tSummonsFrame[SUMMONS::S2].fMaxFrameCount - 4.2f <= m_tSummonsFrame[SUMMONS::S2].fCurrentFrame)
					m_pSoundMgr->MyPlaySound(L"Hole.ogg", CSoundMgr::CHANNELID::EFFECT);

				m_tSummonsFrame[SUMMONS::S2].fCurrentFrame += m_tSummonsFrame[SUMMONS::S2].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tSummonsFrame[SUMMONS::S2].fMoveSpeed;

				if (m_tSummonsFrame[SUMMONS::S2].fCurrentFrame >= m_tSummonsFrame[SUMMONS::S2].fMaxFrameCount - 4.f)
					m_bHole[SUMMONS::HOLE] = true;

				if (m_tSummonsFrame[SUMMONS::S2].fCurrentFrame >= m_tSummonsFrame[SUMMONS::S2].fMaxFrameCount)
				{
					m_bHole[SUMMONS::S2] = false;
					m_tSummonsFrame[SUMMONS::S2].fCurrentFrame = 0.f;
				}
			}
			if (true == m_bHole[SUMMONS::S1])
			{
				if (m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount - 8.7f >= m_tSummonsFrame[SUMMONS::S1].fCurrentFrame
					&& m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount - 9.3f <= m_tSummonsFrame[SUMMONS::S1].fCurrentFrame
					|| m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount - 4.7f >= m_tSummonsFrame[SUMMONS::S1].fCurrentFrame
					&& m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount - 5.3f <= m_tSummonsFrame[SUMMONS::S1].fCurrentFrame)
					m_pSoundMgr->MyPlaySound(L"Summons.wav", CSoundMgr::CHANNELID::EFFECT);

				m_tSummonsFrame[SUMMONS::S1].fCurrentFrame += m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tSummonsFrame[SUMMONS::S1].fMoveSpeed;

				if (m_tSummonsFrame[SUMMONS::S1].fCurrentFrame >= m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount - 1.f)
					m_bHole[SUMMONS::S2] = true;

				if (m_tSummonsFrame[SUMMONS::S1].fCurrentFrame >= m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount)
				{
					m_bHole[SUMMONS::S1] = false;
					m_tSummonsFrame[SUMMONS::S1].fCurrentFrame = 0.f;
				}
			}
		}

		return NO_EVENT;
	}

	if (254 >= m_iAlphaLoadingComplete)
	{
		m_iAlphaLoadingComplete += 3;
		m_iAlphaTitle += 3;
	}

	if (255 <= m_iAlphaPlay)
		m_iAlphaSpeed = -5;
	else if (0 >= m_iAlphaPlay)
		m_iAlphaSpeed = 5;
	m_iAlphaPlay += m_iAlphaSpeed;


	return NO_EVENT;
}

void CSpace::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_bPlayStart = true;
	}
}

void CSpace::Render()
{
	SpaceRender();
	SummonsRender();
	LoadingRender();
	
}

HRESULT CSpace::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 0, 0, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Space1";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	m_fSpeed = 400.f;

	// 고유변수
	m_bShipGo = true;
	m_iAlphaLoading = 255;
	m_iAlphaLoadingComplete = 0;
	m_iAlphaTitle = 0;
	m_iAlphaPlay = 255;
	m_iAlphaSpeed = 5;
	m_bPlayStart = false;
	m_iAlphaShip1 = 255;
	m_iAlphaShip2 = 0;
	m_fTitleScroll = 0.f;
	for (int i = 0; i < SUMMONS::END; ++i)
	{
		m_tSummonsFrame[i].fCurrentFrame = 0.f;
		m_tSummonsFrame[i].fMoveSpeed = 1.f;
		m_bHole[i] = false;
	}
	m_tSummonsFrame[SUMMONS::S2].fMoveSpeed = 1.3f;
	m_tSummonsFrame[SUMMONS::HOLE].fMoveSpeed = 1.3f;
	m_tSummonsFrame[SUMMONS::S1].fMaxFrameCount = 11.f;
	m_tSummonsFrame[SUMMONS::S2].fMaxFrameCount = 7.f;
	m_tSummonsFrame[SUMMONS::HOLE].fMaxFrameCount = 7.f;
	m_bSummons = false;
	m_bHole[SUMMONS::S1] = true;
	m_iIncubator = 0;

	return S_OK;
}

void CSpace::Release()
{
}

HRESULT CSpace::LateInit()
{
	CGameObject* pGameObject = CEgg::Create();
	NULL_CHECK_MSG_RETURN(pGameObject, L"Egg Create Failed", E_FAIL);
	HRESULT hr = m_pObjectMgr->AddObject(OBJECT::TYPE::INCUBATOR, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Egg AddObject Failed", E_FAIL);

	return S_OK;
}

void CSpace::SpaceRender()
{
	D3DXMATRIX matWorld, matScale, matTrans;

	//TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Space");
	//float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	//float fCenterY = pTexInfo->tImgInfo.Height;

	//D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	//D3DXMatrixTranslation(&matTrans, 0 - CScrollMgr::GetScrollPos().x, 0 - CScrollMgr::GetScrollPos().y, 0.f);
	//matWorld = matScale * matTrans;

	//m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	////m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	//m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
	//	&D3DXVECTOR3(0, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Space1");
	float fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollMgr::GetScrollPos().x, 0 - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, fCenterY * 3, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Space2");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollMgr::GetScrollPos().x, 0 - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, fCenterY * 2, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Space3");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollMgr::GetScrollPos().x, 0 - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, fCenterY * 1, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Star1");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0 - CScrollMgr::GetScrollPos().x, 0 - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, fCenterY * 3 , 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Star2");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, fCenterY * 2, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Star3");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Ship1");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.15f, 1.f, 0.f);		
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;
	 
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X + 345.5f, fCenterY * 3 + 10.f, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlphaShip1, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Ship2");
	fCenterX = float(pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(SPACE_X + 345.5f, fCenterY * 3 + 10.f, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlphaShip2, 255, 255, 255));
}

void CSpace::SummonsRender()
{
	if (false == m_bSummons)
		return;

	D3DXMATRIX matWorld, matScale, matTrans;

	if (true == m_bHole[SUMMONS::HOLE])
	{
		SET_TEX_INFO_MULTI(L"Summons", L"Hole", m_tSummonsFrame[SUMMONS::HOLE].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 670.f - CScrollMgr::GetScrollPos().x, -SPACE_Y - CScrollMgr::GetScrollPos().y + SPACE_START_Y/2.f + 40.f, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (true == m_bHole[SUMMONS::S2])
	{
		SET_TEX_INFO_MULTI(L"Summons", L"2", m_tSummonsFrame[SUMMONS::S2].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 0.f);
		D3DXMatrixTranslation(&matTrans, 665.f - CScrollMgr::GetScrollPos().x, -SPACE_Y - CScrollMgr::GetScrollPos().y + SPACE_START_Y / 2.f + 40.f, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (true == m_bHole[SUMMONS::S1])
	{
		SET_TEX_INFO_MULTI(L"Summons", L"1", m_tSummonsFrame[SUMMONS::S1].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 0.f);
		D3DXMatrixTranslation(&matTrans, 600.f - CScrollMgr::GetScrollPos().x, -SPACE_Y - CScrollMgr::GetScrollPos().y + SPACE_START_Y / 2.f + 45.f, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CSpace::LoadingRender()
{
	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Loading");

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 0, 0.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlphaLoading, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"LoadingComplete");
	D3DXMatrixTranslation(&matTrans, 0, 0.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlphaLoadingComplete, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Title");
	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f + 2.f, WINCY / 2.f + 2.f - m_fTitleScroll, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlphaTitle, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Play");
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f - 200.f, WINCY - 240.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlphaPlay, 255, 255, 255));
}

CSpace * CSpace::Create()
{
	CSpace* pInstance = new CSpace;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
