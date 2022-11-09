#pragma once
class CScrollMgr
{
public:
	CScrollMgr();
	~CScrollMgr();

public:
	static const D3DXVECTOR3& GetScrollPos();
	static const bool& GetSCrollLockCheck() { return m_bScrollLock; }
	static bool& GetLakeRender() { return m_bLakeRender; }
	static bool& GetSpaceScroll() { return m_bSpaceScroll; }

public:
	static void SetScrollPos(const D3DXVECTOR3& vMove);
	static void SetScrollTaxiPos();
	static void SetScrollRespawnPos();
	static void ScrollLock();
	static void SetSpaceScroll(bool _b =false) { m_bSpaceScroll = _b; }

private:
	static D3DXVECTOR3 m_vScroll;
	static bool m_bScrollLock;
	static bool m_bLakeRender;
	static bool m_bSpaceScroll;
};

