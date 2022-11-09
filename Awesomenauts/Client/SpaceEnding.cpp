#include "stdafx.h"
#include "SpaceEnding.h"

#include "Booster.h"

CSpaceEnding::CSpaceEnding()
{
}


CSpaceEnding::~CSpaceEnding()
{
}

int CSpaceEnding::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;
	//cout << CScrollMgr::GetScrollPos().x << endl;
	// 컬링 시작
	if (7500.f < CScrollMgr::GetScrollPos().x)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	if (true == m_bTimeCount)
		m_fTimeCount += CTimeMgr::GetInstance()->GetDeltaTime();

	if (0.15f < m_fTimeCount)
	{
		HRESULT hr = CreateBooster();
		FAILED_CHECK_RETURN(hr, NO_EVENT);
		m_fTimeCount = 0.f;
	}
	if (true == m_bEgg[INCUBATOR::EGG])
	{
		if (-4650.f < CScrollMgr::GetScrollPos().y)
		{
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -1500.f * m_pTimeMgr->GetDeltaTime(), 0.f));
			m_fY -= -1500.f * m_pTimeMgr->GetDeltaTime();
			m_bTimeCount = true;
		}
		else
			m_bGo = true;
		if (-2000.f > CScrollMgr::GetScrollPos().y)
		{
			if (8369.f < CScrollMgr::GetScrollPos().x)
				CScrollMgr::SetScrollPos(D3DXVECTOR3(-500.f * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
		}

		m_tIncubatorFrame[INCUBATOR::EGG].fCurrentFrame += m_tIncubatorFrame[INCUBATOR::EGG].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tIncubatorFrame[INCUBATOR::EGG].fMoveSpeed;

		if (m_tIncubatorFrame[INCUBATOR::EGG].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::EGG].fMaxFrameCount)
			m_tIncubatorFrame[INCUBATOR::EGG].fCurrentFrame = 0.f;
	}
	if (true == m_bEgg[INCUBATOR::START])
	{
		m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame += m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tIncubatorFrame[INCUBATOR::START].fMoveSpeed;

		if (m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount - 6.f)
			m_bEgg[INCUBATOR::EGG] = true;

		if (m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount)
		{
			m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame = 0.f;
			m_bEgg[INCUBATOR::START] = false;
		}
	}

	if (true == m_bGo)
	{
		m_tInfo.vPos.y -= 1600.f * m_pTimeMgr->GetDeltaTime();
		m_fLogoCount += m_pTimeMgr->GetDeltaTime();
	}

	if (1.f < m_fLogoCount)
	{
		if (m_iAlpha < 250)
			m_iAlpha += m_pTimeMgr->GetDeltaTime() * 250.f;
		else
			m_iAlpha = 255;
	}


	return NO_EVENT;
}

void CSpaceEnding::LateUpdate()
{
	if (false == m_bCulling)
		return;

	if (GetAsyncKeyState(VK_RETURN))
	{
		m_bEgg[INCUBATOR::START] = true;
		CScrollMgr::SetSpaceScroll(true);
	}

	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
	//SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CSpaceEnding::Render()
{
	if (false == m_bCulling)
		return;

	SpaceRender();
	EggRender();
	LogoRender();
}

HRESULT CSpaceEnding::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { m_pObjectMgr->Get_Player()->Get_Pos().x, m_pObjectMgr->Get_Player()->Get_Pos().y, 0.f };
	m_tInfo.vSize = { 1.f, -1.f, 0.f };
	m_wstrObjectKey = L"Egg";
	m_fSpeed = 800.f;

	// 고유변수	// 고유변수
	for (int i = 0; i < INCUBATOR::IN_END; ++i)
	{
		m_tIncubatorFrame[i].fCurrentFrame = 0.f;
		m_tIncubatorFrame[i].fMoveSpeed = 1.f;
		m_bEgg[i] = false;
	}

	m_tIncubatorFrame[INCUBATOR::EGG].fMoveSpeed = 4.f;
	m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount = 10.f;
	m_tIncubatorFrame[INCUBATOR::EGG].fMaxFrameCount = 4.f;

	m_bGo = false;
	m_fTimeCount = 0.f;
	m_bTimeCount = false;
	m_fLogoCount = 0.f;

	return S_OK;
}

void CSpaceEnding::Release()
{
}

HRESULT CSpaceEnding::LateInit()
{
	return E_NOTIMPL;
}

void CSpaceEnding::SpaceRender()
{
	TEX_INFO* pTexInfo = nullptr;
	D3DXMATRIX matWorld, matScale, matTrans;

	pTexInfo = m_pTextureMgr->GetTexInfo(L"SpaceEnding");

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 8050.f - CScrollMgr::GetScrollPos().x, 0.f - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0, fCenterY * 1, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CSpaceEnding::EggRender()
{
	D3DXMATRIX matWorld, matScale, matTrans;
	float fCenterX;
	float fCenterY;
	TEX_INFO* pTexInfo = nullptr;
	 
	if (true == m_bEgg[INCUBATOR::EGG])
	{
		SET_TEX_INFO_MULTI(L"Incubator", L"Egg", m_tIncubatorFrame[INCUBATOR::EGG].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
		fCenterY = float(m_pTexInfo->tImgInfo.Height * 0.5f);

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	if (true == m_bEgg[INCUBATOR::START])
	{
		SET_TEX_INFO_MULTI(L"Incubator", L"Start", m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
		fCenterY = float(m_pTexInfo->tImgInfo.Height * 0.5f);

		D3DXMatrixScaling(&matScale, 1.15f, 1.15f, 0.f);
		D3DXMatrixTranslation(&matTrans, 655.f, 400.f + m_fY, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (0 != m_fLogoCount)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Title");
		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
		D3DXMatrixTranslation(&matTrans, WINCX / 2.f + 2.f, WINCY / 2.f + 2.f , 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
}

void CSpaceEnding::LogoRender()
{
}

HRESULT CSpaceEnding::CreateBooster()
{
	CGameObject* pGameObject = CBooster::Create(D3DXVECTOR3(m_tInfo.vPos.x - 35.f, m_tInfo.vPos.y - 30.f, 0.f), 1);
	NULL_CHECK_MSG_RETURN(pGameObject, L"Booster Create Failed", E_FAIL);
	HRESULT hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BOOSTER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Booster AddObject Failed", E_FAIL);

	return S_OK;
}

CSpaceEnding * CSpaceEnding::Create()
{
	CSpaceEnding* pInstance = new CSpaceEnding;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

