#pragma once
class CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

public:
	enum MODE { MODE_FULL, MODE_WIN };

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	LPDIRECT3DDEVICE9 GetDevice() const;
	LPD3DXSPRITE GetSprite() const;
	LPD3DXFONT GetFont() const;
	LPD3DXLINE GetLine() const;

public:
	HRESULT InitDevice(
		HWND hWnd, const DWORD& dwWinCX, 
		const DWORD& dwWinCY, MODE eMode);
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);
	void Release();

private:
	// ��ġ������ �ʿ��� Com��ü. (IDirect3D9)
	// LPDIRECT3DDEVICE9�� ������ �ô´�.
	LPDIRECT3D9			m_pSDK;

	// �׷��� ��ġ�� ������ Com��ü.
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	// �̹��� ������ Com��ü.
	LPD3DXSPRITE		m_pSprite;

	// ��Ʈ ������ Com��ü. (���̷�Ʈ ��Ʈ)
	LPD3DXFONT			m_pFont;

	// ���� ������ Com��ü.
	LPD3DXLINE			m_pLine;

	// COM (Component Object Model)
};

