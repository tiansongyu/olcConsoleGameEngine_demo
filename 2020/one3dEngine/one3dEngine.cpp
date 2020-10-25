#define OLC_PGE_APPLICATION
#include "olcConsoleGameEngine.h"
#include<string>
#include<iostream>
#include<Windows.h>
#include<fstream>
#include<strstream>
extern "C"
{
#include"lua54/include/lua.h"
#include"lua54/include/lauxlib.h"
#include"lua54/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib,"lua54/liblua54.a")
#endif // _WIN32
using namespace std;
// Override base class with your custom functionality

class breakout : public olcConsoleGameEngine
{
public:
	breakout()
	{
	}
private:
	string cell;
	int nWight = 16;
	int nHeight = 15;
	int block = 8;
	int ballSpeed = 50;
	float b_x = nWight *block / 3;
	float b_y = nHeight* block / 3;

	int x_dir = 1;
	int y_dir = 1;

	float fbat = 30.0f;
	int fbat_x = 8 * 16 / 2 - 15;
	int fbat_y = block *nHeight;
	int fbat_wight = 30;

	float fBallDX;
	float fBallDY;

	lua_State *script;



public:
	bool OnUserCreate() override
	{
		lua_State *L = luaL_newstate();
		luaL_openlibs(L);

		if (CheckLua(L, luaL_dofile(L, "map.lua")))
		{
			lua_getglobal(L, "map");
			if (lua_isstring(L, -1))
			{
				cell += lua_tostring(L, -1);
			}
		}

		/*cell +="################";
			cell +=	"#..............#";
			cell +=	"#..............#";
			cell +=	"#..111111111...#";
			cell +=	"#..............#";
			cell +=	"#....1111......#";
			cell +=	"#..............#";
			cell +=	"#....11...1....#";
			cell +=	"#..............#";
			cell +=	"#..............#";
			cell +=	"#..............#";
			cell +=	"#..............#";
			cell +=	"#..............#";
			cell +=	"#..............#";
			cell +=	"#..............#";
			cell +=	"#..............#";*/

		//LoadMapfromfile("map.txt");
		float angle = (float(rand())) / (float)RAND_MAX;
		angle = 0.6f;
		fBallDX = cosf(angle);
		fBallDY = sinf(angle);
		return true;
	}
	bool LoadMapfromfile(string name)
	{
		ifstream f(name);
		if (!f.is_open())
			return false;

		while(!f.eof())
		{
			char line[128];
			f.getline(line, 128);
			cell.append(line);
		}
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(VK_LEFT).bHeld)	fbat -= fElapsedTime* 50.0f;
		if (GetKey(VK_RIGHT).bHeld)	fbat += fElapsedTime* 50.0f;

		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		for (int y = 0; y < nHeight; y++)
		{
			for (int x = 0; x < nWight ; x++)
			{
				switch (cell[y*nWight+x])
				{
				case '#':
					Fill(x*block, y*block, (x + 1)*block, (y + 1)*block, PIXEL_SOLID, FG_WHITE);
					break;
				case '.':
					Fill(x*block, y*block, (x + 1)*block, (y + 1)*block, PIXEL_SOLID, FG_BLACK);
					break;
				case '1':
					Fill(x*block, y*block, (x + 1)*block, (y + 1)*block, PIXEL_SOLID, FG_BLUE);
					break;
				default:
					break;
				}
			}
		}

		float old_x = b_x;
		float old_y = b_y;

		b_x += x_dir * fBallDX * fElapsedTime* ballSpeed;
		b_y += y_dir * fBallDY * fElapsedTime* ballSpeed;

		float new_x = b_x;
		float new_y = b_y;
	
		int nCellOldX = (int)(old_x / block);
		int nCellOldY = (int)(old_y / block);

		int nCellNewX = (int)(new_x / block);
		int nCellNewY = (int)(new_y / block);

		char newcell = cell[nCellNewY* nWight + nCellNewX];
		char oldcell = cell[nCellOldY* nWight + nCellOldX];

		if (newcell != '.')
		{
			if (newcell == '1')
				cell[nCellNewY*nWight + nCellNewX] = '.';
			//OutputDebugString(stringToLPCWSTR(to_string(b_x)));
			if (nCellNewX != nCellOldX)
			{
				x_dir *= -1;
			}
			if (nCellNewY != nCellOldY)
			{
				y_dir *= -1;
			}

		}

		FillCircle(b_x,b_y, 2, PIXEL_SOLID, FG_RED);

		if (fbat < 0)fbat = 0;
		if (fbat + fbat_wight > nWight * block)fbat = nWight * block - fbat_wight;
		DrawLine(fbat, fbat_y, fbat + fbat_wight, fbat_y, PIXEL_SOLID, FG_WHITE);

		if (b_y > fbat_y - 2)
		{
			if(b_x > fbat&&b_x<fbat + fbat_wight)
			{
				y_dir *= -1;
			}
			else
			{
				b_x = (nWight / 2.0f)*block;
				b_y = (nHeight / 2.0f)*block;

				float angle = (float(rand())) / (float)RAND_MAX;

				angle = 0.6f;
				fBallDX = cosf(angle);
				fBallDY = sinf(angle);
				//DEAD
			}
		}
		return true;
	}
	bool CheckLua(lua_State * L, int r)
	{
		if (r != LUA_OK)
		{
			std::string errormsg = lua_tostring(L, -1);
			std::cout << errormsg << std::endl;
			return false;
		}
		return true;
	}
};

int main()
{
	breakout game;
	if (game.ConstructConsole(8* 16, 9*15 ,4,4))
		game.Start();
	return 0;
}

