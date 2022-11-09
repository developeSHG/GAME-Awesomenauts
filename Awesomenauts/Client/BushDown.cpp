#include "stdafx.h"
#include "BushDown.h"

#include "Player.h"

CBushDown::CBushDown()
{
}


CBushDown::~CBushDown()
{
}

int CBushDown::Update()
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

	CGameObject* pPlayer = m_pObjectMgr->Get_Player();

	CGameObject::UpdateMyRect(m_tRect, 150.f, 100.f);
	RECT rc = {};
	if (IntersectRect(&rc, &pPlayer->Get_RectBodyInfo(), &m_tRect))
	{
		m_bIn = true;
		if (810 <= SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Get_JumpPower())
		{
			if (CPlayer::STATE::MOVE == SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Get_State()
				|| CPlayer::STATE::IDLE == SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Get_State())
				SCAST<CPlayer*>(m_pObjectMgr->Get_Player())->Set_JumpPower(870.f);
		}

	}
	else
		m_bIn = false;

	if (true == m_bIn)
	{
		if(75 < m_iAlpha[NONE])
			m_iAlpha[NONE] += int(m_pTimeMgr->GetDeltaTime() * -450.f);
		if (245 > m_iAlpha[ONIN])
			m_iAlpha[ONIN] += int(m_pTimeMgr->GetDeltaTime() * 450.f);
	}
	else
	{
		if (245 > m_iAlpha[NONE])
			m_iAlpha[NONE] += int(m_pTimeMgr->GetDeltaTime() * 450.f);
		if (25 < m_iAlpha[ONIN])
			m_iAlpha[ONIN] += int(m_pTimeMgr->GetDeltaTime() * -450.f);
	}

	return NO_EVENT;
}

void CBushDown::LateUpdate()
{
	if (false == m_bCulling)
		return;

	SET_MATRIX_WORLD(m_tInfo);					// ���� ��Ʈ���� ����
}

void CBushDown::Render()
{
	if (false == m_bCulling)
		return;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha[NONE], 255, 255, 255));
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfoIn->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha[ONIN], 255, 255, 255));

#ifdef _DEBUG
	m_pDeviceMgr->GetSprite()->End();
	D3DXVECTOR2 vList[] = {
		D3DXVECTOR2(float(m_tRect.left), float(m_tRect.top)),
		D3DXVECTOR2(float(m_tRect.right), float(m_tRect.top)),
		D3DXVECTOR2(float(m_tRect.right), float(m_tRect.bottom)),
		D3DXVECTOR2(float(m_tRect.left), float(m_tRect.bottom)),
		D3DXVECTOR2(float(m_tRect.left), float(m_tRect.top))
	};
	m_pDeviceMgr->GetLine()->Draw(vList, 5, D3DCOLOR_ARGB(255, 255, 255, 0));
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
#endif
}

HRESULT CBushDown::Initialize()
{
	// CGameObject ��Ӻ��� (�ʱ�ȭ ���� �ʿ�� �ڵ�)
	m_tInfo.vPos = { 4950.f, WINCY + 1000.f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
	m_wstrObjectKey = L"BushDown";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);

	// ��������
	m_pTexInfoIn = m_pTextureMgr->GetTexInfo(L"BushDownIn");
	NULL_CHECK_RETURN(m_pTexInfoIn, E_FAIL);
	m_bIn = false;
	m_iAlpha[NONE] = 255;
	m_iAlpha[ONIN] = 0;

	return S_OK;
}

void CBushDown::Release()
{
}

CBushDown * CBushDown::Create()
{
	CBushDown* pInstance = new CBushDown;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
