// Copyright (C) 2020 tiansongyu
//
// This file is part of SwitchPixelGameEngine.
//
// SwitchPixelGameEngine is free GameEngine: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SwitchPixelGameEngine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SwitchPixelGameEngine.  If not, see <http://www.gnu.org/licenses/>.
//
// SwitchPixelGameEngine is actively maintained and developed!


// Last Updated: 2020/11/5
/*
Usage:
~~~~~~
This class is abstract, so you must inherit from it. Override the OnUserCreate() function
with all the stuff you need for your application (for thready reasons it's best to do
this in this function and not your class constructor). Override the OnUserUpdate(float fElapsedTime)
function with the good stuff, it gives you the elapsed time since the last call so you
can modify your stuff dynamically. Both functions should return true, unless you need
the application to close.


Example:

#include "SwitchPixelGameEngine.h"
#include <string>
class Demo : public SwitchGameEngine
{
public:
    Demo()
    {
    }
    std::string s_str = "hello world !";
    virtual bool OnUserCreate() override
    {
        return true;
    }
    virtual bool OnUserUpdate(float fElapsedTime) override
    {

        ClearAll();

        //Fill(0,0,100,100,FG_RED);
        //Fill(50,50,200, 200,FG_YELLOW);

        DrawTriangle(20,20,20,60,50,50,FG_YELLOW);

        DrawCircle(20,60,10,FG_GREEN);
        DrawString(ScreenWidth()/2, ScreenHeight()/2, s_str);

        return true;
    }
};

int main()
{
    Demo example;
    example.ConstructConsole(1280,720,1,1);
    example.GameThread();
}

*/

#pragma once

#include <switch.h>

//C Stardand lib
#include <math.h>
#include <stdio.h>
#include <string>
#include <chrono>

extern "C"
{
#include <ft2build.h>
#include FT_FREETYPE_H
}

enum COLOUR
{
	FG_BLACK = 0x00000000,
	FG_DARK_BLUE = 0x008B0000,
	FG_DARK_GREEN = 0x00006400,
	FG_DARK_CYAN = 0x008B8B00,
	FG_DARK_RED = 0x0000008B,
	FG_DARK_MAGENTA = 0x008B008B,
	FG_DARK_YELLOW = 0x0000D7FF,
	FG_GREY = 0x00808080, 
	FG_DARK_GREY = 0x00A9A9A9,
	FG_BLUE = 0x00FF0000,
	FG_GREEN = 0x00008000,
	FG_CYAN = 0x00FFFF00,
	FG_RED = 0x000000FF,
	FG_MAGENTA = 0x00FF00FF,
	FG_YELLOW = 0x00FFFF,
	FG_WHITE = 0x00FFFFFF,
};


//默认switch的屏幕是 1280x720
//TODO:添加声音播放
//TODO:添加触摸功能
//TODO:添加按键控制
//TODO:添加多线程
//TODO:整理函数位置结构
//TODO:RGBA分开存储
//TODO:更换字体颜色和大小

#define FB_WIDTH 1280
#define FB_HEIGHT 720

class SwitchGameEngine
{
public:
	SwitchGameEngine()
	{

		m_nScreenWidth = FB_WIDTH;
		m_nScreenHeight = FB_HEIGHT;
		block_size_x = 1;
		block_size_y = 1;
	}

public:

	virtual void Draw(int x, int y, int rgba)
	{
		if (x >= 0 && x < m_nScreenWidth && y >= 0 && y < m_nScreenHeight)
		{
			for(int dx = 0; dx < block_size_x ; dx++)
				for(int dy =0; dy < block_size_y ; dy++)
					framebuf[(y* block_size_y + dy) * m_nScreenWidth + (x * block_size_x + dx)] = rgba;
		}

	}

	void Fill(int x1, int y1, int x2, int y2, int rgba)
	{
		Clip(x1, y1);
		Clip(x2, y2);
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
				Draw(x, y, rgba);
	}

	void DrawCircle(int xc, int yc, int r, int rgba = FG_RED)
	{
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r)
			return;

