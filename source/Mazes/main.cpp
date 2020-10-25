#include "olcConsoleGameEngine.h"
#include<stack>
#include<iostream>
using namespace std;
class Mazes :public olcConsoleGameEngine
{


public:
	Mazes()
	{
		m_sAppName = L"Mazes ";
	}


private:
	int m_Width;
	int m_Height;

	int path_width = 3;
	int block_size = 4;

	int* m_maze;  //建立int数组在存放每个块的属性
	enum
	{
		//用十六进制数来表示方向和访问性，牛
		CELL_PATHN = 0x01,    //向上可访问
		CELL_PATHE = 0x02,	  //向右可访问
		CELL_PATHS = 0x04,	  //向下可访问
		CELL_PATHW = 0x08,    //向左可访问
		CELL_VISITED = 0x10   //是否被访问过
	};
	int m_nVisitedCells;  //记录被访问过的块数

	stack<pair<int, int >> m_stack;  //栈记录访问过的点

protected:
	bool OnUserCreate()override
	{
		m_Width = 40;
		m_Height = 25;
		
		m_maze = new int[m_Width * m_Height];
		memset(m_maze, 0x00, m_Width * m_Height * sizeof(int));

		int x = rand() % m_Width;
		int y = rand() % m_Height;
		m_stack.push(make_pair(x, y)); //初始化第一个点

		m_maze[y*m_Width+x ] = CELL_VISITED;
		m_nVisitedCells = 1;
		return true;
	}
	virtual bool OnUserUpdate(float esptime)override
	{
		//this_thread::sleep_for(1ms);
		auto offset = [&](int x, int y) {
			return (m_stack.top().second + y) * m_Width + (m_stack.top().first + x);
		};
		vector<int> neighbours;

		if (m_nVisitedCells < m_Width * m_Height)
		{
			//上
			if (m_stack.top().second > 0 && (m_maze[offset(0,-1)] & CELL_VISITED) == 0)
			{
				neighbours.push_back(0);
			}
			//下
			if (m_stack.top().second < m_Height-1 && (m_maze[offset(0, +1)] & CELL_VISITED) == 0)
			{
				neighbours.push_back(1);
			}
			//左
			if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
			{
				neighbours.push_back(2);
			}
			//右
			if (m_stack.top().first < m_Width -1&& (m_maze[offset(+1, 0)] & CELL_VISITED) == 0)
			{
				neighbours.push_back(3);
			}

			if (!neighbours.empty())
			{
				int index = neighbours[rand() % neighbours.size()];

				switch (index)
				{
				case 0://上

					m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATHS;
					m_maze[offset(0, 0)] |= CELL_PATHN;
					m_stack.push(make_pair(m_stack.top().first + 0, m_stack.top().second  -1));
					break;
				case 1://下

					m_maze[offset(0, 1)] |= CELL_VISITED | CELL_PATHN;
					m_maze[offset(0, 0)] |= CELL_PATHS;
					m_stack.push(make_pair(m_stack.top().first + 0, m_stack.top().second + 1));
					break;
				case 2: //左

					m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATHE;
					m_maze[offset(0, 0)] |= CELL_PATHW;
					m_stack.push(make_pair(m_stack.top().first  -1, m_stack.top().second + 0 ));
					break;
				case 3: //右

					m_maze[offset(1, 0)] |= CELL_VISITED | CELL_PATHW;
					m_maze[offset(0, 0)] |= CELL_PATHE;
					m_stack.push(make_pair(m_stack.top().first + 1, m_stack.top().second + 0));
					break;
				}
				m_nVisitedCells++;
			}
			else
				m_stack.pop();
		}
		else
		{
			while (1)
			{
				if (!m_stack.empty())
					m_stack.pop();
				else
					break;
			}
			

			m_Width = 40;
			m_Height = 25;

			m_maze = new int[m_Width * m_Height];
			memset(m_maze, 0x00, m_Width * m_Height * sizeof(int));

			int x = rand() % m_Width;
			int y = rand() % m_Height;
			m_stack.push(make_pair(x, y)); //初始化第一个点

			m_maze[y * m_Width + x] = CELL_VISITED;
			m_nVisitedCells = 1;
		}
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		for(int x =0;x<m_Width;x++)
			for (int y = 0; y < m_Height; y++)
			{
				for (int py = 0; py < path_width; py++)
					for (int px = 0; px < path_width  ; px++)
					{
						if (m_maze[y * m_Width + x] & CELL_VISITED)
							Draw(x * block_size + px, y * block_size + py, PIXEL_SOLID, FG_YELLOW);
						else
							Draw(x * block_size + px, y * block_size + py, PIXEL_SOLID, FG_BLUE);
					}
				for (int p = 0; p < path_width; p++)
				{
					if (m_maze[y * m_Width + x] & CELL_PATHE)
					{
						Draw(x * block_size + path_width, y * block_size + p, PIXEL_SOLID, FG_YELLOW);
					}
					if (m_maze[y * m_Width + x] & CELL_PATHS)
					{
						Draw(x * block_size + p, y * block_size + path_width, PIXEL_SOLID, FG_YELLOW);
					}
				}
			}

			for (int py = 0; py < path_width; py++)
				for (int px = 0; px < path_width; px++)
					Draw(m_stack.top().first * (path_width + 1) + px, m_stack.top().second * (path_width + 1) + py, PIXEL_SOLID, FG_GREEN); // Draw Cell
				
		return true;
	}

private:
};

int main()
{
	// Seed random number generator
	srand(clock());
	Mazes demo;
	demo.ConstructConsole(160, 100, 8, 8);
	demo.Start();
	return 0;
}