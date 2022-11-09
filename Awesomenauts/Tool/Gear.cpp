#include "stdafx.h"
#include "Gear.h"

#include "ToolView.h"

CGear::CGear()
{
}

CGear::~CGear()
{
	Release();
}

int CGear::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	return NO_EVENT;
}

void CGear::LateUpdate()
{
}

void CGear::Render()
{
	//for (auto& pInfo : m_listInfo)
	//{
	//	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
	//		L"Gear", strSelectName.GetString(), 0);
	//	NULL_CHECK(m_pTexInfo);

	//	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	//	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	//	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	//	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	//	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
	//		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//	m_pDeviceMgr->GetSprite()->End();
	//}

	for (auto& map : m_mapInfo)
	{
		for (auto& pInfo : map.second)
		{
			const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
				L"Gear", map.first.GetString(), 0);
			NULL_CHECK(pTexInfo);

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			SET_MATRIX_WORLD((*pInfo));					
			m_pDeviceMgr->GetSprite()->SetTransform(&pInfo->matWorld);
			if(pInfo == m_pToolView->m_pPickGear)
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(130, 255, 255, 255));
			else
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
		}
	}
}

void CGear::MiniRender()
{
	for (auto& map : m_mapInfo)
	{
		for (auto& pInfo : map.second)
		{
			const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
				L"Gear", map.first.GetString(), 0);
			NULL_CHECK(pTexInfo);

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			D3DXMATRIX matScale, matTrans;
			D3DXMatrixScaling(&matScale, pInfo->vSize.x / 7.65f, pInfo->vSize.y / 3.45f, 0.f);
			D3DXMatrixTranslation(&matTrans, (pInfo->vPos.x) / 7.65f, (pInfo->vPos.y) / 3.45f, 0.f);
			pInfo->matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&pInfo->matWorld);
			if (pInfo == m_pToolView->m_pPickGear)
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(130, 255, 255, 255));
			else
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

HRESULT CGear::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Gear";

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	//m_wstrStateKey[STATE::IDLE];

	return S_OK;
}

HRESULT CGear::LateInit()
{
	return S_OK;
}

void CGear::Release()
{
	for (auto& map : m_mapInfo)
	{
		for (auto& pInfo : map.second)
		{
			SAFE_DELETE(pInfo);
		}
		map.second.clear();
	}
	m_mapInfo.clear();
}

void CGear::SetToolView(CToolView * pView)
{
	m_pToolView = pView;
}

CGear * CGear::Create(CToolView* pView)
{
	CGear* pInstance = new CGear;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetToolView(pView);

	return pInstance;
}
