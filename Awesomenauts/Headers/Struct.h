#pragma once

#ifndef __STRUCT_H__

#include "Enum.h"

typedef struct tagPathInfo
{
	wstring wstrObjectKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrRelativePath = L"";
	int		iImgCount = 0;
}PATH_INFO;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;
	D3DXMATRIX  matWorld;
}INFO;

typedef struct tagUnitData
{
#ifdef CLIENT
	wstring strName = L"";
#else
	CString strName = L"";
#endif
	int		iAtt = 0;
	int		iDef = 0;
	int		iJobIndex = 0;
	int		iItem = 0;
}UNIT_DATA;

typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9	pTexture;	// 불러온 텍스처을 제어하는 Com객체.
	D3DXIMAGE_INFO		tImgInfo;	// 불러온 텍스처의 정보
}TEX_INFO;

typedef struct tagTileInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE		byOption;
	int			iMyIndex;
	int			iParentIndex;
}TILE_INFO;

typedef struct tagFrame
{
	tagFrame() {}
	tagFrame(float _f1, float _f2, float _f3)
	{
		fCurrentFrame = _f1;
		fMaxFrameCount = _f2;
		fMoveSpeed = _f3;
	};

	float fCurrentFrame;
	float fMaxFrameCount;
	float fMoveSpeed;
}FRAME;

typedef struct tagImageSink
{
	tagImageSink() {}
	tagImageSink(float _f1, float _f2)
	{
		fX = _f1;
		fY = _f2;
	};

	float fX;
	float fY;
}IMAGE_SINK;

typedef struct tagJump
{
	float fPower;
	float fGravity;
}JUMP_INFO;

typedef struct tagTile
{
	TILE_TYPE eType;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	RECT rcRect;
}TILE;

typedef struct tagRect
{
	RECT rcRect;
	float fcx;
	float fcy;
}RECT_INFO;

enum ITEM_TYPE
{
	SLIVER, MEDICIN
};
typedef struct tagItem
{
	ITEM_TYPE eType;
	D3DXVECTOR3 vPos;
}ITEM_INFO;

typedef struct tagStatus
{
	float fMaxHp;
	float fHp;
	float fAttack;
	float fMaxExp;
	float fExp;
	int iMinionKill;
	int iHeroKill;
	int iLevel;
}STATUS_INFO;

typedef struct tagSkillCoolTime
{
	float fCurrentTime;
	float fMaxTime;
}SKILL_COOLTIME;

typedef struct tagElement
{
	tagElement() {}
	tagElement(wstring _wstrObjectKey, wstring _wstrStateKey, DIR _eDir, D3DXVECTOR3 _vDir,
		D3DXVECTOR3 _vSize, int _fAttack, float _fSpeed, FRAME _tFrame, IMAGE_SINK _tImageSink,
		float _fBodyCX, float _fBodyCY, float _fEffectMaxFrame, float _fEffectSpeed, IMAGE_SINK _tEffectImageSink, float _fAlphaTimeCount)
	{
		wstrObjectKey = _wstrObjectKey;
		wstrStateKey = _wstrStateKey;
		eDir = _eDir;
		vDir = _vDir;
		vSize = _vSize;
		fAttack = _fAttack;
		fSpeed = _fSpeed;
		tFrame = _tFrame;
		tImageSink = _tImageSink;
		fBodyCX = _fBodyCX;
		fBodyCY = _fBodyCY;
		fEffectMaxFrame = _fEffectMaxFrame;
		fEffectSpeed = _fEffectSpeed;
		tEffectImageSink = _tEffectImageSink;
		fAlphaTimeCount = _fAlphaTimeCount;
	}

	wstring wstrObjectKey;
	wstring wstrStateKey;
	DIR	eDir;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSize;
	int fAttack;
	float fSpeed;
	FRAME tFrame;
	IMAGE_SINK tImageSink;
	float fBodyCX;
	float fBodyCY;
	float fEffectMaxFrame;
	float fEffectSpeed;
	IMAGE_SINK tEffectImageSink;
	float fAlphaTimeCount;
}ELEMENT_INFO;


#define __STRUCT_H__
#endif