#include "stdafx.h"
#include "AstarMgr.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
	/*: m_pTerrain(nullptr), m_iOriginStartIndex(0)*/
{
}


CAstarMgr::~CAstarMgr()
{
}

//list<TILE_INFO*>& CAstarMgr::GetBestPath()
//{
//	// TODO: 여기에 반환 구문을 삽입합니다.
//	return m_BestPath;
//}
//
//void CAstarMgr::StartAstar(D3DXVECTOR3 vStart, D3DXVECTOR3 vGoal)
//{
//	m_OpenLst.clear();
//	m_CloseLst.clear();
//
//#ifdef _DEBUG
//	for (auto& pTile : m_BestPath)
//		pTile->byDrawID = 2;
//#endif
//
//	m_BestPath.clear();
//
//	m_pTerrain = dynamic_cast<CTerrain*>(
//		CObjectMgr::GetInstance()->GetObject(CObjectMgr::TERRAIN));
//	NULL_CHECK(m_pTerrain);
//
//	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();
//
//	m_iOriginStartIndex = GetTileIndex(vStart);
//	int iGoalIndex = GetTileIndex(vGoal);
//
//	if (-1 == m_iOriginStartIndex || -1 == iGoalIndex)
//		return;
//
//	// 목적지와 출발지가 같다면
//	if (m_iOriginStartIndex == iGoalIndex)
//		return;
//
//	// 목적지가 갈 수 없는 곳이라면 
//	if (0 != vecTile[iGoalIndex]->byOption)
//		return;
//
//	if (true == PathFinding(m_iOriginStartIndex, iGoalIndex))
//		MakePath(m_iOriginStartIndex, iGoalIndex);
//}
//
//bool CAstarMgr::PathFinding(int iStartIndex, int iGoalIndex)
//{
//	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();
//	const vector<list<TILE_INFO*>>& vecGraph = m_pTerrain->GetVecGraph();
//
//	if (!m_OpenLst.empty())
//		m_OpenLst.pop_front(); // Queue
//
//	m_CloseLst.push_back(iStartIndex);
//
//	for (auto& pTile : vecGraph[iStartIndex])
//	{
//		// 인접한 타일이 목적지라면 true (탐색 종료)
//		if (pTile->iMyIndex == iGoalIndex)
//		{
//			pTile->iParentIndex = iStartIndex;
//			return true;
//		}
//
//		// 인접타일이 open과 close에 존재하지 않는다면 open에 넣는다.
//		if (false == FindIndexInOpenLst(pTile->iMyIndex)
//			&& false == FindIndexInCloseLst(pTile->iMyIndex))
//		{
//			pTile->iParentIndex = iStartIndex;
//			m_OpenLst.push_back(pTile->iMyIndex);
//		}
//	}
//
//	// 더이상 탐색할 수 없다면 false (탐색 종료)
//	if (m_OpenLst.empty())
//		return false;
//
//	// 휴리스틱 비용 조사 후 OpenLst 정렬
//	// 휴리스틱 = 최초 출발지와의 거리 + 도착지와의 거리
//	int iOriginStartIndex = m_iOriginStartIndex;
//
//	m_OpenLst.sort(
//		[&iOriginStartIndex, &iGoalIndex, &vecTile](int iPrevIndex, int iNextIndex)
//	{
//		D3DXVECTOR3 vDir1 = vecTile[iOriginStartIndex]->vPos - vecTile[iPrevIndex]->vPos;
//		D3DXVECTOR3 vDir2 = vecTile[iGoalIndex]->vPos - vecTile[iPrevIndex]->vPos;
//		float fHueristicA = D3DXVec3Length(&vDir1) + D3DXVec3Length(&vDir2);
//
//		vDir1 = vecTile[iOriginStartIndex]->vPos - vecTile[iNextIndex]->vPos;
//		vDir2 = vecTile[iGoalIndex]->vPos - vecTile[iNextIndex]->vPos;
//		float fHueristicB = D3DXVec3Length(&vDir1) + D3DXVec3Length(&vDir2);
//
//		return fHueristicA < fHueristicB; // 오름차순
//	}
//	);
//
//	// 재귀
//	return PathFinding(m_OpenLst.front(), iGoalIndex);
//}
//
//void CAstarMgr::MakePath(int iStartIndex, int iGoalIndex)
//{
//	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();
//
//	int iParentIndex = iGoalIndex;
//
//	do
//	{
//#ifdef _DEBUG
//		vecTile[iParentIndex]->byDrawID = 36;
//#endif
//		m_BestPath.push_front(vecTile[iParentIndex]);
//		iParentIndex = m_BestPath.front()->iParentIndex;
//
//	} while (iStartIndex != iParentIndex);
//}
//
//int CAstarMgr::GetTileIndex(const D3DXVECTOR3& vPos)
//{
//	for (size_t i = 0; i < m_pTerrain->GetVecTile().size(); ++i)
//	{
//		if (IsPicking(vPos, i))
//			return (int)i;
//	}
//
//	return -1;
//}
//
//bool CAstarMgr::IsPicking(const D3DXVECTOR3& vPos, size_t index)
//{
//	// 내적을 이용한 픽킹 (데카르트)
//	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();
//
//	// 12시, 3시, 6시, 9시 방향의 꼭지점
//	D3DXVECTOR3 vPoint[4] =
//	{
//		{ vecTile[index]->vPos.x, vecTile[index]->vPos.y + TILECY * 0.5f, 0.f },
//		{ vecTile[index]->vPos.x + TILECX * 0.5f, vecTile[index]->vPos.y, 0.f },
//		{ vecTile[index]->vPos.x, vecTile[index]->vPos.y - TILECY * 0.5f, 0.f },
//		{ vecTile[index]->vPos.x - TILECX * 0.5f, vecTile[index]->vPos.y, 0.f }
//	};
//
//	// 시계방향으로 방향벡터 4개
//	D3DXVECTOR3 vDir[4] =
//	{
//		vPoint[1] - vPoint[0],
//		vPoint[2] - vPoint[1],
//		vPoint[3] - vPoint[2],
//		vPoint[0] - vPoint[3]
//	};
//
//	// 각 방향벡터의 법선벡터를 구한다.
//	// *법선벡터: 직각을 이루는 벡터 (직교벡터, 노말벡터)
//	D3DXVECTOR3 vNormal[4] = {};
//
//	// 외적을 활용한 법선벡터 구하기.
//	for (int i = 0; i < 4; ++i)
//		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);
//
//	// 각 꼭지점과 마우스위치의 방향벡터 4개
//	D3DXVECTOR3 vMouseDir[4] =
//	{
//		vPos - vPoint[0],
//		vPos - vPoint[1],
//		vPos - vPoint[2],
//		vPos - vPoint[3],
//	};
//
//	// vMouseDir과 vNormal을 내적했을 때 모두 둔각(음수)이 나오면 true
//	for (int i = 0; i < 4; ++i)
//	{
//		// 하나라도 예각이 나오면 false
//		if (0 < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
//			return false;
//	}
//
//	return true;
//}
//
//bool CAstarMgr::FindIndexInOpenLst(int iIndex)
//{
//	// <algorithm>에서 제공.
//	auto iter_find = find(m_OpenLst.begin(), m_OpenLst.end(), iIndex);
//
//	if (m_OpenLst.end() == iter_find)
//		return false;
//
//	// 존재한다면 true
//	return true;
//}
//
//bool CAstarMgr::FindIndexInCloseLst(int iIndex)
//{
//	// <algorithm>에서 제공.
//	auto iter_find = find(m_CloseLst.begin(), m_CloseLst.end(), iIndex);
//
//	if (m_CloseLst.end() == iter_find)
//		return false;
//
//	// 존재한다면 true
//	return true;
//}
//
