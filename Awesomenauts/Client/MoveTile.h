#pragma once
#include "GameObject.h"
class CMoveTile :
	public CGameObject
{
private:
	explicit CMoveTile();

public:
	virtual ~CMoveTile();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	void LakeRender();
	void Set_Culling(const float _fLeft, const float _fRight) { m_fCullingLeft = _fLeft, m_fCullingRight = _fRight; }
	void Set_MoveY(const float _fMoveY) { m_fMoveDownY = _fMoveY; }

public:
	static CMoveTile* Create(const D3DXVECTOR3& _v, const float _fLeft, const float _fRight, const float _fMoveY, const float _fSpeed = 100.f);

private:
	float m_fCullingLeft;
	float m_fCullingRight;
	float m_fMoveUpY;
	float m_fMoveDownY;
	bool m_bCheck;
};

