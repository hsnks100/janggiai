#pragma once

#include <functional>
#include <queue>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <iostream>
//#include <stl_pair.h>

using namespace std;

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include <windows.h>
//#endif

// �����?�ϳ��� ��Ÿ���� Ŭ����.
// 
typedef array<long long, 10> boardType; // long long �� �Ʒ��� 10��
typedef unsigned int actionType; // �׼� Ÿ��
struct IntVec2
{
	int x, y;
	IntVec2(){}
	IntVec2(int _x, int _y)
	{
		x = _x;
		y = _y; 
	}
	bool operator==(const IntVec2& t) const
	{
		return x == t.x && y == t.y;
	}
};

static int getFromY(const actionType& at) 
{
	return (at >> (4 * 3)) & 0xF; //  
}
static int getFromX(const actionType& at) 
{
	return (at >> (4 * 2)) & 0xF; //  
}
static int getToY(const actionType& at) 
{
	return (at >> (4 * 1)) & 0xF; //  
}
static int getToX(const actionType& at)
{
	return (at >> (4 * 0)) & 0xF; //  
}
static IntVec2 getTo(const actionType& at)
{
	return IntVec2(at & 0xF, (at >> 4) & 0xF);
}
static IntVec2 getFrom(const actionType& at)
{
	return IntVec2((at >> 8) & 0xF, (at >> 12) & 0xF);
}
static actionType makeAction(int fromY, int fromX, int toY, int toX)
{
	return (fromY << (4 * 3)) | (fromX << (4 * 2)) | (toY << (4 * 1)) | (toX << (4 * 0));
}


class JanggiNode
{
public:

	enum GimulType { hanjol = 8, hansang, hansa, hanma, hanpo, hancha, hanking,
		chojol = 1, chosang, chosa, choma, chopo, chocha, choking};
	static const int m_width = 9;
	//mutable map<int, int> m_gimulScores;
	static int m_gimulScores[20];

	static vector<IntVec2> m_goongPath[15][15];
	static bool m_goongPathInit;
	static int m_positionCache[0xFFFFF + 1];
	static int m_scoreCache[0xFFFFF + 1]; // cho �������� ���� ĳ��.
	void init();
	void setGoong(vector<IntVec2>(* path)[15], int shiftY, int shiftX);

	JanggiNode(const JanggiNode& nono)
	{
		init();
		m_board = nono.m_board;
		m_justPlayer = nono.m_justPlayer;
		m_calculatedEval = nono.m_calculatedEval;
		m_lastMove = nono.m_lastMove;
	}
	const JanggiNode& operator=(const JanggiNode& nono)
	{
		init();
		m_board = nono.m_board;
		m_justPlayer = nono.m_justPlayer;
		m_calculatedEval = nono.m_calculatedEval;
		m_lastMove = nono.m_lastMove;
		return *this;
	}
	JanggiNode()
	{
		init();
			  


		//setGimul(0, 0, hancha);
		//setGimul(0, 1, hansang);
		//setGimul(0, 2, hanma);
		//setGimul(0, 3, hansa);
		//setGimul(0, 5, hansa);
		//setGimul(0, 6, hansang);
		//setGimul(0, 7, hanma);
		//setGimul(0, 8, hancha);
		//setGimul(1, 4, hanking);


		//setGimul(2, 1, hanpo);
		//setGimul(2, 7, hanpo);
		//setGimul(3, 0, hanjol);
		//setGimul(5, 2, hanjol);
		//setGimul(3, 6, hanjol);
		//setGimul(3, 8, hanjol);

		//setGimul(9, 0, chocha);
		//setGimul(9, 1, chosang);
		//setGimul(9, 2, choma);
		//setGimul(9, 3, chosa);
		//setGimul(9, 5, chosa);
		//setGimul(9, 6, chosang);
		//setGimul(9, 7, choma);
		//setGimul(9, 8, chocha);
		//setGimul(8, 4, choking);

		//setGimul(6, 1, chocha);
		//setGimul(7, 1, chopo);
		//setGimul(7, 7, chopo);
		//setGimul(6, 0, chojol);
		//setGimul(6, 2, chojol);
		//setGimul(6, 4, chojol);
		//setGimul(6, 6, chojol);
		//setGimul(6, 8, chojol);
		
		//printf("%s", toString().c_str()); 

	}
	int m_justPlayer;
	virtual void DoMove(actionType move);
	virtual void GetPossibleMoves(vector<actionType>& getter) const; 
	virtual int GetGameResult(int playerjm, int currentDepth) const;
	virtual int GetGameResult2(int playerjm, int currentDepth) const;
	virtual int GetGameResult0(int playerjm, int currentDepth) const; // �� ����
	//typedef array<long long, 10> boardType; // long long �� �Ʒ��� 10��
	//typedef array<array<byte, 5>, 10> boardType2; // long long �� �Ʒ��� 10��
	boardType m_board; 
	//boardType2 m_board2; 
	mutable int m_calculatedEval;
	actionType m_lastMove;

