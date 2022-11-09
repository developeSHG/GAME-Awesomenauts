#include "stdafx.h"
#include "Element.h"


CElement::CElement()
{
}

CElement::~CElement()
{
}

int CElement::Update()
{
	if (TRUE == m_bIsDead)
		return OBJ_DEAD;

	if(false == m_bEffectActive)
		m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetDeltaTime() * m_fSpeed;

	m_fTimeCount += m_pTimeMgr->GetDeltaTime();

	if (m_tElement.fAlphaTimeCount <= m_fTimeCount)
		m_bAlpha = true;

	if (true == m_bAlpha && false == m_bEffectActive)
	{
		m_iAlpha += int(m_pTimeMgr->GetDeltaTime() * -400.f);

		if(5 >= m_iAlpha)
			return OBJ_DEAD;
	}
	

	// 컬링 시작
	if (m_tInfo.vPos.x - 1485.f < CScrollMgr::GetScrollPos().x && m_tInfo.vPos.x + 120.f > CScrollMgr::GetScrollPos().x
		&& m_tInfo.vPos.y - 900.f < CScrollMgr::GetScrollPos().y && m_tInfo.vPos.y + 70.f > CScrollMgr::GetScrollPos().y)
		m_bCulling = true;
	else
		m_bCulling = false;

	if (false == m_bCulling)
		return NO_EVENT;
	// 컬링 끝

	return NO_EVENT;
}

void CElement::LateUpdate()
{

	SET_MATRIX_WORLD(m_tInfo);
	CAnimation::FrameMove(m_tFrame.fMoveSpeed);		// 프레임무브
	if (true == m_bEffectActive)
	{
		if (m_tFrame.fCurrentFrame >= m_tFrame.fMaxFrameCount - 1)
			m_bIsDead = true;
	}
	FrameCheck();									// State 체크
	CUnit::NewUpdateRect(true);						// 바디렉트 최신화
}

void CElement::Render()
{
	if (false == m_bCulling)
		return;

	if (true == m_bEffectActive)
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Effect", m_tFrame.fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
		float fCenterY = float(m_pTexInfo->tImgInfo.Height);

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX - m_tImageSink.fX, fCenterY - m_tImageSink.fY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		SET_TEX_INFO_MULTI(m_wstrObjectKey, L"Element", m_tFrame.fCurrentFrame);
		NULL_CHECK(m_pTexInfo);

		float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
		float fCenterY = float(m_pTexInfo->tImgInfo.Height);

		D3DXMATRIX matScale, matTrans, matRotate;		
		float fAngle = D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook);

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		if(m_tInfo.vDir.y > 0.f)
			D3DXMatrixRotationZ(&matRotate, -acosf(fAngle));
		else
			D3DXMatrixRotationZ(&matRotate, acosf(fAngle));
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);
		m_tInfo.matWorld = matScale * matRotate * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX - m_tImageSink.fX, fCenterY - m_tImageSink.fY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}

	// 호수 랜더 시작
	CUnit::LakeRender();
	// 호수 랜더 끝

#ifdef _DEBUG
	LINE_RENDER(D3DCOLOR_ARGB(255, 255, 255, 255))
#endif
}

HRESULT CElement::Initialize()
{
	// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	m_wstrObjectKey = m_tElement.wstrObjectKey;
	m_fSpeed = m_tElement.fSpeed;
	m_tInfo.vDir = m_tElement.vDir;
	m_tInfo.vLook = { -1.f, 0.f, 0.f };
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	m_eDir = m_tElement.eDir;
	if(DIR::LEFT == m_eDir)
		m_tElement.vSize.x *= -1;
	else
		m_tElement.vSize.x *= -1;

	m_tInfo.vSize = m_tElement.vSize;

	// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	m_tFrame.fCurrentFrame = m_tElement.tFrame.fCurrentFrame;
	m_tFrame.fMaxFrameCount = m_tElement.tFrame.fMaxFrameCount;
	m_tFrame.fMoveSpeed = m_tElement.tFrame.fMoveSpeed;
	m_tRectInfo[RECT_TYPE::BODY].fcx = m_tElement.fBodyCX;
	m_tRectInfo[RECT_TYPE::BODY].fcy = m_tElement.fBodyCY;
	m_tImageSink.fX = m_tElement.tImageSink.fX;
	m_tImageSink.fY = m_tElement.tImageSink.fY;
	m_bEffectActive = false;
	m_bAttackRectActive = true;
	m_tStatus.fAttack = m_tElement.fAttack;

	//// CGameObject 상속변수 (초기화 변경 필요시 코딩)
	//m_tInfo.vSize = { -0.8f, 0.8f, 0.f };
	//m_wstrObjectKey = L"Tower";
	//m_fSpeed = 300.f;
	//m_tInfo.vDir = m_vTargetPos - m_tInfo.vPos;
	//D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	//// CAnimation, 고유변수 (초기화 변경 필요시 코딩)
	//m_tFrame.fCurrentFrame = 0.f;
	//m_tFrame.fMaxFrameCount = 2.f;
	//m_tFrame.fMoveSpeed = 3.f;
	//m_tRectInfo[RECT_TYPE::BODY].fcx = 30.f;
	//m_tRectInfo[RECT_TYPE::BODY].fcy = 15.f;
	//m_tImageSink.fX = -10.f;
	//m_tImageSink.fY = 18.f;
	//m_bAlpha = false;

	return S_OK;
}

void CElement::Release()
{
}

CElement * CElement::Create(const D3DXVECTOR3 & _vPos, const ELEMENT_INFO& _tElement)
{
	CElement* pInstance = new CElement;

	pInstance->Set_Pos(_vPos);
	pInstance->Set_ElementInfo(_tElement);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
