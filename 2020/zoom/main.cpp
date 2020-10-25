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
	float fOffsetX = 0.0f; //ƫ��
	float fOffsetY = 0.0f;


	float fStartPanX = 0.0f;  //
	float fStartPanY = 0.0f;

	float fscaleX = 1.0f; //���ű���
	float fscaleY = 1.0f;
public:

	void WorldToScreen(float fworldX, float fworldY, int& nScreenX, int& nScreenY)
	{
		nScreenX = (int)((fworldX - fOffsetX) * fscaleX);
		nScreenY = (int)((fworldY - fOffsetY) * fscaleY);
		// ͼ������  ת��Ϊ��Ļ����
		//���� ���ű��ʼ���ʵ������
	}
	void ScreenToWorld(int nScreenX, int nScreenY, float& fWorldX, float& fWorldY)
	{
		fWorldX = ((float)nScreenX / fscaleX) + fOffsetX;
		fWorldY = ((float)nScreenY / fscaleY) + fOffsetY;
		//��Ļ����ת��Ϊͼ������
	}
public:
	bool OnUserCreate() override
	{
		fOffsetX = -ScreenWidth() / 2;
		fOffsetY = -ScreenHeight() / 2;
		//����ƫ�Ƶ�����

		return true;
	}
	bool OnUserUpdate(float fElapsedTime)override
	{

		float fMouseX = (float)GetMouseX(); //��ȡ���λ��
		float fMouseY = (float)GetMouseY();

		if (GetMouse(0).bPressed)
		{
			fStartPanX = fMouseX;  //��¼�Ϸų�ʼλ��
			fStartPanY = fMouseY;
		}
		if (GetMouse(0).bHeld)
		{
			fOffsetX -= (fMouseX - fStartPanX); //������Ϸ�λ�ƴ�С
			fOffsetY -= (fMouseY - fStartPanY); //

			fStartPanX = fMouseX; //�����Ϸţ�������һ֡��λ��
			fStartPanY = fMouseY;
		}
		float fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom;
		ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom);
		//��¼��������һ˲������λ��
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
		//��¼������֮������λ��
		//��ͼ��ع����ĳ�ʼλ��
		// ======== ������֮ǰ��¼λ��=========����֮��ص���ʼ���λ��
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
			//ֻ��Ⱦ x��y����0 ����С��  height��wight��  ����
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