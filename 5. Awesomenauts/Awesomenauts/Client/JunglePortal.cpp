#include "stdafx.h"
#include "JunglePortal.h"


CJunglePortal::CJunglePortal()
{
}


CJunglePortal::~CJunglePortal()
{
}

int CJunglePortal::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	// �ø� ����
	if (3400.f < CScrollMgr::GetScrollPos().x && 5300.f > CScrollMgr::GetScrollPos().x
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

void CJunglePortal::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// ���� ��Ʈ���� ����
}

void CJunglePortal::Render()
{
	if (false == m_bCulling)
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Jungle", m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = float(m_pTexInfo->tImgInfo.Width * 0.5f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CJunglePortal::Initialize()
{
	// CGameObject ��Ӻ��� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
	//m_tInfo.vPos = { 4945.f, 1241.f, 0.f };
	m_tInfo.vPos = { 4950.f, 1530.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_wstrObjectKey = L"Portal";

	// CAnimation, �������� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
	m_tFrame.fCurrentFrame = 0.f;
	m_tFrame.fMaxFrameCount = 14.f;
	m_tFrame.fMoveSpeed = 1.f;

	return S_OK;
}

void CJunglePortal::Release()
{
}

CJunglePortal * CJunglePortal::Create()
{
	CJunglePortal* pInstance = new CJunglePortal;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}