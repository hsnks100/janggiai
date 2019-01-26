#include "JanggiNode.h"

#include "ks19937.h"
#include "JanggiAlphaBeta.h"
vector<IntVec2> JanggiNode::m_goongPath[15][15];
bool JanggiNode::m_goongPathInit = false;
int JanggiNode::m_gimulScores[20];
int JanggiNode::m_positionCache[0xFFFFF + 1];
int JanggiNode::m_scoreCache[0xFFFFF + 1]; // cho �������� ���� ĳ��.
void JanggiNode::GetPossibleMoves(vector<actionType>& getter) const
{
	struct CoordValue
	{
		IntVec2 coord;
		int val;
		bool operator<(const CoordValue& a) const
		{ 
			return val < a.val;
		} 
		CoordValue(const IntVec2& c, int v)
		{
			coord = c;
			val = v;
		}
	};
	priority_queue<CoordValue> cq;
	// m_board �� ����... ������ ������.  
	//int getGimul(int y, int x) const
	//{

	//	int v = (m_board[y] >> ((8 - x)*4)) & 0xF; // & (0xF000000000LL >> (4 * x)));
	//	return v;
	//}

	vector<actionType>& ret = getter;
	ret.reserve(300);
	vector<actionType> kingvec;
	for(int y=0; y<m_board.size(); y++)
	{
		auto row = m_board[y];
		if(row == 0)
			continue;
		for(int x=0; x<m_width; x++)
		{ 
			int val = (row >> ((8 - x) * 4)) & 0xF;
			if(val)
			{
				// ���� ������ �༮�� ����� ���� ���Խ�Ŵ.
				int nowPlayer = 3 - m_justPlayer;
				if(7 * nowPlayer - 6 <= val && val <= 7 * nowPlayer){
					m_possibleInserter[val](y, x, &ret, false);
					//if(val == hanking || val == choking)
					//{
					//	m_possibleInserter[val](y, x, &kingvec, false);
					//	//ret.push_back(make_pair( 
					//}
					//else
					//{
					//	cq.push(CoordValue(IntVec2(x, y), val));

					//}
					//assert(m_possibleInserter[val]);
					//m_possibleInserter[val](y, x, &ret, false); 
				} 
			}
		}
	} 
	//while(!cq.empty())
	//{
	//	auto q = cq.top();
	//	cq.pop();
	//	m_possibleInserter[q.val](q.coord.y, q.coord.x, &ret, false);
	//}
	//ret.insert(ret.end(), kingvec.begin(), kingvec.end());
	//return ret;
}
void JanggiNode::chojolInserter( int y, int x, vector<actionType>* ret, bool teamConnection /*= false*/ )
{
	int dx[] = {-1, 0, 1};
	int dy[] = {0, -1, 0};
	for(int i=0; i<=2; i++)
	{
		int newY = y + dy[i];
		int newX = x + dx[i];
		if(validCoord(newY, newX) == false)
			continue;
		auto gimul = getGimul(newY, newX);
		if(sameTeam(chojol, gimul) ^ teamConnection) // ���� ���ε� ������ üũ�� �ƴϸ� �о�, �ٸ� ���ε�, ������üũ�� �Ǿ������� �о�
			continue;

		//(*ret).push_back(make_pair(IntVec2(x, y), IntVec2(newX, newY))); 
		(*ret).push_back(makeAction(y, x, newY, newX));
	} 
}
void JanggiNode::hanjolInserter(int y, int x, vector<actionType>* ret, bool teamConnection)
{
	int dx[] = {-1, 0, 1};
	int dy[] = {0, 1, 0};
	for(int i=0; i<=2; i++)
	{
		int newY = y + dy[i];
		int newX = x + dx[i];
		if(validCoord(newY, newX) == false)
			continue;
		auto gimul = getGimul(newY, newX);
		if(sameTeam(getGimul(y, x), gimul) ^ teamConnection)
			continue;

		//(*ret).push_back(make_pair(IntVec2(x, y), IntVec2(newX, newY))); 
		(*ret).push_back(makeAction(y, x, newY, newX));
	} 

}
void JanggiNode::maInserter(int y, int x, vector<actionType>* ret, bool teamConnection)
{
	int dx[4][2] = {-1, 1, 2, 2, 1, -1, -2, -2};
	int dy[4][2] = {2, 2, 1, -1, -2, -2, -1, 1};
	int nodx[] = {0, 1, 0, -1};
	int nody[] = {1, 0, -1, 0};
	for(int i=0; i<=3; i++)
	{
		if(validCoord(y + nody[i], x + nodx[i]) == false)
			continue;
		auto gimul = getGimul(y + nody[i], x + nodx[i]);
		if(gimul)
			continue;
		for(int j=0; j<=1; j++)
		{
			int newY = y + dy[i][j];
			int newX = x + dx[i][j];
			if(validCoord(newY, newX) == false)
				continue;
			gimul = getGimul(newY, newX);
			if(sameTeam(gimul, getGimul(y, x)) ^ teamConnection)
				continue;
			(*ret).push_back(makeAction(y, x, newY, newX));
		}
	} 
}
void JanggiNode::sangInserter(int y, int x, vector<actionType>* ret, bool teamConnection)
{
	// ���� �ۼ�����.
	//int dx[4][2] = {-1, 1, 2, 2, 1, -1, -2, -2};
	//int dy[4][2] = {2, 2, 1, -1, -2, -2, -1, 1};
	int dx[] = {2, 3, 3, 2, -2, -3, -3, -2};
	int dy[] = {-3, -2, 2, 3, 3, 2, -2, -3};
	int nodx[8][2] = { {0, 1}, {1, 2}, {1, 2}, {0, 1}, {0, -1}, {-1, -2}, {-1, -2}, {0, -1}};
	int nody[8][2] = { {-1, -2}, {0, -1}, {0, 1}, {1, 2}, {1, 2}, {0, 1}, {0, -1}, {-1, -2}};
	//int nody[] = {1, 0, -1, 0};
	for(int i=0; i<8; i++)
	{
		int goalY = y + dy[i]; // ���� ������
		int goalX = x + dx[i]; // ���� ������
		if(validCoord(goalY, goalX) == false)
			continue;
		bool myuk = false;
		for(int j=0; j<2; j++)
		{
			if(validCoord(y + nody[i][j], x + nodx[i][j]) == false)
				continue;
			if(getGimul(y + nody[i][j], x + nodx[i][j]) != 0)
			{
				myuk = true;
				break; 
			}
		}
		if(myuk)
			continue;
		auto gimul = getGimul(goalY, goalX);
		if(sameTeam(getGimul(y, x), gimul) ^ teamConnection)
			continue;
		//ret->push_back(make_pair(IntVec2(x, y), IntVec2(goalX, goalY)));
		ret->push_back(makeAction(y, x, goalY, goalX));
	} 
}
void JanggiNode::goongInserter(int y, int x, vector<actionType>* ret, bool teamConnection)
{
	for(IntVec2& t : m_goongPath[y][x])
	{
		if(sameTeam(getGimul(y, x), getGimul(t.y, t.x)) ^ teamConnection)
			continue;
		//ret->push_back(make_pair(IntVec2(x, y), IntVec2(t.x, t.y)));
		ret->push_back(makeAction(y, x, t.y, t.x));
	} 
}
void JanggiNode::poInserter(int y, int x, vector<actionType>* ret, bool teamConnection)
{
	bool jumped = false;
	auto subInserter = [=, &jumped](int newY, int newX) -> bool{
		if(validCoord(newY, newX) == false)
			return false;
		auto gimul = getGimul(newY, newX);
		if(gimul == hanpo || gimul == chopo)
			return false;
		if(jumped == false) {
			if(gimul) {
				jumped = true; 
			}
		} 
		else {
			if(gimul == 0) {
				ret->push_back(makeAction(y, x, newY, newX));
			}
			else {
				if(sameTeam(getGimul(y, x), gimul) ^ teamConnection) 
				{
				}
				else
				{
					ret->push_back(makeAction(y, x, newY, newX));
					//ret->push_back(make_pair(IntVec2(x, y), IntVec2(newX, newY))); 
				}
				return false;
			} 
		} 
		return true; 
	};
	for(int newY = y + 1; newY < m_board.size(); newY++)
	{
		int newX = x;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	jumped = false;
	for(int newY = y - 1; newY >= 0; newY--)
	{
		int newX = x;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	jumped = false;
	for(int newX = x + 1; newX < m_width; newX++)
	{
		int newY = y;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	jumped = false;
	for(int newX = x - 1; newX >= 0; newX--)
	{
		int newY = y;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	
	// �þȿ����� �� �������� ���� ����.
}
void JanggiNode::chaInserter(int y, int x, vector<actionType>* ret, bool teamConnection)
{ 
	auto subInserter = [=](int newY, int newX) -> bool{
		if(validCoord(newY, newX) == false)
			return false;
		auto gimul = getGimul(newY, newX);
		if(gimul == 0) {
			//ret->push_back(make_pair(IntVec2(x, y), IntVec2(newX, newY)));
			ret->push_back(makeAction(y, x, newY, newX));
		}
		else {
			if(sameTeam(getGimul(y, x), gimul) ^ teamConnection)
			{
			}
			else
			{
				//ret->push_back(make_pair(IntVec2(x, y), IntVec2(newX, newY))); 
				ret->push_back(makeAction(y, x, newY, newX));
			}
			return false;
		} 
		return true; 
	};
	for(int newY = y + 1; newY < m_board.size(); newY++)
	{
		int newX = x;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	for(int newY = y - 1; newY >= 0; newY--)
	{
		int newX = x;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	for(int newX = x + 1; newX < m_width; newX++)
	{
		int newY = y;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	for(int newX = x - 1; newX >= 0; newX--)
	{
		int newY = y;
		auto res = subInserter(newY, newX);
		if(res == false)
			break; 
	}
	// �� �ȿ����� �������� �̱��� 


}
void JanggiNode::setGoong(vector<IntVec2>(* path)[15], int shiftY, int shiftX)
{
	/*
	[...]	
	[...]	
	[-->]	

	[...]	
	[-->]	
	[...]	

	[-->]	
	[...]	
	[...]	
	������ �� ����
	*/

	path[2 + shiftY][0 + shiftX].push_back(IntVec2(0 + shiftX, 1 + shiftY));
	path[2 + shiftY][0 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[2 + shiftY][0 + shiftX].push_back(IntVec2(1 + shiftX, 2 + shiftY));

	path[2 + shiftY][1 + shiftX].push_back(IntVec2(0 + shiftX, 2 + shiftY));
	path[2 + shiftY][1 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[2 + shiftY][1 + shiftX].push_back(IntVec2(2 + shiftX, 2 + shiftY));


	path[2 + shiftY][2 + shiftX].push_back(IntVec2(1 + shiftX, 2 + shiftY));
	path[2 + shiftY][2 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[2 + shiftY][2 + shiftX].push_back(IntVec2(2 + shiftX, 1 + shiftY));


	path[1 + shiftY][0 + shiftX].push_back(IntVec2(0 + shiftX, 0 + shiftY));
	path[1 + shiftY][0 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[1 + shiftY][0 + shiftX].push_back(IntVec2(0 + shiftX, 2 + shiftY));


	path[1 + shiftY][1 + shiftX].push_back(IntVec2(0 + shiftX, 0 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(1 + shiftX, 0 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(2 + shiftX, 0 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(0 + shiftX, 1 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(2 + shiftX, 1 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(0 + shiftX, 2 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(1 + shiftX, 2 + shiftY));
	path[1 + shiftY][1 + shiftX].push_back(IntVec2(2 + shiftX, 2 + shiftY));


	path[1 + shiftY][2 + shiftX].push_back(IntVec2(2 + shiftX, 0 + shiftY));
	path[1 + shiftY][2 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[1 + shiftY][2 + shiftX].push_back(IntVec2(2 + shiftX, 2 + shiftY));


	path[0 + shiftY][0 + shiftX].push_back(IntVec2(1 + shiftX, 0 + shiftY));
	path[0 + shiftY][0 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[0 + shiftY][0 + shiftX].push_back(IntVec2(0 + shiftX, 1 + shiftY));


	path[0 + shiftY][1 + shiftX].push_back(IntVec2(0 + shiftX, 0 + shiftY));
	path[0 + shiftY][1 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[0 + shiftY][1 + shiftX].push_back(IntVec2(2 + shiftX, 0 + shiftY));

	path[0 + shiftY][2 + shiftX].push_back(IntVec2(1 + shiftX, 0 + shiftY));
	path[0 + shiftY][2 + shiftX].push_back(IntVec2(1 + shiftX, 1 + shiftY));
	path[0 + shiftY][2 + shiftX].push_back(IntVec2(2 + shiftX, 1 + shiftY)); 
}
 int JanggiNode::GetGameResult2(int playerjm, int currentDepth) const
 {
	if(m_calculatedEval != 0xcccccccc)
		return m_calculatedEval;
	int bonusMaxPlayer = 0;
	int bonusMinPlayer = 0;
	int maxPlayerScore = 0;
	int sumOfMaxPlayerConn = 0;
	int minPlayerScore = 0;
	int sumOfMinPlayerConn = 0;

	int aggreMaxPlayer = 0;
	int aggreMinPlayer = 0;
	int maxCentral = 0;
	int minCentral = 0;
	int choScore = 0;
	for(int y=0; y<m_board.size(); y++) {
		if(m_board[y] == 0)
			continue;

		choScore += m_scoreCache[m_board[y] >> (4 * 5)];
		choScore += m_scoreCache[m_board[y] & 0xFFFFF]; 
	}
	if(playerjm == 2)
	{
		choScore = -choScore;
	}
	m_calculatedEval = choScore;
	return choScore;

 }
 int JanggiNode::GetGameResult0(int playerjm, int currentDepth) const
 {
	 if(m_calculatedEval != 0xcccccccc)
		 return m_calculatedEval;
	 //return ks19937::getIntValue(-10, 10);
	 int bonusMaxPlayer = 0;
	 int bonusMinPlayer = 0;
	 int maxPlayerScore = 0;
	 int sumOfMaxPlayerConn = 0;
	 int minPlayerScore = 0;
	 int sumOfMinPlayerConn = 0;

	 int aggreMaxPlayer = 0;
	 int aggreMinPlayer = 0;
	 int maxCentral = 0;
	 int minCentral = 0;
	 for(int y=0; y<m_board.size(); y++) {
		 if(m_board[y] == 0)
			 continue;
		 for(int xPosition=0; xPosition<m_width; xPosition++)
		 {
			 int val = getGimul(y, xPosition);
			 if(chojol <= val && val <= choking) {
				 maxPlayerScore += m_gimulScores[val];
				 if(val == chosa && xPosition == 4 && y == 8)
				 {
					 maxCentral += 20;
				 }
				 if(val == chojol || val == choma || val == chosang)
				 {
					 //aggreMaxPlayer += (9 - y);
					 maxCentral += (4 - abs(xPosition - 4))*2;
				 }
				 if(val == choking && y == 9)
				 {
					 maxCentral += 100;
				 } 
			 } 
			 else if(hanjol <= val && val <= hanking){
				 minPlayerScore += m_gimulScores[val];
				 if(val == hansa && xPosition == 4 && y == 1)
				 {
					 maxCentral += 20;
				 }
				 if(val == hanjol || val == hanma || val == hansang){
					 minCentral += (4 - abs(xPosition - 4))*2;
				 }
				 if(val == hanking && y == 0)
				 {
					 minCentral += 100;
				 }
			 } 
		 }
	 } 
	 maxPlayerScore *= 1000;
	 minPlayerScore *= 1000;
	 //aggreMaxPlayer -= 19;
	 //aggreMinPlayer -= 19;
	 bonusMaxPlayer = min((int)1e9, (int)(sumOfMaxPlayerConn + aggreMaxPlayer + maxCentral));
	 bonusMinPlayer = min((int)1e9, (int)(sumOfMinPlayerConn + aggreMinPlayer + minCentral ));

	 maxPlayerScore += bonusMaxPlayer;
	 minPlayerScore += bonusMinPlayer;

	 if(playerjm == 2)
		 swap(maxPlayerScore, minPlayerScore);

	 m_calculatedEval = maxPlayerScore - minPlayerScore;
	 return maxPlayerScore - minPlayerScore;

 }
int JanggiNode::GetGameResult(int playerjm, int currentDepth) const
{
	// playerjm ���忡�� ���� �� ����... ��
	// �� ���� ����� ���� ���� ����.............

	if(m_calculatedEval != 0xcccccccc)
		return m_calculatedEval;
	//return ks19937::getIntValue(-10, 10);
	int bonusMaxPlayer = 0;
	int bonusMinPlayer = 0;
	int maxPlayerScore = 0;
	int sumOfMaxPlayerConn = 0;
	int minPlayerScore = 0;
	int sumOfMinPlayerConn = 0;

	int aggreMaxPlayer = 0;
	int aggreMinPlayer = 0;
	int maxCentral = 0;
	int minCentral = 0;
	for(int y=0; y<m_board.size(); y++) {
		if(m_board[y] == 0)
			continue;
		long long mask = 0xFFFFF;
		for(int i=0; i<=1; i++)
		{
			auto shift = (m_board[y] >> (20 * (1-i)));
			int position = m_positionCache[(shift) & mask];
			while(position)
			{
				int xPosition = (position & 0xF) + i * 5 - 2;
				position >>= 4; 
				//printf("%d, ", xPosition);
				int val = getGimul(y, xPosition);
				if(chojol <= val && val <= choking) {
					maxPlayerScore += m_gimulScores[val];
					//if(currentDepth == JanggiAlphaBeta::m_maxDepth)
					//	sumOfMaxPlayerConn += getOuterConnection(y, x); 
					//if(val != choking)
					if(val == chosa && xPosition == 4 && y == 8)
					{
						maxCentral += 20;
					}
					if(val == chojol || val == choma || val == chosang)
					{
						//aggreMaxPlayer += (9 - y);
						maxCentral += (4 - abs(xPosition - 4))*2;
					}
					if(val == choking && y == 9)
					{
						maxCentral += 100;
					}

				} 
				else if(hanjol <= val && val <= hanking){
					minPlayerScore += m_gimulScores[val];
					//if(currentDepth == JanggiAlphaBeta::m_maxDepth)
					//	sumOfMinPlayerConn += getOuterConnection(y, xPosition); 
					if(val == hansa && xPosition == 4 && y == 1)
					{
						maxCentral += 20;
					}
					if(val == hanjol || val == hanma || val == hansang){
						//aggreMinPlayer += y;
						minCentral += (4 - abs(xPosition - 4))*2;
					}
					if(val == hanking && y == 0)
					{
						minCentral += 100;
					}
				} 
			} 
		}
	} 
	maxPlayerScore *= 1000;
	minPlayerScore *= 1000;
	//aggreMaxPlayer -= 19;
	//aggreMinPlayer -= 19;
	bonusMaxPlayer = min((int)1e9, (int)(sumOfMaxPlayerConn + aggreMaxPlayer + maxCentral));
	bonusMinPlayer = min((int)1e9, (int)(sumOfMinPlayerConn + aggreMinPlayer + minCentral ));

	maxPlayerScore += bonusMaxPlayer;
	minPlayerScore += bonusMinPlayer;

	if(playerjm == 2)
		swap(maxPlayerScore, minPlayerScore);
	
	m_calculatedEval = maxPlayerScore - minPlayerScore;
	return maxPlayerScore - minPlayerScore;
}
void JanggiNode::DoMove(actionType move)
{
	auto oriValue = getGimul(getFromY(move), getFromX(move));

	setRemoveGimul(getFromY(move), getFromX(move));
	setGimul(getToY(move), getToX(move), oriValue);
	m_justPlayer = 3 - m_justPlayer;
	m_lastMove = move;
}
int JanggiNode::getOuterConnection(int y, int x) const
{
	vector<actionType> ret;
	int val = getGimul(y, x);
	if(val)
	{
		m_possibleInserter[val](y, x, &ret, true);  // ���� ���� ī����
	} 
	return ret.size();
}

void JanggiNode::init()
{
	if(m_goongPathInit == false)
	{
		setGoong(m_goongPath, 7, 3);
		setGoong(m_goongPath, 0, 3); 
		m_gimulScores[hanjol] = 2;
		m_gimulScores[chojol] = 2;
		m_gimulScores[hansang] = 3;
		m_gimulScores[chosang] = 3;
		m_gimulScores[hansa] = 3;
		m_gimulScores[chosa] = 3;
		m_gimulScores[hanma] = 5;
		m_gimulScores[choma] = 5;
		m_gimulScores[hanpo] = 7;
		m_gimulScores[chopo] = 7;
		m_gimulScores[hancha] = 13;
		m_gimulScores[chocha] = 13;
		m_gimulScores[hanking] = 10000;
		m_gimulScores[choking] = 10000; 
		m_goongPathInit = true;

		// 0x?0??? => 0x1345 , 
		// position cache
		for(int i=1; i<=0xFFFFF; i++)
		{
			int writeCount = 0;
			for(int j=0; j<=4; j++)
			{
				if(i & (0xF << (j * 4)))
				{
					m_positionCache[i] |= (5 - j) << (writeCount * 4);
					writeCount++;
				}
			} 
		}

		// score cache
		m_scoreCache[0] = 0;
		for(int i=1; i<=0xFFFFF; i++)
		{
			int score = 0;
			int j = i;
			while(j)
			{
				int gimul = j & 0xF;
				if(hanjol <= gimul && gimul <= hanking)
				{
					score -= m_gimulScores[gimul];
				}
				else if(chojol <= gimul && gimul <= choking)
				{
					score += m_gimulScores[gimul];
				}
				j >>= 4;
			}
			m_scoreCache[i] = score;
		}
		
		printf("end init\n");
	}

	m_calculatedEval = 0xcccccccc;
	m_possibleInserter[hanjol] = bind(&JanggiNode::hanjolInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4); 
	m_possibleInserter[chojol] = bind(&JanggiNode::chojolInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	m_possibleInserter[hansang] = bind(&JanggiNode::sangInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	m_possibleInserter[chosang] = bind(&JanggiNode::sangInserter, 
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	m_possibleInserter[hansa] = bind(&JanggiNode::goongInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	m_possibleInserter[chosa] = bind(&JanggiNode::goongInserter, 
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);


	m_possibleInserter[hanking] = bind(&JanggiNode::goongInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	m_possibleInserter[choking] = bind(&JanggiNode::goongInserter, 
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	m_possibleInserter[hanma] = bind(&JanggiNode::maInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	m_possibleInserter[choma] = bind(&JanggiNode::maInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	m_possibleInserter[hanpo] = bind(&JanggiNode::poInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	m_possibleInserter[chopo] = bind(&JanggiNode::poInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	m_possibleInserter[hancha] = bind(&JanggiNode::chaInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	m_possibleInserter[chocha] = bind(&JanggiNode::chaInserter,
		this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	m_justPlayer = 2;

}
