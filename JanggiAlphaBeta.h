 #pragma once

#include "JanggiNode.h"
#include <vector>
#include <deque>
#include <set>
#include <algorithm>
template <typename JanggiNode>
void sort_indexes(vector<int>& getIndex, const vector<JanggiNode> &v) {

	// initialize original index locations
	getIndex.resize(v.size());
	for (size_t i = 0; i != getIndex.size(); ++i) getIndex[i] = i;

	sort(getIndex.begin(), getIndex.end(),
		[&v](size_t i1, size_t i2) {return v[i1].m_calculatedEval > v[i2].m_calculatedEval;}); 
}

class JanggiAlphaBeta
{
public:
	JanggiAlphaBeta()
	{
		m_cuttingCount = 0;
	}

	
	static int m_maxDepth; // = (6); // = 6;

	// ���� memoization �� ū �������� �� ������ �ȵȴٴ� ���̴�.
	// ab �� ���ϰ��� ������ move �� ��ƺ���.
	// �����ӱ��� memoization �� ����Ǿ������� ���� �� ...
	int alphabetaMax(const JanggiNode& node, int depth, int alpha, int beta);
	int alphabetaMin(const JanggiNode& node, int depth, int alpha, int beta);
	//int newAlphabeta(const JanggiNode& node, int depth, int alpha, int beta, bool maxPlayer);
	//int newAlphabeta(const JanggiNode& node, int depth, int alpha, int beta, bool maxPlayer);
	//actionType m_bestMove;
	//actionType m_randomMove;
	int m_cuttingCount;
	int m_pivotPlayer;
	deque<pair<int, IntVec2>> m_history;
	vector<pair<actionType, int>> m_possibleChoice;


	map<pair<array<long long, 10>, bool>, int> m_memoization;

	long long hashBoard(boardType bt, int player)
	{
		long long hsh = 0;
		for(long long t : bt)
		{
			hsh = (hsh + (324723947LL + t)) ^ 0x15C42F1C49CDF17FLL;
		}
		hsh = (hsh + (324723947LL + (long long)player)) ^ 0x15C42F1C49CDF17FLL;
		return hsh;
		//hash = (hash + (324723947 + a[i])) ^93485734985;
		//return 
		//	(bt[0] << 1LL) ^
		//	(bt[1] << 2LL) ^
		//	(bt[2] << 3LL) ^
		//	(bt[3] << 4LL) ^
		//	(bt[4] << 5LL) ^
		//	(bt[5] << 6LL) ^
		//	(bt[6] << 7LL) ^
		//	(bt[7] << 8LL) ^
		//	(bt[8] << 9LL) ^
		//	(bt[9] << 10LL);
	}


};
