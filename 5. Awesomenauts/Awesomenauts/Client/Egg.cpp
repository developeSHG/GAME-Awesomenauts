#include "stdafx.h"
#include "Egg.h"

#include "Player.h"
#include "Booster.h"

CEgg::CEgg()
{
}

CEgg::~CEgg()
{
}

int CEgg::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	if (true == m_bEgg[INCUBATOR::EGG])
	{
		m_tIncubatorFrame[INCUBATOR::EGG].fCurrentFrame += m_tIncubatorFrame[INCUBATOR::EGG].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tIncubatorFrame[INCUBATOR::EGG].fMoveSpeed;

		if (m_tIncubatorFrame[INCUBATOR::EGG].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::EGG].fMaxFrameCount)
			m_tIncubatorFrame[INCUBATOR::EGG].fCurrentFrame = 0.f;
	}
	if (true == m_bEgg[INCUBATOR::START])
	{
		if (0.f == m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame)
			m_pSoundMgr->MyPlaySound(L"Start.wav", CSoundMgr::CHANNELID::EFFECT);

		m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame += m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tIncubatorFrame[INCUBATOR::START].fMoveSpeed;

		if (m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount - 6.f)
			m_bEgg[INCUBATOR::EGG] = true;

		if (m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount)
		{
			m_tIncubatorFrame[INCUBATOR::START].fCurrentFrame = 0.f;
			m_bEgg[INCUBATOR::START] = false;
		}
	}
	if (true == m_bEgg[INCUBATOR::ARRIVE])
	{
		if (0.f == m_tIncubatorFrame[INCUBATOR::ARRIVE].fCurrentFrame)
		{
			m_pSoundMgr->StopAll();
			m_pSoundMgr->MyPlaySound(L"Arrive.wav", CSoundMgr::CHANNELID::EFFECT);
		}

		m_tIncubatorFrame[INCUBATOR::ARRIVE].fCurrentFrame += m_tIncubatorFrame[INCUBATOR::ARRIVE].fMaxFrameCount* m_pTimeMgr->GetDeltaTime() * m_tIncubatorFrame[INCUBATOR::ARRIVE].fMoveSpeed;

		if (m_tIncubatorFrame[INCUBATOR::ARRIVE].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::ARRIVE].fMaxFrameCount - 3.f)
			CGameObject::LateInit();

		if (m_tIncubatorFrame[INCUBATOR::ARRIVE].fCurrentFrame >= m_tIncubatorFrame[INCUBATOR::ARRIVE].fMaxFrameCount)
		{
			m_tIncubatorFrame[INCUBATOR::ARRIVE].fCurrentFrame = 0.f;
			m_bEgg[INCUBATOR::ARRIVE] = false;
		}
	}

	if (false == m_bEgg[INCUBATOR::EGG])
		return NO_EVENT;

	KeyInput();

	if (true == m_bScroll && false == m_bIsInit && false == m_bMap)
	{
		CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, 1200.f * m_pTimeMgr->GetDeltaTime(), 0.f));
		if(m_tInfo.vPos.y > 290.f)
			m_tInfo.vPos.y += -1200.f * m_pTimeMgr->GetDeltaTime();
		else if(m_tInfo.vPos.y < 700.f)
			m_bTimeCount = true;

		if (904 < CScrollMgr::GetScrollPos().y)
		{
			m_bMap = true;
			float y = CScrollMgr::GetScrollPos().y - 904.f;
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -y, 0.f));
		}
	}
	else if(false == m_bScroll)
	{
		m_tInfo.vPos.y += 700.f * m_pTimeMgr->GetDeltaTime();
		
		if (m_tInfo.vPos.y > 800.f)
			m_bScroll = true;
	}
	else if (true == m_bMap && false == m_bIsInit)
	{
		if(360.f >= m_tInfo.vPos.y)
			m_tInfo.vPos.y += 1100.f * m_pTimeMgr->GetDeltaTime();
		else
		{
			m_bEgg[INCUBATOR::EGG] = false;
			m_bEgg[INCUBATOR::ARRIVE] = true;
			m_bTimeCount = false;
			m_tInfo.vPos.y = 420.f;
		}
	}

	if (true == m_bTimeCount)
		m_fTimeCount += CTimeMgr::GetInstance()->GetDeltaTime();
	
	if (0.15f < m_fTimeCount)
	{
		HRESULT hr = CreateBooster();
		FAILED_CHECK_RETURN(hr, NO_EVENT);
		m_fTimeCount = 0.f;
	}

	return NO_EVENT;
}

void CEgg::LateUpdate()
{
	//CGameObject::UpdateMyRect(m_tRectInfo[BODY].rcRect, 400.f, 698.f);
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
	//CAnimation::FrameMove(m_tFrame.fMoveSpeed);		// 프레임무브
}

