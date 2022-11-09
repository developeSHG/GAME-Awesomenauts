#include "stdafx.h"
#include "Door.h"

#include "Unit.h"
#include "Goblin.h"

CDoor::CDoor()
{
}


CDoor::~CDoor()
{
}

int CDoor::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	D3DXVECTOR3 vDist;
	bool bOpen = false;
	for (int i = 0; i < TYPE::TYPE_END; ++i)
	{
		D3DXVECTOR3 vPlayerPos = {
			m_pObjectMgr->Get_Player()->Get_Pos().x + CScrollMgr::GetScrollPos().x,
			m_pObjectMgr->Get_Player()->Get_Pos().y + CScrollMgr::GetScrollPos().y,
			0.f
		};
		vDist = vPlayerPos - m_tDoorInfo[i].vPos;

		if (165.f > D3DXVec3Length(&vDist))
			bOpen = true;

		if (false == bOpen)
		{
			for (auto& pRedUnit : m_pObjectMgr->Get_RedUnitList())
			{
				vDist = pRedUnit->Get_Pos() - m_tDoorInfo[i].vPos;

				if (165.f > D3DXVec3Length(&vDist))
				{
					bOpen = true;
					break;
				}
			}
			if (false == bOpen)
			{
				for (auto& pBlueUnit : m_pObjectMgr->Get_BlueUnitList())
				{
					vDist = pBlueUnit->Get_Pos() - m_tDoorInfo[i].vPos;

					if (165.f > D3DXVec3Length(&vDist))
					{
						bOpen = true;
						break;
					}
				}
			}
		}

		if (true == bOpen)
		{
			m_tDoorFrame[i].fCurrentFrame += m_tDoorFrame[i].fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * m_tDoorFrame[i].fMoveSpeed;

			if (m_tDoorFrame[i].fCurrentFrame >= m_tDoorFrame[i].fMaxFrameCount)
				m_tDoorFrame[i].fCurrentFrame = m_tDoorFrame[i].fMaxFrameCount;

			SET_MATRIX_WORLD(m_tDoorInfo[i]);					// 월드 매트릭스 설정
			continue;
		}
		else
		{
			m_tDoorFrame[i].fCurrentFrame -= m_tDoorFrame[i].fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * m_tDoorFrame[i].fMoveSpeed;

			if (m_tDoorFrame[i].fCurrentFrame <= 0.f)
				m_tDoorFrame[i].fCurrentFrame = 0;
		}
		SET_MATRIX_WORLD(m_tDoorInfo[i]);					// 월드 매트릭스 설정
	}

	return NO_EVENT;
}

void CDoor::LateUpdate()
{
}

void CDoor::Render()
{
	if (200.f < CScrollMgr::GetScrollPos().x && 1800.f > CScrollMgr::GetScrollPos().x)
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Red1", m_tDoorFrame[RED1].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tDoorInfo[RED1].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (1500.f < CScrollMgr::GetScrollPos().x && 3100.f > CScrollMgr::GetScrollPos().x)
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Red2", m_tDoorFrame[RED2].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tDoorInfo[RED2].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (6800.f < CScrollMgr::GetScrollPos().x && 8300.f > CScrollMgr::GetScrollPos().x)
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Blue1", m_tDoorFrame[BLUE1].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tDoorInfo[BLUE1].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (5500.f < CScrollMgr::GetScrollPos().x && 7200.f > CScrollMgr::GetScrollPos().x)
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Blue2", m_tDoorFrame[BLUE2].fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tDoorInfo[BLUE2].matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT CDoor::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_wstrObjectKey = L"Door";

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	m_tDoorInfo[RED1].vPos = { 1650.f, 1368.f, 0.f };
	m_tDoorInfo[RED2].vPos = { 2937.f, 1553.5f, 0.f };
	m_tDoorInfo[BLUE1].vPos = { 8267.f, 1368.f, 0.f };
	m_tDoorInfo[BLUE2].vPos = { 6980.f, 1553.5f, 0.f };
	for (int i = 0; i<TYPE::TYPE_END; ++i)
		m_tDoorInfo[i].vSize = { 1.4f, 1.4f, 0.f };
	m_tDoorFrame[RED1].fCurrentFrame = 0.f;
	m_tDoorFrame[RED1].fMaxFrameCount = 17.f;
	m_tDoorFrame[RED1].fMoveSpeed = 3.f;
	for (int i = 1; i < TYPE::TYPE_END; ++i)
	{
		m_tDoorFrame[i].fCurrentFrame = 0.f;
		m_tDoorFrame[i].fMaxFrameCount = 16.f;
		m_tDoorFrame[i].fMoveSpeed = 3.f;
	}


	return S_OK;
}

void CDoor::Release()
{
}

CDoor * CDoor::Create()
{
	CDoor* pInstance = new CDoor;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}