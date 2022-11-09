#pragma once
#include "GameObject.h"
#include "Animation.h"
class CItem :
	public CGameObject, public CAnimation
{
private:
	explicit CItem();

public:
	virtual ~CItem();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// Get
	const bool& Get_CoolTime() const  { return m_bCoolTime; }
	const bool& Get_Acting() const { return m_bActing; }
	const ITEM_TYPE& Get_ItemType() const { return m_eItemType; }

public:
	// Set
	void Set_ActingTrue() { m_bActing = true; m_bCreate = false; if (m_tFrame.fCurrentFrame >= 4.0f)m_tFrame.fCurrentFrame = 0.f; }
	void Set_ItemType(ITEM_TYPE _e) { m_eItemType = _e; }

public:
	void LakeRender();

public:
	static CItem* Create(const D3DXVECTOR3& _vPos, ITEM_TYPE _e);

private:
	bool m_bActing;
	bool m_bCreate;
	float m_fTimeCount;
	bool m_bCoolTime;
	ITEM_TYPE m_eItemType;
	wstring m_wstrItemStateKey;
};

