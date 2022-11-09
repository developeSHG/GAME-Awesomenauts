#include "stdafx.h"
#include "BluePortal.h"


CBluePortal::CBluePortal()
{
}


CBluePortal::~CBluePortal()
{
}

int CBluePortal::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// �ø� ����
	if (6000.f < CScrollMgr::GetScrollPos().x && 7600.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// �ø� ��

	CAnimation::FrameMove(m_tFrame.fMoveSpeed);	// �����ӹ���

	return NO_EVENT;
}

void CBluePortal::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// ���� ��Ʈ���� ����
}

void CBluePortal::Render()
{
	if (false == m_bCulling)
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Blue", m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBluePortal::Initialize()
{
	// CGameObject ��Ӻ��� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
	m_tInfo.vPos = { 7454.f, 1660.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Portal";

	// CAnimation, �������� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
	//m_tFrame.fCurrentFrame = m_tFrame.fCurrentFrame;
	m_tFrame.fCurrentFrame = 0.f;
	m_tFrame.fMaxFrameCount = 8.f;
	m_tFrame.fMoveSpeed = 1.2f;

	return S_OK;
}

void CBluePortal::Release()
{
}

CBluePortal * CBluePortal::Create()
{
	CBluePortal* pInstance = new CBluePortal;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}