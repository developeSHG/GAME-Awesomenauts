#pragma once

#ifndef __DEFINE_H__

#define WINCX 1280
#define WINCY 720

#define TILECX 130
#define TILECY 68

#define TILEX 20
#define TILEY 30

#define SPACE_X 262.5f
#define SPACE_Y 19653.f
#define SPACE_START_Y  550.f

#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256

#define NO_EVENT 0
#define OBJ_DEAD 1

#define GRAVITY 9.8f

#define LAKE_Y 2040.f

#define RUBY		0x0001
#define SAPPHIRE	0x0002
#define DIAMOND		0x0004

#define SCAST static_cast
#define DCAST dynamic_cast

#define SET_TEX_INFO_SINGLE(wstrObjectKey)	\
m_pTexInfo = m_pTextureMgr->GetTexInfo(wstrObjectKey)

#define SET_TEX_INFO_MULTI(wstrObjectKey, wstrStateKey, fCurrentFrame)	\
m_pTexInfo = m_pTextureMgr->GetTexInfo(wstrObjectKey, wstrStateKey, (int)fCurrentFrame)

#ifdef CLIENT
#define SET_MATRIX_WORLD(INFO)	\
D3DXMATRIX matScale, matTrans;		\
D3DXMatrixScaling(&matScale, INFO.vSize.x, INFO.vSize.y, 0.f);		\
D3DXMatrixTranslation(&matTrans, INFO.vPos.x - CScrollMgr::GetScrollPos().x, INFO.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);		\
INFO.matWorld = matScale * matTrans
#else
#define SET_MATRIX_WORLD(INFO)	\
D3DXMATRIX matScale, matTrans;		\
D3DXMatrixScaling(&matScale, INFO.vSize.x, INFO.vSize.y, 0.f);		\
D3DXMatrixTranslation(&matTrans, INFO.vPos.x - m_pToolView->GetScrollPos(0), INFO.vPos.y - m_pToolView->GetScrollPos(1), 0.f);		\
INFO.matWorld = matScale * matTrans
#endif

#define SET_MATRIX_WORLD_NOSCROLL(INFO)	\
D3DXMATRIX matScale, matTrans;		\
D3DXMatrixScaling(&matScale, INFO.vSize.x, INFO.vSize.y, 0.f);		\
D3DXMatrixTranslation(&matTrans, INFO.vPos.x, INFO.vPos.y, 0.f);		\
INFO.matWorld = matScale * matTrans

#define LINE_RENDER(ARGB) \
m_pDeviceMgr->GetSprite()->End();	\
D3DXVECTOR2 vList[] = {	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::BODY].rcRect.left), float(m_tRectInfo[RECT_TYPE::BODY].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::BODY].rcRect.right), float(m_tRectInfo[RECT_TYPE::BODY].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::BODY].rcRect.right), float(m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::BODY].rcRect.left), float(m_tRectInfo[RECT_TYPE::BODY].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::BODY].rcRect.left), float(m_tRectInfo[RECT_TYPE::BODY].rcRect.top))	\
};	\
m_pDeviceMgr->GetLine()->Draw(vList, 5, ARGB);	\
m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

#define LINE_RENDER_OBSERVE(ARGB) \
m_pDeviceMgr->GetSprite()->End();	\
D3DXVECTOR2 vList1[] = {	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.left), float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.right), float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.right), float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.left), float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.left), float(m_tRectInfo[RECT_TYPE::OBSERVE].rcRect.top))	\
};	\
m_pDeviceMgr->GetLine()->Draw(vList1, 5, ARGB);	\
m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

#define LINE_RENDER_ATTACK(ARGB) \
m_pDeviceMgr->GetSprite()->End();	\
D3DXVECTOR2 vList2[] = {	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.left), float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.right), float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.right), float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.left), float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.left), float(m_tRectInfo[RECT_TYPE::ATTACK].rcRect.top))	\
};	\
m_pDeviceMgr->GetLine()->Draw(vList2, 5, ARGB);	\
m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

#define LINE_RENDER_SKILL(ARGB) \
m_pDeviceMgr->GetSprite()->End();	\
D3DXVECTOR2 vList3[] = {	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.left), float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.right), float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.top)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.right), float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.left), float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.bottom)),	\
	D3DXVECTOR2(float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.left), float(m_tRectInfo[RECT_TYPE::SKILL].rcRect.top))	\
};	\
m_pDeviceMgr->GetLine()->Draw(vList3, 5, ARGB);	\
m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define SAFE_DELETE(ptr)				\
if(ptr)									\
{										\
	delete ptr;							\
	ptr = nullptr;						\
}

#ifdef CLIENT
#define ERR_MSG(msg)	\
MessageBox(nullptr, msg, L"System Error!", MB_OK)
#else
#define ERR_MSG(msg)	\
AfxMessageBox(msg)
#endif

#define NULL_CHECK(ptr) \
if(nullptr == (ptr))	\
	return;

#define NULL_CHECK_MSG(ptr, msg)	\
if(nullptr == (ptr))				\
{									\
	ERR_MSG(msg);					\
	return;							\
}

#define NULL_CHECK_RETURN(ptr, returnVal)	\
if(nullptr == (ptr))						\
	return returnVal;

#define NULL_CHECK_MSG_RETURN(ptr, msg, returnVal)	\
if(nullptr == (ptr))								\
{													\
	ERR_MSG(msg);									\
	return returnVal;								\
}

#define FAILED_CHECK(hr)\
if(FAILED(hr))			\
	return;

#define FAILED_CHECK_MSG(hr, msg)	\
if(FAILED(hr))						\
{									\
	ERR_MSG(msg);					\
	return;							\
}

#define FAILED_CHECK_RETURN(hr, returnVal)	\
if(FAILED(hr))								\
	return returnVal;

#define FAILED_CHECK_MSG_RETURN(hr, msg, returnVal)	\
if(FAILED(hr))										\
{													\
	ERR_MSG(msg);									\
	return returnVal;								\
}

#define NO_COPY(ClassName)				\
private:								\
	ClassName(const ClassName& obj);	\
	ClassName& operator=(const ClassName& obj);

#define DECLARE_SINGLETON(ClassName)	\
		NO_COPY(ClassName)				\
public:									\
	static ClassName* GetInstance();	\
	void DestroyInstance();				\
private:								\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::GetInstance()				\
{												\
	if(nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
void ClassName::DestroyInstance()				\
{												\
	if(m_pInstance)								\
	{											\
		delete m_pInstance;						\
		m_pInstance = nullptr;					\
	}											\
}

#define __DEFINE_H__
#endif