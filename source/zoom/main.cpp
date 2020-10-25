#include"olcConsoleGameEngine.h"
#include<string>
using namespace std;
class OneLoneCoder_PanAndZoom :public olcConsoleGameEngine
{
public:
	OneLoneCoder_PanAndZoom()
	{
		m_sAppName = L"Pan and Zoom";
	}
private:
	float fOffsetX = 0.0f; //偏移
	float fOffsetY = 0.0f;


	float fStartPanX = 0.0f;  //
	float fStartPanY = 0.0f;

	float fscaleX = 1.0f; //缩放倍数
	float fscaleY = 1.0f;
public:

	void WorldToScreen(float fworldX, float fworldY, int& nScreenX, int& nScreenY)
	{
		nScreenX = (int)((fworldX - fOffsetX) * fscaleX);
		nScreenY = (int)((fworldY - fOffsetY) * fscaleY);
		// 图形坐标  转换为屏幕坐标
		//乘以 缩放比率即可实现缩放
	}
	void ScreenToWorld(int nScreenX, int nScreenY, float& fWorldX, float& fWorldY)
	{
		fWorldX = ((float)nScreenX / fscaleX) + fOffsetX;
		fWorldY = ((float)nScreenY / fscaleY) + fOffsetY;
		//屏幕坐标转化为图形坐标
	}
public:
	bool OnUserCreate() override
	{
		fOffsetX = -ScreenWidth() / 2;
		fOffsetY = -ScreenHeight() / 2;
		//设置偏移到中心

		return true;
	}
	bool OnUserUpdate(float fElapsedTime)override
	{

		float fMouseX = (float)GetMouseX(); //获取鼠标位置
		float fMouseY = (float)GetMouseY();

		if (GetMouse(0).bPressed)
		{
			fStartPanX = fMouseX;  //记录拖放初始位置
			fStartPanY = fMouseY;
		}
		if (GetMouse(0).bHeld)
		{
			fOffsetX -= (fMouseX - fStartPanX); //计算出拖放位移大小
			fOffsetY -= (fMouseY - fStartPanY); //

			fStartPanX = fMouseX; //持续拖放，保存上一帧的位置
			fStartPanY = fMouseY;
		}
		float fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom;
		ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom);
		//记录在缩放上一瞬间的鼠标位置
		if (GetKey(L'Q').bHeld)
		{
			fscaleX *= 1.011f;
			fscaleY *= 1.011f;
		}
		if (GetKey(L'A').bHeld)
		{
			fscaleX *= 0.989f;
			fscaleY *= 0.989f;

		}


		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		float After_zoomX;
		float After_zoomY;
		ScreenToWorld(fMouseX, fMouseY, After_zoomX, After_zoomY);
		//记录在缩放之后的鼠标位置
		//将图像回归鼠标的初始位置
		// ======== 在缩放之前记录位置=========缩放之后回到初始鼠标位置
		fOffsetX += (fMouseWorldX_BeforeZoom - After_zoomX);
		fOffsetY += (fMouseWorldY_BeforeZoom - After_zoomY);
		int linenumber = 0;
		for (float y = 0.0f; y <= 10.0f; y++)
		{
			float sx = 0.0f, sy = y;
			float ex = 10.f, ey = y;

			int pixel_sx, pixel_sy, pixel_ex, pixel_ey;

			WorldToScreen(sx, sy, pixel_sx, pixel_sy);
			WorldToScreen(ex, ey, pixel_ex, pixel_ey);
			//只渲染 x和y大于0 并且小于  height和wight的  线条
			if (pixel_ey<0 || pixel_ey>ScreenHeight())
				continue;
			DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, PIXEL_SOLID, FG_WHITE);
			linenumber++;
		}
		for (float x = 0.0f; x <= 10.0f; x++)
		{
			float sy = 0.0f, sx = x;
			float ey = 10.f, ex = x;

			int pixel_sx, pixel_sy, pixel_ex, pixel_ey;

			WorldToScreen(sx, sy, pixel_sx, pixel_sy);
			WorldToScreen(ex, ey, pixel_ex, pixel_ey);
			if (pixel_sx<0 || pixel_ex>ScreenWidth())
				continue;
			DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, PIXEL_SOLID, FG_WHITE);
			linenumber++;
		}
		DrawString(2, 2, L"line number :  " + to_wstring(linenumber));
		return true;
	}
};
int main()
{
	OneLoneCoder_PanAndZoom demo;
	demo.ConstructConsole(160, 100, 4, 4);
	demo.Start();
	return 0;
}