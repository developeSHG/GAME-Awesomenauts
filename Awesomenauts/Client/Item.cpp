#include "stdafx.h"
#include "Item.h"


CItem::CItem()
{
}


CItem::~CItem()
{
}

int CItem::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	if (true == m_bCoolTime)
	{
		m_fTimeCount += CTimeMgr::GetInstance()->GetDeltaTime();


		if (10.f < m_fTimeCount && ITEM_TYPE::SLIVER == m_eItemType)
		{
			m_fTimeCount = 0.f;
			m_bCoolTime = false;
			m_tFrame.fCurrentFrame = 3.f;
			m_bActing = true;
			m_bCreate = true;
		}
		else if (12.f < m_fTimeCount && ITEM_TYPE::MEDICIN == m_eItemType)
		{
			m_fTimeCount = 0.f;
			m_bCoolTime = false;
			m_tFrame.fCurrentFrame = 3.f;
			m_bActing = true;
			m_bCreate = true;
		}
		return NO_EVENT;
	}

	// 컬링 시작
	if (m_tInfo.vPos.x - 1350.f < CScrollMgr::GetScrollPos().x && m_tInfo.vPos.x + 100.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝


	if (true == m_bActing)
	{
		if (false == m_bCreate)
		{
			if(0.f == m_tFrame.fCurrentFrame)
				m_pSoundMgr->MyPlaySound(L"Item.mp3", CSoundMgr::CHANNELID::UI);

			m_tFrame.fCurrentFrame += m_tFrame.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * 0.3f;
			if (m_tFrame.fCurrentFrame >= 4.0f)
				m_bCoolTime = true;
		}
		else
		{
			m_tFrame.fCurrentFrame -= m_tFrame.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * 0.3f;
			if (m_tFrame.fCurrentFrame <= 0.0f)
			{
				m_bActing = false;
				m_bCreate = false;
				m_tFrame.fCurrentFrame = 4.f;
			}
		}
	}
	else if(false == m_bActing)
	{
		m_tFrame.fCurrentFrame += m_tFrame.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * 0.9f;
		if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount)
			m_tFrame.fCurrentFrame = 4.f;
	}

	return NO_EVENT;
}

void CItem::LateUpdate()
{
	CGameObject::UpdateMyRect(m_tRectInfo[BODY].rcRect, 60.f, 20.f);
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CItem::Render()
{
	if (false == m_bCulling || true == m_bCoolTime)
		return;

	if (ITEM_TYPE::MEDICIN == m_eItemType && true == CScrollMgr::GetSpaceScroll())
		return; 

	if (TRUE == m_bIsDead)
		return;

	SET_TEX_INFO_MULTI(m_wstrObjectKey, m_wstrItemStateKey, m_tFrame.fCurrentFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 호수 랜더 시작
	LakeRender();
	// 호수 랜더 끝

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 55, 255, 55))
#endif
}

HRESULT CItem::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	//m_tInfo.vPos = { 500.f, WINCY + 560.f, 0.f };
	m_wstrObjectKey = L"Item";
	if (ITEM_TYPE::SLIVER == m_eItemType)
	{
		m_tInfo.vSize = { 0.6f, 0.6f, 0.f };
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 42.f;
		m_tFrame.fMoveSpeed = 0.5f;
		m_wstrItemStateKey = L"Sliver";
	}
	else
	{
		m_tInfo.vSize = { 1.f, 1.f, 0.f };
		m_tFrame.fCurrentFrame = 0.f;
		m_tFrame.fMaxFrameCount = 18.f;
		m_tFrame.fMoveSpeed = 1.f;
		m_wstrItemStateKey = L"Medicin";
	}

	// 고유변수
	m_bActing = true;
	m_bCreate = true;
	m_fTimeCount = 0.f;
	m_bCoolTime = false;

	return S_OK;
}

void CItem::Release()
{
}

void CItem::LakeRender()
{
	if (!CScrollMgr::GetLakeRender())
		return;

	D3DXVECTOR3	m_vLake = { m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, LAKE_Y - CScrollMgr::GetScrollPos().y, 0.f };
	D3DXVECTOR3 vPos = { m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f };
	D3DXVECTOR3 vDist = vPos - m_vLake;

	D3DXMATRIX matWorld, matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y * -1, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x - vDist.x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y - vDist.y / 0.5, 0.f);
	matWorld = matScale * matTrans;

	float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(75, 255, 255, 255));
}

CItem * CItem::Create(const D3DXVECTOR3& _vPos, ITEM_TYPE _e)
{
	CItem* pInstance = new CItem;

	pInstance->Set_ItemType(_e);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->Set_Pos(_vPos);

	return pInstance;;
}
