#include "JanggiAlphaBeta.h"
#include <utility>


int JanggiAlphaBeta::m_maxDepth = 1; // = (6); // = 6;
//bool operator<(const pair<array<long long, 10>, bool>& a, const pair<array<long long, 10>, bool>& b)
//{
//	return a.first < b.first;
//}
int JanggiAlphaBeta::alphabetaMax(const JanggiNode& node, int depth, int alpha, int beta)
{
	if(depth == 0)
	{ 
		return node.GetGameResult2(m_pivotPlayer, depth);
	}
	auto res = node.GetGameResult2(m_pivotPlayer, depth);  
	// ���� ���̰� ��û����  ������ ���� �����ɷ� ����
	if(abs(res) > 5e6) //
	{
		return res;
	} 

	int v;
	v = alpha;
	vector<actionType> moves;
	node.GetPossibleMoves(moves);
	vector<JanggiNode> newNodes;
	newNodes.reserve(moves.size() + 5);
	for(auto& m : moves)
	{
		//JanggiNode newNode(node.m_board, node.m_justPlayer);
		//newNode.DoMove(m); 
		if(depth >= m_maxDepth - 3)
		{
			auto newNode = JanggiNode(node.m_board, node.m_justPlayer, m);
			newNode.GetGameResult(m_pivotPlayer, depth);
			newNodes.push_back(newNode);
		}
		else
		{
			newNodes.push_back(JanggiNode(node.m_board, node.m_justPlayer, m)); 
		}
	}

	vector<int> sortingIndexes;
	if(depth >= m_maxDepth - 3)
	{
		sort_indexes(sortingIndexes, newNodes);
	}
	else
	{
		sortingIndexes.resize(newNodes.size());
		for (size_t i = 0; i != newNodes.size(); ++i) 
			sortingIndexes[i] = i; //[i] = i;
	}
	int stepDepth = (m_maxDepth - depth) / 2;
	stepDepth = 0;
	//stepDepth++;
	float percent = (100.f - 10*(stepDepth)) / 100.f;
	percent = max(0.1f, percent);
	percent = min(1.f, percent);
	int sizeSel = sortingIndexes.size() * percent;
	for(int sIndex=0; sIndex<sizeSel; sIndex++)
	{
		auto tempAB = alphabetaMin(newNodes[sortingIndexes[sIndex]], depth - 1, v, beta);
		if(tempAB > v)
		{
			v = tempAB;
			if(depth == m_maxDepth)
			{
				m_possibleChoice.push_back(make_pair(newNodes[sortingIndexes[sIndex]].m_lastMove, v));
			}
		}
		if(beta <= v)
		{
			m_cuttingCount++;
			break;
		}
	} 
	return v; 
}
int JanggiAlphaBeta::alphabetaMin(const JanggiNode& node, int depth, int alpha, int beta)
{
	if(depth == 0)
	{ 
		return node.GetGameResult2(m_pivotPlayer, depth);
	}
	auto res = node.GetGameResult2(m_pivotPlayer, depth);  
	// ���� ���̰� ��û����  ������ ���� �����ɷ� ����
	if(abs(res) > 5e6) //
	{
		return res;
	} 

	int v = beta;
	vector<actionType> moves;
	node.GetPossibleMoves(moves);
	vector<JanggiNode> newNodes;
	newNodes.reserve(moves.size() + 5);
	for(auto& m : moves)
	{
		if(depth >= m_maxDepth - 3)
		{
			auto newNode = JanggiNode(node.m_board, node.m_justPlayer, m);
			newNode.GetGameResult(m_pivotPlayer, depth);
			newNodes.push_back(newNode);
		}
		else
		{
			newNodes.push_back(JanggiNode(node.m_board, node.m_justPlayer, m)); 
		}
	}
	//if(depth == m_maxDepth)
	vector<int> sortingIndexes;
	if(depth >= m_maxDepth - 3)
	{
		sort_indexes(sortingIndexes, newNodes);
	}
	else
	{ 
		sortingIndexes.resize(newNodes.size());
		for (size_t i = 0; i != newNodes.size(); ++i) 
			sortingIndexes[i] = i; 
	}
	int stepDepth = (m_maxDepth - depth) / 2;
	stepDepth = 0;
	float percent = (100.f - 10*(stepDepth)) / 100.f;
	percent = max(0.1f, percent);
	percent = min(1.f, percent);
	int sizeSel = sortingIndexes.size() * percent;
	for(int sIndex=0; sIndex<sizeSel; sIndex++)
	{
		auto tempAB = alphabetaMax(newNodes[sortingIndexes[sIndex]], depth - 1, alpha, v);
		v = min(v, tempAB);
		if(v <= alpha)
		{
			m_cuttingCount++;
			break;
		}

	}
	return v;
}