		while (y >= x) // only formulate 1/8 of circle
		{
			Draw(xc - x, yc - y, rgba); //upper left left
			Draw(xc - y, yc - x, rgba); //upper upper left
			Draw(xc + y, yc - x, rgba); //upper upper right
			Draw(xc + x, yc - y, rgba); //upper right right
			Draw(xc - x, yc + y, rgba); //lower left left
			Draw(xc - y, yc + x, rgba); //lower lower left
			Draw(xc + y, yc + x, rgba); //lower lower right
			Draw(xc + x, yc + y, rgba); //lower right right
			if (p < 0)
				p += 4 * x++ + 6;
			else
				p += 4 * (x++ - y--) + 10;
		}
	}

	void FillCircle(int xc, int yc, int r, int rgba = FG_RED)
	{
		// Taken from wikipedia
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r)
			return;

		auto drawline = [&](int sx, int ex, int ny) {
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, rgba);
		};

		while (y >= x)
		{
			// Modified to draw scan-lines instead of edges
			drawline(xc - x, xc + x, yc - y);
			drawline(xc - y, xc + y, yc - x);
			drawline(xc - x, xc + x, yc + y);
			drawline(xc - y, xc + y, yc + x);
			if (p < 0)
				p += 4 * x++ + 6;
			else
				p += 4 * (x++ - y--) + 10;
		}
	};
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int rgba = FG_RED)
	{
		DrawLine(x1, y1, x2, y2, rgba);
		DrawLine(x2, y2, x3, y3, rgba);
		DrawLine(x3, y3, x1, y1, rgba);
	}
	//Thanks to https://www.avrfreaks.net/sites/default/files/triangles.c
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int rgba = FG_RED)
	{
		auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny,rgba); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
		if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
		if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										// Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i < dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:
			if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y > y3) return;
		}
	}
	void DrawLine(int x1, int y1, int x2, int y2, int rgba)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1;
		dy = y2 - y1;
		dx1 = abs(dx);
		dy1 = abs(dy);
		px = 2 * dy1 - dx1;
		py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1;
				y = y1;
				xe = x2;
			}
			else
			{
				x = x2;
				y = y2;
				xe = x1;
			}

			Draw(x, y, rgba);

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						y = y + 1;
					else
						y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, rgba);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1;
				y = y1;
				ye = y2;
			}
			else
			{
				x = x2;
				y = y2;
				ye = y1;
			}

			Draw(x, y, rgba);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						x = x + 1;
					else
						x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, rgba);
			}
		}
	}
	void Clip(int &x, int &y)
	{
		if (x < 0) x = 0;
		if (x >= m_nScreenWidth) x = m_nScreenWidth;
		if (y < 0) y = 0;
		if (y >= m_nScreenHeight) y = m_nScreenHeight;
	}



