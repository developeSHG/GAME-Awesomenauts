#pragma once
class CGameObject
{
public:
	enum RECT_TYPE
	{
		BODY, OBSERVE, ATTACK, SKILL, RECT_TYPE_END
	};

protected:
	explicit CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual HRESULT LateInit();
	virtual void Release() PURE;

public:
	virtual void UpdateMyRect(RECT& _rcRect, float _fcx, float _fcy);

public:
	// Get
	const D3DXMATRIX& Get_Matrix() const { return m_tInfo.matWorld; }
	const D3DXVECTOR3& Get_Pos() const { return m_tInfo.vPos; }
	const RECT_INFO* Get_RectInfo() const { return m_tRectInfo; }
	const RECT& Get_RectBodyInfo() const { return m_tRectInfo[RECT_TYPE::BODY].rcRect; }
	const RECT& Get_RectObserveInfo() const { return m_tRectInfo[RECT_TYPE::OBSERVE].rcRect; }
	const RECT& Get_RectAttackInfo() const { return m_tRectInfo[RECT_TYPE::ATTACK].rcRect; }
	const RECT& Get_RectSkillInfo() const { return m_tRectInfo[RECT_TYPE::SKILL].rcRect; }
	const bool& Get_Dead() const { return m_bIsDead; }

public:
	// Set
	void Set_Pos(const D3DXVECTOR3& _v) { m_tInfo.vPos = _v; }
	void Set_PosPlus(const D3DXVECTOR3& _v) { m_tInfo.vPos += _v; }
	void Set_Dead() { m_bIsDead = TRUE; }
	void Set_DeadFalse() { m_bIsDead = FALSE; }
	void Set_Speed(const float& _fSpeed) { m_fSpeed = _fSpeed; }

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CObjectMgr*		m_pObjectMgr;
	CTimeMgr*		m_pTimeMgr;
	CKeyMgr*		m_pKeyMgr;
	CSoundMgr*		m_pSoundMgr;

protected:
	INFO m_tInfo;
	wstring m_wstrObjectKey;
	TEX_INFO* m_pTexInfo;

protected:
	RECT_INFO m_tRectInfo[RECT_TYPE_END];
	bool m_bIsInit;
	bool m_bIsDead;
	float m_fSpeed;
	bool m_bCulling;
};

