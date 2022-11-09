#pragma once

class CMyTerrain;
class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

//public:
//	list<TILE_INFO*>& GetBestPath();
//
//public:
//	void StartAstar(D3DXVECTOR3 vStart, D3DXVECTOR3 vGoal);
//
//private:
//	bool PathFinding(int iStartIndex, int iGoalIndex);
//	void MakePath(int iStartIndex, int iGoalIndex);
//
//private:
//	int GetTileIndex(const D3DXVECTOR3& vPos);
//	bool IsPicking(const D3DXVECTOR3& vPos, size_t index);
//
//private:
//	bool FindIndexInOpenLst(int iIndex);
//	bool FindIndexInCloseLst(int iIndex);
//
//private:
//	list<int>			m_OpenLst;
//	list<int>			m_CloseLst;
//	list<TILE_INFO*>	m_BestPath;
//
//	CMyTerrain*			m_pTerrain;
//	int					m_iOriginStartIndex;
};

