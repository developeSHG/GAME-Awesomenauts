#pragma once

class CGameObject;

template<typename T>
class CAbstractFactory
{
public:
	static CGameObject* Create()
	{
		CGameObject* pObj = new T;
		pObj->Initialize();
		return pObj;
	}
	static CGameObject* Create(float _x, float _y)
	{
		CGameObject* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_x, _y);
		return pObj;
	}
};
// __ABSCRACTFACTORY