	JanggiNode(const boardType& temp, int justPlayer, actionType action = 0)
	{
		init();
		m_board = temp;
		m_justPlayer = justPlayer;
		if(action != 0)
			DoMove(action);
	}
	//utility function
	inline int getGimul(int y, int x) const
	{

		int v = (m_board[y] >> ((8 - x)*4)) & 0xF; // & (0xF000000000LL >> (4 * x)));
		return v;
	}
	inline int getGimul(const IntVec2& iv) const
	{ 
		int v = (m_board[iv.y] >> ((8 - iv.x)*4)) & 0xF; // & (0xF000000000LL >> (4 * x)));
		return v;
	}

	//int getGimul2(int y, int x)
	//{
	//	int v = m_board2[y][x/2]&(0xF0 >> (4 * (x%2)));
	//	return v;
	//}
	void setRemoveGimul(int y, int x)
	{
		//auto mask = rotr64(0x0000000f00000000, 4 * x);
		auto mask = 0x0000000f00000000 >> ( 4 * x );
		//mask = ~mask;
		m_board[y] &= ~mask;
	}
	void setGimul(int y, int x, int g)
	{
		g &= 0xF;
		long long gg = g;
		auto mask = 0x0000000f00000000 >> ( 4 * x );
		m_board[y] &= ~mask;

		m_board[y] |= (gg << ((8LL - x) * 4LL)); 
	}
	inline long long rotl64 (long long n, unsigned int c) const
	{
		const unsigned int mask = (CHAR_BIT*sizeof(n)-1); 
		c &= mask;  
		return (n<<c) | (n>>( (-c)&mask ));
	}

	inline long long rotr64 (long long n, unsigned int c) const
	{
		const unsigned int mask = (CHAR_BIT*sizeof(n)-1); 
		c &= mask;
		return (n>>c) | (n<<( (-c)&mask ));
	}
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//	void showState() const
//	{
//		auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//		for(int y=0; y<m_board.size(); y++)
//		{
//			for(int x=0; x<=8; x++)
//			{
//				int v = getGimul(y, x);
//				if(v == 0)
//				{
//					SetConsoleTextAttribute(hConsole, 20);
//				}
//				else if(chojol <= v && v <= choking)
//				{
//					if(m_lastMove & 0x00FF == makeAction(0, 0, y, x)) // .second == IntVec2(x, y))
//						SetConsoleTextAttribute(hConsole, 12);
//					else
//						SetConsoleTextAttribute(hConsole, 10);
//				}
//				else
//				{
//					if(m_lastMove & 0x00FF == makeAction(0, 0, y, x)) // .second == IntVec2(x, y))
//					//if(m_lastMove.second == IntVec2(x, y))
//						SetConsoleTextAttribute(hConsole, 12);
//					else
//						SetConsoleTextAttribute(hConsole, 15);
//					//SetConsoleTextAttribute(hConsole, 15);
//
//				}
//				if(v == 0)
//				{
//					cout << "_" << " ";
//				}
//				else if(v < 10){
//					cout << v << " ";
//				}
//				else
//				{
//					cout << (char)('A' - 10 + v) << " ";
//					//printf("%c ", 'A' - 10 + v);
//				}
//			}
//			cout << endl;
//		}
//		SetConsoleTextAttribute(hConsole, 15);
//		printf("-----------------\n");
//	}
//#endif

