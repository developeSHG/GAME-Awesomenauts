#include "stdafx.h"
#include "Unit.h"

#include "Hurt.h"
#include "Hero.h"

CUnit::CUnit()
	: m_bIsColl(true),
	m_eDir(DIR::RIGHT),
	m_eTeamType(TEAM_TYPE::TEAM_TYPE_END),
	m_bHurt(false)
{
	m_eUnitType = UNIT_TYPE::UNIT_TYPE_END;
	m_pTarget = nullptr;
	m_iAlpha = 255;
	m_fHpAlpha = 255;
	m_bAlpha = false;
	m_bAttackRectActive = false;
	ZeroMemory(&m_vLake, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_tJumpInfo, sizeof(JUMP_INFO));
	ZeroMemory(&m_tStatus, sizeof(STATUS_INFO));

	for (int i = 0; i < HURT_RGB::RGB_END; ++i)
		m_iHurtRgb[i] = 255;

	m_bLevelUp = false;

	m_tFrameLevelUp.fCurrentFrame = 0.f;
	m_tFrameLevelUp.fMaxFrameCount = 21.f;
	m_tFrameLevelUp.fMoveSpeed = 1.f;
	m_tStatus.iLevel = 1;
}


CUnit::~CUnit()
{
}

void CUnit::LakeRender()
{
	if (!CScrollMgr::GetLakeRender())
		return;

	m_vLake = { m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, LAKE_Y - CScrollMgr::GetScrollPos().y, 0.f };
	D3DXVECTOR3 vPos = { m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f };
	D3DXVECTOR3 vDist = vPos - m_vLake;

	D3DXMATRIX matWorld, matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y * -1, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x - vDist.x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y - vDist.y / 0.5, 0.f);
	matWorld = matScale * matTrans;

	float fCenterX = float(m_pTexInfo->tImgInfo.Width / 2.f);
	float fCenterY = float(m_pTexInfo->tImgInfo.Height);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	int iAlpha = 0;

	if (30 > m_iAlpha)
		iAlpha = 30 - m_iAlpha;
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - m_tImageSink.fX, fCenterY - m_tImageSink.fY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha/3 - 10 + iAlpha, 255, m_iHurtRgb[HURT_RGB::G], m_iHurtRgb[HURT_RGB::B]));
}

void CUnit::AttackRectActive(float _fCurrentFrame, float _fStart, float _fEnd, bool _bScroll /*= true*/)
{
	if (_fStart <= _fCurrentFrame && _fEnd >= _fCurrentFrame)
	{
		if(true == m_bAttackOnlyTarget)
			m_bAttackRectActive = true;
		UpdateAttackRect(_bScroll);
	}
}

void CUnit::Set_StatusExpUp(float _f)
{
	m_tStatus.fExp += _f;
	if (m_tStatus.fMaxExp <= m_tStatus.fExp)
	{
		float f = m_tStatus.fExp - m_tStatus.fMaxExp;
		m_tStatus.fExp = f;
		m_tStatus.fMaxExp += 50.f;
		m_tStatus.fMaxHp += 20.f;
		m_tStatus.fHp += 10.f;
		m_tStatus.fAttack += 5.f;
		m_bLevelUp = true;

		int iLevel = SCAST<CUnit*>(m_pObjectMgr->Get_Player())->Get_Status().iLevel;
		m_tStatus.iLevel += 1;
		if (SCAST<CUnit*>(m_pObjectMgr->Get_Player())->Get_Status().iLevel > iLevel)
		{
			CSoundMgr::GetInstance()->MyPlaySound(L"upgrade.wav", CSoundMgr::CHANNELID::UI);
			SCAST<CHurt*>(m_pObjectMgr->Get_Obj(OBJECT::HURT).front())->Set_LevelUpActive();

			if (3 == m_tStatus.iLevel)
				SCAST<CHero*>(m_pObjectMgr->Get_Player())->Set_SkillCoolTimeActive(SKILL_TYPE::TWO);
			else if (5 == m_tStatus.iLevel)
				SCAST<CHero*>(m_pObjectMgr->Get_Player())->Set_SkillCoolTimeActive(SKILL_TYPE::THREE);
			else if (7 == m_tStatus.iLevel)
				SCAST<CHero*>(m_pObjectMgr->Get_Player())->Set_SkillCoolTimeActive(SKILL_TYPE::FOUR);
		}
	}
}

void CUnit::LevelUpRender(bool _bScroll/* = true*/, float _fx/* = 0.f*/, float _fy/* = 0.f*/)
{
	if (false == m_bLevelUp)
		return; 

	m_tFrameLevelUp.fCurrentFrame += m_tFrameLevelUp.fMaxFrameCount * m_pTimeMgr->GetDeltaTime() * m_tFrameLevelUp.fMoveSpeed;

	if (m_tFrameLevelUp.fCurrentFrame >= m_tFrameLevelUp.fMaxFrameCount)
	{
		m_tFrameLevelUp.fCurrentFrame = 0.f;
		m_bLevelUp = false;
		return;
	}

	D3DXMATRIX matWorld, matScale, matTrans;

	TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"LevelUp", L"Up", (int)m_tFrameLevelUp.fCurrentFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = float(pTexInfo->tImgInfo.Width) * 0.5f;
	float fCenterY = float(pTexInfo->tImgInfo.Height);

	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
	if (_bScroll)
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + _fx - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y + _fy - CScrollMgr::GetScrollPos().y, 0.f);
	else
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + _fx, m_tInfo.vPos.y + _fy, 0.f);

	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CUnit::UpdateRect(bool _bScroll /*= true*/)
{
	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	if (true == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx) - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx) - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		}
	}
	else if (false == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		}
	}
}

