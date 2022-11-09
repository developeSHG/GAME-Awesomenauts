#include "stdafx.h"
#include "Booster.h"


CBooster::CBooster()
{
}

CBooster::~CBooster()
{
}

int CBooster::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	m_tFrame.fCurrentFrame += m_tFrame.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * m_tFrame.fMoveSpeed;

	if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
	{
		m_tFrame.fCurrentFrame = 0.f;
		m_bIsDead = OBJ_DEAD;
	}

	if (0 == m_iType)
		m_tInfo.vPos.y -= m_pTimeMgr->GetDeltaTime() * m_fSpeed;
	else
		m_tInfo.vPos.y += m_pTimeMgr->GetDeltaTime() * m_fSpeed;

	return NO_EVENT;
}

void CBooster::LateUpdate()
{
	SET_MATRIX_WORLD_NOSCROLL(m_tInfo);					// 월드 매트릭스 설정
}

void CBooster::Render()
{
	if (TRUE == m_bIsDead)
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Booster", m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBooster::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	if(1 == m_iType)
		m_tInfo.vSize = { 1.5f, -1.5f, 0.f };
	m_wstrObjectKey = L"Incubator";
	m_fSpeed = 300.f;

	// 고유변수
	m_tFrame.fCurrentFrame = 0.f;
	m_tFrame.fMaxFrameCount = 8.f;
	m_tFrame.fMoveSpeed = 1.1f;

	return S_OK;
}

void CBooster::Release()
{
}

HRESULT CBooster::LateInit()
{
	return E_NOTIMPL;
}

CBooster * CBooster::Create(D3DXVECTOR3& _vPos, int _i)
{
	CBooster* pInstance = new CBooster;

	pInstance->Set_Type(_i);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(_vPos);

	return pInstance;;
}