void CEgg::Render()
{
	D3DXMATRIX matWorld, matScale, matTrans;
	float fCenterX;
	float fCenterY;

	float _fcx = 120.f;
	float _fcy = 200.f;
	m_tRectInfo[BODY].rcRect.left = LONG(m_tInfo.vPos.x - _fcx / 2.f);
	m_tRectInfo[BODY].rcRect.top = LONG(m_tInfo.vPos.y - _fcy / 2.f) + 85.f;
	m_tRectInfo[BODY].rcRect.right = LONG(m_tInfo.vPos.x + _fcx / 2.f);
	m_tRectInfo[BODY].rcRect.bottom = LONG(m_tInfo.vPos.y + _fcy / 2.f) + 85.f;
#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 55, 255, 55))
#endif
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
		D3DXMatrixTranslation(&matTrans, 655.f - CScrollMgr::GetScrollPos().x, -SPACE_Y - CScrollMgr::GetScrollPos().y + SPACE_START_Y / 2.f + 45.f, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (true == m_bEgg[INCUBATOR::ARRIVE])
	{
		SET_TEX_INFO_MULTI(L"Incubator", L"Arrive", m_tIncubatorFrame[INCUBATOR::ARRIVE].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
		fCenterY = float(m_pTexInfo->tImgInfo.Height * 0.5f);

		D3DXMatrixScaling(&matScale, 1.4f, 1.4f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 80.f, 0.f);
		matWorld = matScale * matTrans;	

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	SET_TEX_INFO_SINGLE(L"GreenLine");
	NULL_CHECK(m_pTexInfo);

	fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	fCenterY = float(m_pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 905.f - CScrollMgr::GetScrollPos().x, 0.f - CScrollMgr::GetScrollPos().y, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
	&D3DXVECTOR3(fCenterX, fCenterY - 500.f, 0.f), nullptr, D3DCOLOR_ARGB(225, 255, 255, 255));

}

HRESULT CEgg::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vPos = { 656.f, 400.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Egg";
	m_fSpeed = 800.f;

	// 고유변수
	for (int i = 0; i < INCUBATOR::IN_END; ++i)
	{
		m_tIncubatorFrame[i].fCurrentFrame = 0.f;
		m_tIncubatorFrame[i].fMoveSpeed = 1.f;
		m_bEgg[i] = false;
	}
	m_tIncubatorFrame[INCUBATOR::ARRIVE].fMoveSpeed = 1.25f;
	m_tIncubatorFrame[INCUBATOR::EGG].fMoveSpeed = 4.f;
	m_tIncubatorFrame[INCUBATOR::START].fMaxFrameCount = 10.f;
	m_tIncubatorFrame[INCUBATOR::EGG].fMaxFrameCount = 4.f;
	m_tIncubatorFrame[INCUBATOR::ARRIVE].fMaxFrameCount = 6.f;
	m_bEgg[INCUBATOR::START] = true;
	m_bScroll = false;
	m_bMap = false;
	m_fTimeCount = 0.f;
	m_bTimeCount = false;

	return S_OK;
}

void CEgg::Release()
{
}

HRESULT CEgg::LateInit()
{
	SET_TEX_INFO_SINGLE(L"Egg");
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	float fCenterX = m_pTexInfo->tImgInfo.Width*0.25f * 0.5f;

	m_pObjectMgr->Get_Player()->Set_Pos(D3DXVECTOR3(m_tInfo.vPos.x - fCenterX + 47.5f, 479.f, 0.f));
	CScrollMgr::SetSpaceScroll();


	m_pSoundMgr->PlayBGM(L"Bgm.mp3");

//	CSoundMgr::GetInstance()->MyPlaySound(L"bgm43.wav", CSoundMgr::CHANNELID::BGM);


	return S_OK;
}

void CEgg::KeyInput()
{
	if (false == CScrollMgr::GetSpaceScroll())
		return; 

	if (m_pKeyMgr->KeyPressing(KEY_A))
	{
		if (-257.5f < CScrollMgr::GetScrollPos().x && -2500.f > CScrollMgr::GetScrollPos().y)
			CScrollMgr::SetScrollPos(D3DXVECTOR3(-m_fSpeed * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
		else if (-100.f < CScrollMgr::GetScrollPos().x && -2500.f < CScrollMgr::GetScrollPos().y)
			CScrollMgr::SetScrollPos(D3DXVECTOR3(-m_fSpeed * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
	}
	else if (m_pKeyMgr->KeyPressing(KEY_D))
	{
		if (257.5f > CScrollMgr::GetScrollPos().x && -2500.f > CScrollMgr::GetScrollPos().y)
			CScrollMgr::SetScrollPos(D3DXVECTOR3(m_fSpeed * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
		else if (100.f > CScrollMgr::GetScrollPos().x && -2500.f < CScrollMgr::GetScrollPos().y)
			CScrollMgr::SetScrollPos(D3DXVECTOR3(m_fSpeed * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
	}
	if (-2500.f < CScrollMgr::GetScrollPos().y)
	{
		if (-10.f > CScrollMgr::GetScrollPos().x)
			CScrollMgr::SetScrollPos(D3DXVECTOR3(950.f * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
		else if (10.f < CScrollMgr::GetScrollPos().x)
			CScrollMgr::SetScrollPos(D3DXVECTOR3(-950.f * m_pTimeMgr->GetDeltaTime(), 0.f, 0.f));
	}
}

HRESULT CEgg::CreateBooster()
{
	CGameObject* pGameObject = CBooster::Create(D3DXVECTOR3(m_tInfo.vPos.x - 35.f, m_tInfo.vPos.y - 25.f, 0.f));
	NULL_CHECK_MSG_RETURN(pGameObject, L"Booster Create Failed", E_FAIL);
	HRESULT hr = m_pObjectMgr->AddObject(OBJECT::TYPE::BOOSTER, pGameObject);
	FAILED_CHECK_MSG_RETURN(hr, L"Booster AddObject Failed", E_FAIL);

	return S_OK;
}

CEgg * CEgg::Create()
{
	CEgg* pInstance = new CEgg;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;;
}
