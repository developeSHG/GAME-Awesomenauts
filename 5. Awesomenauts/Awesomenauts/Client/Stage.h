#pragma once
#include "Scene.h"
class CStage :
	public CScene
{
public:
	enum MINION_WAVE
	{
		ONE, TWO, THREE, FOUR, WAVE_END
	};

private:
	explicit CStage();

public:
	virtual ~CStage();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	HRESULT MoveTileCreate();
	HRESULT ItemCreate();
	HRESULT SpaceItemCreate();
	HRESULT TowerCreate();
	HRESULT RedMinionCreate();
	HRESULT BlueMinionCreate();
	HRESULT JungleMinionCreate();

public:
	static CStage* Create();

private:
	CTimeMgr* m_pTimeMgr;
	float m_fMinionCreateTimeCount;
	float m_fJungleCreateTimeCount;
	bool m_bGameActive;
	int m_iMinionWave;
	float m_fMinionWaveCreateCount;
	bool m_bUpCreate;
};

