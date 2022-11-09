#pragma once
#include "Unit.h"
class CElement :
	public CUnit
{
private:
	explicit CElement();

public:
	virtual ~CElement();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// Set
	void Set_ElementInfo(const ELEMENT_INFO& _tElement) { m_tElement = _tElement; }
	void Set_EffectActive() { 
		if (false == m_bEffectActive) 
		{ 
			m_tFrame.fCurrentFrame = 0.f;  m_tFrame.fMaxFrameCount = m_tElement.fEffectMaxFrame; m_tFrame.fMoveSpeed = m_tElement.fEffectSpeed; 
			m_tImageSink = m_tElement.tEffectImageSink;
		}  
		m_bEffectActive = true;
	}

public:
	static CElement* Create(const D3DXVECTOR3& _vPos, const ELEMENT_INFO& _tElement);

private:
	ELEMENT_INFO m_tElement;
	float m_fTimeCount;
	bool m_bEffectActive;
};

