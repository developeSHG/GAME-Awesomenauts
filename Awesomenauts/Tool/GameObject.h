#pragma once

class CToolView;
class CGameObject
{
public:
	enum RECT_TYPE
	{
		BODY, ATTACK, RECT_TYPE_END
	};
	enum DIR
	{
		LEFT, RIGHT
	};

protected:
	explicit CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
	virtual void MiniRender() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual HRESULT LateInit();
	virtual void Release() PURE;

public:
	// Get
	const D3DXVECTOR3& Get_Pos() const { return m_tInfo.vPos; }
	const RECT_INFO* Get_RectInfo() const { return m_tRectInfo; }
	const bool& Get_Dead() const { return m_bIsDead; }
	const bool& Get_AfterSpeed() const { return m_bAfterSpeed; }

public:
	// Set
	void Set_Pos(const D3DXVECTOR3& _v) { m_tInfo.vPos = _v; }
	void Set_Dead() { m_bIsDead = TRUE; }
	virtual void SetToolView(CToolView* pView){}

public:
	void UpdateRect(DIR _e = DIR::RIGHT);
	//bool AfterSpeed(DIR _e, bool _b = false);

protected:
	CToolView*		m_pToolView;
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

protected:
	INFO m_tInfo;
	wstring m_wstrObjectKey;
	TEX_INFO* m_pTexInfo;

protected:
	RECT_INFO m_tRectInfo[RECT_TYPE_END];
	bool m_bIsInit;
	bool m_bIsDead;
	float m_fSpeed;
	bool m_bAfterSpeed;
};