public:
	//Note that this doesn't handle any blending.
	void draw_glyph(FT_Bitmap *bitmap, u32 *framebuf, u32 x, u32 y)
	{
		u32 framex, framey;
		u32 tmpx, tmpy;
		u8 *imageptr = bitmap->buffer;

		if (bitmap->pixel_mode != FT_PIXEL_MODE_GRAY)
			return;

		for (tmpy = 0; tmpy < bitmap->rows; tmpy++)
		{
			for (tmpx = 0; tmpx < bitmap->width; tmpx++)
			{
				framex = x + tmpx;
				framey = y + tmpy;

				framebuf[framey * framebuf_width + framex] = RGBA8_MAXALPHA(imageptr[tmpx], imageptr[tmpx], imageptr[tmpx]);
			}

			imageptr += bitmap->pitch;
		}
	}

	//Note that this doesn't handle {tmpx > width}, etc.
	//str is UTF-8.
	void draw_text(FT_Face face, u32 *framebuf, u32 x, u32 y, const char *str)
	{
		u32 tmpx = x;
		FT_Error ret = 0;
		FT_UInt glyph_index;
		FT_GlyphSlot slot = face->glyph;

		u32 i;
		u32 str_size = strlen(str);
		uint32_t tmpchar;
		ssize_t unitcount = 0;

		for (i = 0; i < str_size;)
		{
			unitcount = decode_utf8(&tmpchar, (const uint8_t *)&str[i]);
			if (unitcount <= 0)
				break;
			i += unitcount;

			if (tmpchar == '\n')
			{
				tmpx = x;
				y += face->size->metrics.height / 64;
				continue;
			}

			glyph_index = FT_Get_Char_Index(face, tmpchar);
			//If using multiple fonts, you could check for glyph_index==0 here and attempt using the FT_Face for the other fonts with FT_Get_Char_Index.

			ret = FT_Load_Glyph(
				face,		 /* handle to face object */
				glyph_index, /* glyph index           */
				FT_LOAD_DEFAULT);

			if (ret == 0)
			{
				ret = FT_Render_Glyph(face->glyph,			  /* glyph slot  */
									  FT_RENDER_MODE_NORMAL); /* render mode */
			}

			if (ret)
				return;

			draw_glyph(&slot->bitmap, framebuf, tmpx + slot->bitmap_left, y - slot->bitmap_top);

			tmpx += slot->advance.x >> 6;
			y += slot->advance.y >> 6;
		}
	}

	static u64 getSystemLanguage(void)
	{
		Result rc;
		u64 code = 0;

		rc = setInitialize();
		if (R_SUCCEEDED(rc))
		{
			rc = setGetSystemLanguage(&code);
			setExit();
		}

		return R_SUCCEEDED(rc) ? code : 0;
	}

	void FontInit()
	{
		//Use this when using multiple shared-fonts.
		/*
		PlFontData fonts[PlSharedFontType_Total];
		size_t total_fonts=0;
		rc = plGetSharedFont(LanguageCode, fonts, PlSharedFontType_Total, &total_fonts);
		if (R_FAILED(rc))
			return error_screen("plGetSharedFont() failed: 0x%x\n", rc);
		*/
		// Use this when you want to use specific shared-font(s). Since this example only uses 1 font, only the font loaded by this will be used.

		plInitialize(PlServiceType_User);

		rc = plGetSharedFontByType(&font, PlSharedFontType_Standard);
		ret = FT_Init_FreeType(&library);
		ret = FT_New_Memory_Face(library,
								 (const FT_Byte *)font.address, /* first byte in memory */
								 font.size,						/* size in bytes        */
								 0,								/* face_index           */
								 &face);
		ret = FT_Set_Char_Size(
			face,	 /* handle to face object           */
			0,		 /* char_width in 1/64th of points  */
			24 * 64, /* char_height in 1/64th of points */
			96,		 /* horizontal device resolution    */
			96);	 /* vertical device resolution      */
	}
	void DrawString(int x, int y, std::string tmp_str)
	{
		const char *str = tmp_str.c_str();
		draw_text(face, framebuf, x, y, str);
	}
	void ClearAll()
	{
		for (u32 y = 0; y < m_nScreenHeight; y++)
		{
			for (u32 x = 0; x < m_nScreenWidth; x++)
			{
				u32 pos = y * stride / sizeof(u32) + x;
				framebuf[pos] = 0x00000000; //Set framebuf to different shades of grey.
			}
		}
	}
	int ConstructConsole(int width,int height,int fontw,int fonth)
	{
		m_nScreenWidth = width;
		m_nScreenHeight = height;
		block_size_x = fontw;
		block_size_y = fonth;
		//init windows
		win = nwindowGetDefault();
		framebufferCreate(&fb, win, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
		framebufferMakeLinear(&fb);
		//字体初始化
		FontInit();
		return 1;
	}
	void GameThread()
	{
		OnUserCreate();

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		while (appletMainLoop())
		{
			// Handle Timing
			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			float fElapsedTime = elapsedTime.count();

			// Scan all the inputs. This should be done once for each frame
			// 手柄输入检测函数

			hidScanInput();

			// hidKeysDown returns information about which buttons have been
			// just pressed in this frame compared to the previous one
			//按键判断函数
			u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS)
				break;
			//摁下+键退出
			// break in order to return to hbmenu

			// Retrieve the framebuffer
			//建立屏幕缓冲区
			framebuf = (u32 *)framebufferBegin(&fb, &stride);
			framebuf_width = stride / sizeof(u32);
			//用户接口

			OnUserUpdate(fElapsedTime);
			char s[10];
			sprintf(s,"%3.2f",1.0f / fElapsedTime);
			DrawString(500,30,std::string("FPS: ")+std::string(s));
			// Each pixel is 4-bytes due to RGBA8888.
			//开始渲染
			// We're done rendering, so we end the frame here.
			framebufferEnd(&fb);
		}

	}
	int ScreenWidth()
	{
		return m_nScreenWidth;
	}
	int ScreenHeight()
	{
		return m_nScreenHeight;
	}


public:
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	virtual bool OnUserDestroy() { return true; }
	~SwitchGameEngine()
	{
		framebufferClose(&fb);
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		delete[] framebuf;
	}
protected:
	int m_nScreenWidth;
	int m_nScreenHeight;
	int block_size_x;
	int block_size_y;


	Framebuffer fb;
	NWindow *win;
	u32 stride;
	u32 *framebuf;
	//font
	Result rc = 0;
	FT_Error ret = 0;
	PlFontData font;
	FT_Library library;
	FT_Face face;



	static u64 LanguageCode;
	u32 framebuf_width = 0;
};