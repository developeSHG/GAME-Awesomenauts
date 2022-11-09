#pragma once
#include "Scene.h"
class CLogo :
	public CScene
{
private:
	explicit CLogo();

public:
	virtual ~CLogo();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

private:
	static unsigned __stdcall LoadingTextureByThread(void* pParam);
	HRESULT TextureLoad();

public:
	static CLogo* Create();

private:
	HANDLE m_hLoadingThread;
	CRITICAL_SECTION m_CriticalSection;
	wstring	m_wstrLoadingMessage;
	
private:
	static bool m_bLoadingComplete;
	float m_fAngle;
};