void CUnit::NewUpdateRect(bool _bScroll /*= true;*/)
{
	float fScrollX = CScrollMgr::GetScrollPos().x;
	float fScrollY = CScrollMgr::GetScrollPos().y;

	if (true == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x - (m_tRectInfo[RECT_TYPE::BODY].fcx / 2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx/2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.left = LONG(m_tInfo.vPos.x - (m_tRectInfo[RECT_TYPE::OBSERVE].fcx / 2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::OBSERVE].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::OBSERVE].fcx / 2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x - (m_tRectInfo[RECT_TYPE::BODY].fcx / 2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx / 2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.left = LONG(m_tInfo.vPos.x - (m_tRectInfo[RECT_TYPE::OBSERVE].fcx / 2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::OBSERVE].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::OBSERVE].fcx / 2.f) - fScrollX);
			m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		}
	}
	else if (false == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x - (m_tRectInfo[RECT_TYPE::BODY].fcx / 2.f) );
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx / 2.f));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::BODY].rcRect.left = LONG(m_tInfo.vPos.x - (m_tRectInfo[RECT_TYPE::BODY].fcx / 2.f));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::BODY].fcy));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.right = LONG(m_tInfo.vPos.x + (m_tRectInfo[RECT_TYPE::BODY].fcx / 2.f));
			m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		}
	}
}

void CUnit::UpdateAttackRect(bool _bScroll /*= true;*/)
{
	float fScrollY = CScrollMgr::GetScrollPos().y;

	if (true == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right);
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::ATTACK].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right + (m_tRectInfo[RECT_TYPE::ATTACK].fcx));
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left - (m_tRectInfo[RECT_TYPE::ATTACK].fcx));
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::ATTACK].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left);
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		}
	}
	else if (false == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right);
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::ATTACK].fcy));
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right + (m_tRectInfo[RECT_TYPE::ATTACK].fcx));
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left - (m_tRectInfo[RECT_TYPE::ATTACK].fcx));
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::ATTACK].fcy));
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left);
			m_tRectInfo[RECT_TYPE::ATTACK].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		}
	}
}

void CUnit::UpdateSkillRect(bool _bScroll)
{
	float fScrollY = CScrollMgr::GetScrollPos().y;

	if (true == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right);
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::SKILL].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right + (m_tRectInfo[RECT_TYPE::SKILL].fcx));
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left - (m_tRectInfo[RECT_TYPE::SKILL].fcx));
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::SKILL].fcy) - fScrollY);
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left);
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.bottom = LONG(m_tInfo.vPos.y - fScrollY);
			break;
		}
	}
	else if (false == _bScroll)
	{
		switch (m_eDir)
		{
		case DIR::RIGHT:
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right);
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::SKILL].fcy));
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.right + (m_tRectInfo[RECT_TYPE::SKILL].fcx));
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		case DIR::LEFT:
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.left = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left - (m_tRectInfo[RECT_TYPE::SKILL].fcx));
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.top = LONG(m_tInfo.vPos.y - (m_tRectInfo[RECT_TYPE::SKILL].fcy));
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.right = LONG(m_tRectInfo[RECT_TYPE::BODY].rcRect.left);
			m_tRectInfo[RECT_TYPE::SKILL].rcRect.bottom = LONG(m_tInfo.vPos.y);
			break;
		}
	}
}

void CUnit::HurtActive()
{
	if (false == m_bHurt)
		return;

	m_iHurtRgb[HURT_RGB::G] += int(m_pTimeMgr->GetDeltaTime() * 300.f);
	m_iHurtRgb[HURT_RGB::B] += int(m_pTimeMgr->GetDeltaTime() * 400.f);
	if (m_iHurtRgb[HURT_RGB::G] >= 255)
	{
		m_iHurtRgb[HURT_RGB::G] = 255;
	}
	if (m_iHurtRgb[HURT_RGB::B] >= 255)
	{
		m_iHurtRgb[HURT_RGB::B] = 255;
		m_bHurt = false;
	}
}

void CUnit::TargetDirCheck()
{
	if (m_pTarget)
	{
		if (m_pTarget == m_pObjectMgr->Get_Player())
		{
			if (m_pTarget->Get_Pos().x  < m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x)
				m_eDir = DIR::LEFT;
			else
				m_eDir = DIR::RIGHT;
		}
		else
		{
			if (m_pTarget->Get_Pos().x < m_tInfo.vPos.x)
				m_eDir = DIR::LEFT;
			else
				m_eDir = DIR::RIGHT;
		}

		if (STATE::DEAD == SCAST<CUnit*>(m_pTarget)->Get_CurrentState())
			m_pTarget = nullptr;
	}
}
