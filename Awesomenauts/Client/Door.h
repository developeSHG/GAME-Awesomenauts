#pragma once
#include "GameObject.h"
#include "Animation.h"
class CDoor :
	public CGameObject, public CAnimation
{
public:
	enum TYPE { RED1, RED2, BLUE1, BLUE2, TYPE_END };

private:
	explicit CDoor();

public:
	virtual ~CDoor();

	// CGameObject��(��) ���� ��ӵ�
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CDoor* Create();

private:
	INFO m_tDoorInfo[TYPE_END];
	FRAME m_tDoorFrame[TYPE_END];
};

