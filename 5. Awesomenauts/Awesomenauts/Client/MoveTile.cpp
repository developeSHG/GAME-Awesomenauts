#include "stdafx.h"
#include "MoveTile.h"


CMoveTile::CMoveTile()
{
}

CMoveTile::~CMoveTile()
{
}

int CMoveTile::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	RECT rc = {};
	CGameObject::UpdateMyRect(rc, 75.f, 50.f);
	m_tRectInfo[BODY].rcRect = rc;
	//m_tRectInfo[BODY].rcRect.top += 15;
	//m_tRectInfo[BODY].rcRect.bottom += 15;
	
	if (m_fMoveUpY >= m_tInfo.vPos.y && true == m_bCheck)
		m_bCheck = false;
	else if (m_fMoveDownY <= m_tInfo.vPos.y && false == m_bCheck)
		m_bCheck = true;

	if(true == m_bCheck)
		m_tInfo.vPos.y += -m_fSpeed * m_pTimeMgr->GetDeltaTime();
	else
		m_tInfo.vPos.y += m_fSpeed * m_pTimeMgr->GetDeltaTime();

	// 컬링 시작
	if (m_fCullingLeft < CScrollMgr::GetScrollPos().x && m_fCullingRight > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝


	return NO_EVENT;
}

void CMoveTile::LateUpdate()
{
	SET_MATRIX_WORLD(m_tInfo);					// 월드 매트릭스 설정
}

void CMoveTile::Render()
{
	if (false == m_bCulling)
		return;

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY + 12, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 호수 랜더 시작
	LakeRender();
	// 호수 랜더 끝

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 170, 170, 170))
#endif
}

HRESULT CMoveTile::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
	m_wstrObjectKey = L"MoveTile";
	SET_TEX_INFO_SINGLE(m_wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	m_fMoveUpY = m_tInfo.vPos.y;
	m_fMoveDownY += m_tInfo.vPos.y;

	// 고유변수

	return S_OK;
}

void CMoveTile::Release()
{
}

void CMoveTile::LakeRender()
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

CMoveTile * CMoveTile::Create(const D3DXVECTOR3& _v, const float _fLeft, const float _fRight, const float _fMoveY, const float _fSpeed /*= 100.f*/)
{
	CMoveTile* pInstance = new CMoveTile;

	pInstance->Set_Culling(_fLeft, _fRight);
	pInstance->Set_MoveY(_fMoveY);
	pInstance->Set_Pos(_v);
	pInstance->Set_Speed(_fSpeed);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