	string toString(bool pretty)
	{
		//char buf[1000];
		//string buf;
		ostringstream oss;
		for(int y=0; y<m_board.size(); y++)
		{
			for(int x=0; x<=8; x++)
			{
				int v = getGimul(y, x);
				if (v < 10)
					oss << v;
				else
				{
					oss << (char)('A' - 10 + v);
					//printf("%c ", 'A' - 10 + v);
				}
				if (pretty)
					oss << " ";
			}
			if(pretty)
				oss << endl;
			//printf("\n");
		}
		return oss.str();
	}
	void string2Layout(const string& init)
	{

		for(int i=0; i<m_board.size(); i++)
		{
			m_board[i] = 0;
		}

		//setGimul(0, 0, hancha);
		//setGimul(0, 1, hansang);
		//setGimul(0, 2, hanma);
		//setGimul(0, 3, hansa);
		//setGimul(0, 5, hansa);
	//	setGimul(0, 6, hansang);

	//enum GimulType { hanjol = 8, hansang, hansa, hanma, hanpo, hancha, hanking,
	//	chojol = 1, chosang, chosa, choma, chopo, chocha, choking};
		for (int i = 0; i < init.size(); i++)
		{
			int y = i / 9;
			int x = i % 9; 
			if (init[i] == '1')
			{
				setGimul(y, x, chojol);
			}
			else if (init[i] == '2')
			{
				setGimul(y, x, chosang);
			}
			else if (init[i] == '3')
			{
				setGimul(y, x, chosa);
			}
			else if (init[i] == '4')
			{
				setGimul(y, x, choma);
			}
			else if (init[i] == '5')
			{
				setGimul(y, x, chopo);
			}
			else if (init[i] == '6')
			{
				setGimul(y, x, chocha);
			}
			else if (init[i] == '7')
			{
				setGimul(y, x, choking);
			}
			else if (init[i] == '8')
			{
				setGimul(y, x, hanjol);
			}
			else if (init[i] == '9')
			{
				setGimul(y, x, hansang);
			}
			else if (init[i] == 'A')
			{
				setGimul(y, x, hansa);
			}
			else if (init[i] == 'B')
			{
				setGimul(y, x, hanma);
			}
			else if (init[i] == 'C')
			{
				setGimul(y, x, hanpo);
			}
			else if (init[i] == 'D')
			{
				setGimul(y, x, hancha);
			}
			else if (init[i] == 'E')
			{
				setGimul(y, x, hanking);
			} 
			else
			{
			}
		}
	}

	bool validCoord(int y, int x)
	{
		return 0 <= y && y < m_board.size() &&
			0 <= x && x < m_width;
	}
	bool sameTeam(int g1, int g2)
	{
		return (1 <= g1 && g1 <= 7 && 1 <= g2 && g2 <= 7 ||
			g2 >= 8 && g1 >= 8); 
	} 
	// (int*)[15] int* a[15]


	public:
		mutable map<int, function<void(int, int, vector<actionType>*, bool)>> m_possibleInserter;
private:
	void chojolInserter(int y, int x, vector<actionType>* ret, bool teamConnection = false);
	void hanjolInserter(int y, int x, vector<actionType>* ret, bool teamConnection = false);
	void maInserter(int y, int x, vector<actionType>* ret, bool teamConnection = false);
	void sangInserter(int y, int x, vector<actionType>* ret, bool teamConnection = false);
	//void hanSaInserter(int y, int x, vector<actionType>* ret);
	//void choSaInserter(int y, int x, vector<actionType>* ret);
	void goongInserter(int y, int x, vector<actionType>* ret, bool teamConnection = false);
	void poInserter(int y, int x, vector<actionType>* ret, bool teamConnection = false);
	void chaInserter(int y, int x, vector<actionType>* ret, bool teamConnection = false);
	int getOuterConnection(int y, int x) const;
};
