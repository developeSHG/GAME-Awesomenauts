#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwKey(0), m_dwKeyDown(0), m_dwKeyPressed(0)
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::Update()
{
	m_dwKey = 0;

	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	//if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	//	m_dwKey |= KEY_RETURN;
	//if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	//	m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;
	if (GetAsyncKeyState('F') & 0x8000)
		m_dwKey |= KEY_F;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;
	if (GetAsyncKeyState('U') & 0x8000)
		m_dwKey |= KEY_U;
	if (GetAsyncKeyState('I') & 0x8000)
		m_dwKey |= KEY_I;
	if (GetAsyncKeyState('O') & 0x8000)
		m_dwKey |= KEY_O;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P;
	if (GetAsyncKeyState(0x31) & 0x8000)
		m_dwKey |= KEY_1;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 이 전에 누른적 없고 현재 눌렸으면 TRUE
	if (!(m_dwKeyDown & dwKey) && (m_dwKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}

	// 이전에 누른적 있고 현재 누르지 않았으면 FALSE
	if ((m_dwKeyDown & dwKey) && !(m_dwKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// 이전에 누른적 있고 현재 누르지 않았다면 TRUE
	if ((m_dwKeyPressed & dwKey) && !(m_dwKey & dwKey))
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}

	// 이전에 누른적 없고 현재 눌렀다면 FALSE
	if (!(m_dwKeyPressed & dwKey) && (m_dwKey & dwKey))
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}

// ex) Ctrl + v
bool CKeyMgr::KeyCombined(DWORD dwFirst, DWORD dwSecond)
{
	if (KeyDown(dwSecond) && KeyPressing(dwFirst))
		return true;

	return false;
}
