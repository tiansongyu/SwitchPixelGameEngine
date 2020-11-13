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

//Version: 0.2
// Last Updated: 2020/11/10
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
class Demo : public SwitchPixelGameEngine
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
#include <png.h>
extern "C"
{
#include <ft2build.h>
#include FT_FREETYPE_H
}
#define FB_WIDTH 1280
#define FB_HEIGHT 720
enum FONTSIZE
{
	SMALL_FONT = 0x38,
	MEDIUM_FONT = 0x60,
	LARGE_FONT = 0x8A,
};
enum COLOUR
{
	FG_BLACK = 0xFF000000,
	FG_DARK_BLUE = 0xFF8B0000,
	FG_DARK_GREEN = 0xFF006400,
	FG_DARK_CYAN = 0xFF8B8B00,
	FG_DARK_RED = 0xFF00008B,
	FG_DARK_MAGENTA = 0xFF8B008B,
	FG_DARK_YELLOW = 0xFF00D7FF,
	FG_GREY = 0xFF808080, 
	FG_DARK_GREY = 0xFFA9A9A9,
	FG_BLUE = 0xFFFF0000,
	FG_GREEN = 0xFF008000,
	FG_CYAN = 0xFFFFFF00,
	FG_RED = 0xFF0000FF,
	FG_MAGENTA = 0xFFFF00FF,
	FG_YELLOW = 0xFF00FFFF,
	FG_WHITE = 0xFFFFFFFF,
};
enum MODE{
	PICTURE,
	PIXEL,
};
struct PNG_DATA
{	
	uint32_t nWight;
	uint32_t nHeignt;
	uint32_t* colour;
};
struct RGBA
{
	union 
	{
		uint32_t rgba;
		uint8_t color[4];
	};
};
PNG_DATA  PNGtoRGBA(const char* file_path)
{
	FILE *fp = fopen(file_path, "rb");
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png) abort();

	png_infop info = png_create_info_struct(png);
	if(!info) abort();

	if(setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	png_read_info(png, info);

	uint32_t _nWidth      = png_get_image_width(png, info);
	uint32_t _nHeight     = png_get_image_height(png, info);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth  = png_get_bit_depth(png, info);

	uint32_t* _m_Colours = new uint32_t[_nWidth * _nHeight];
	memset(_m_Colours,0x0,_nWidth * _nHeight *sizeof(uint32_t));

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if(bit_depth == 16)
		png_set_strip_16(png);

	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if(png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if(color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xff, PNG_FILLER_AFTER);

	if(color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * _nHeight);
	for(uint32_t y = 0; y < _nHeight; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
	}
	png_read_image(png, row_pointers);

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);

	RGBA _rgba;
	for(uint32_t y = 0; y < _nHeight; y++)
	{
		png_bytep row = row_pointers[y];
		for(uint32_t x = 0; x < _nWidth; x++)
		{
			png_bytep px = &(row[x * 4]);

			_rgba.color[0] = px[0];
			_rgba.color[1] = px[1];
			_rgba.color[2] = px[2];
			_rgba.color[3] = px[3];			
			_m_Colours[y * _nWidth + x] = _rgba.rgba;
		}
	} 
	PNG_DATA _png;
	_png.nWight = _nWidth;
	_png.nHeignt = _nHeight;
	_png.colour = _m_Colours;
	for(uint32_t y = 0; y < _nHeight; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);
	return _png;
}
class SgeSprite
{
public:
	SgeSprite()
	{
		nWidth = 0;
		nHeight = 0;
		pos_x = 0;
		pos_y = 0;
		pixel_color = FG_RED;
		mode = MODE::PIXEL ;
	}
	~SgeSprite()
	{
		delete[] m_Colours;
	}
	SgeSprite(uint32_t x,uint32_t y,uint32_t w,uint32_t h,COLOUR color = FG_RED)
	{
		pos_x = x;
		pos_y = y;
		nWidth = w ;
		nHeight = h;
		mode = MODE::PIXEL;
		pixel_color = color;
		m_Colours = new uint32_t[nWidth * nHeight];
		memset(m_Colours,color,w * h);
	}
	SgeSprite(uint32_t x,uint32_t y,const char* file_path)
	{	
		pos_x = x;
		pos_y = y;	
		mode = MODE::PICTURE;
		
		PNG_DATA _png;
		_png = PNGtoRGBA(file_path);
		nWidth = _png.nWight;
		nHeight = _png.nHeignt;
		m_Colours = _png.colour;
	}
	
	void SetPosition(uint32_t x,uint32_t y)
	{
		pos_x = x;
		pos_y = y;
	}
	uint32_t GetPos_x(){return pos_x;}
	uint32_t GetPos_y(){return pos_y;}
	uint32_t GetWight(){return nWidth;}
	uint32_t GetHeight(){return nHeight;}
	uint32_t* GetColour(){return m_Colours;}
	COLOUR  GetPixelColour(){return pixel_color;}
	MODE GetMode(){return mode;}
public:
	uint32_t nWidth,nHeight;
	uint32_t pos_x,pos_y;
private:
	uint32_t* m_Colours ;
	COLOUR pixel_color ;
	MODE mode ;

};

class SwitchPixelGameEngine
{
public:
	SwitchPixelGameEngine()
	{

		m_nScreenWidth = FB_WIDTH;
		m_nScreenHeight = FB_HEIGHT;
		block_size_x = 1;
		block_size_y = 1;
		mouse_pos_x = 0 ;
		mouse_pos_y = 0 ;
	}
public:
	void DrawSprite(SgeSprite* sprite)
	{	
		uint32_t* tmp_color = sprite->GetColour();
		MODE tmp_mode = sprite->GetMode();
		for(uint32_t y = 0; y< sprite->GetHeight(); y++)
			for(uint32_t x = 0 ; x < sprite->GetWight() ; x++)
			{
				Draw(sprite->GetPos_x() + x,sprite->GetPos_y()  + y  ,tmp_mode == MODE::PIXEL ?sprite->GetPixelColour() : tmp_color[y * sprite->GetWight() + x]);
			}	
	}

	virtual void Draw(int x, int y,  u32 rgba)
	{
		if (x >= 0 && x < m_nScreenWidth && y >= 0 && y < m_nScreenHeight)
		{
			if((rgba >> 24) == 0x00)
				return ;
			if((rgba >> 24) != 0xFF)
			{
				rgba = AlphaMix(x * block_size_x + 0,y* block_size_y + 0,rgba);
			}
			uint16_t _x = x* block_size_x;
			uint16_t _y = y* block_size_y;
			for(uint8_t dx = 0; dx < block_size_x ; dx++)
				for(uint8_t dy =0; dy < block_size_y ; dy++)
				{
					framebuf[(_y + dy) * FB_WIDTH + (_x + dx)] = rgba;
				}
		}

	}

	uint32_t AlphaMix(uint32_t x ,uint32_t y, uint32_t rgba)
	{
		RGBA old_rgba,new_rgba;
		old_rgba.rgba = Getrgba(x,y);
		new_rgba.rgba = rgba;
		new_rgba.color[0] = new_rgba.color[3] * 1.0f / 255 * new_rgba.color[0] + (255 - new_rgba.color[3]) * 1.0f / 255 * old_rgba.color[0] ;
		new_rgba.color[1] = new_rgba.color[3] * 1.0f / 255 * new_rgba.color[1] + (255 - new_rgba.color[3]) * 1.0f/ 255 * old_rgba.color[1] ;
		new_rgba.color[2] = new_rgba.color[3] * 1.0f / 255 * new_rgba.color[2] + (255 - new_rgba.color[3]) * 1.0f/ 255 * old_rgba.color[2] ;

		return new_rgba.rgba;
	}

	uint32_t Getrgba(uint32_t x,uint32_t y)
	{
		return framebuf[y * FB_WIDTH + x];
	}
	void Fill(int x1, int y1, int x2, int y2, const u32 rgba)
	{
		Clip(x1, y1);
		Clip(x2, y2);
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
				Draw(x, y, rgba);
	}

	void DrawCircle(int xc, int yc, int r, const u32 rgba = FG_RED)
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

	void FillCircle(int xc, int yc, int r, const u32 rgba = FG_RED)
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
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const u32 rgba = FG_RED)
	{
		DrawLine(x1, y1, x2, y2, rgba);
		DrawLine(x2, y2, x3, y3, rgba);
		DrawLine(x3, y3, x1, y1, rgba);
	}
	//Thanks to https://www.avrfreaks.net/sites/default/files/triangles.c
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const u32 rgba = FG_RED)
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
			if (minx > t1x) minx = t1x; 
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x; 
			if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										// Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) 
			break;

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
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y > y3) return;
		}
	}
	void DrawLine(int x1, int y1, int x2, int y2, const u32 rgba = FG_RED)
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
	void draw_glyph(FT_Bitmap *bitmap, u32 x, u32 y)
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
				if(imageptr[tmpx]==0xff)
					if (framex >= 0 && framex < FB_WIDTH && framey >= 0 && framey < FB_HEIGHT)					
						framebuf[framey * framebuf_width + framex] = fontcolor->rgba;
			}
			imageptr += bitmap->pitch;
		}
	}

	//Note that this doesn't handle {tmpx > width}, etc.
	//str is UTF-8.
	void draw_text(u32 x, u32 y, const char *str)
	{
		ret = FT_Set_Char_Size(
		face,	 /* handle to face object           */
		0,		 /* char_width in 1/64th of points  */
		24 * 64, /* char_height in 1/64th of points */
		0,		 /* horizontal device resolution    */
		fontsize);	 /* vertical device resolution      */
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

			draw_glyph(&slot->bitmap, tmpx + slot->bitmap_left, y - slot->bitmap_top);

			tmpx += slot->advance.x >> 6;
			y += slot->advance.y >> 6;
		}
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

		rc = plGetSharedFontByType(&font, PlSharedFontType_ChineseSimplified);
		if (R_FAILED(rc))
        	fatalThrow(rc);
		ret = FT_Init_FreeType(&library);
		ret = FT_New_Memory_Face(library,
								 (const FT_Byte *)font.address, /* first byte in memory */
								 font.size,						/* size in bytes        */
								 0,								/* face_index           */
								 &face);
		SetFontColor(FG_WHITE);
		SetFontSize(MEDIUM_FONT);
	}
	void DrawString(int x, int y, const char *str)
	{
		draw_text(x, y, str);
	}
	void ClearScreen()
	{
		memset(framebuf,0x0,FB_WIDTH * FB_HEIGHT *sizeof(u32));
	}
	void ERROR(const char* str)
	{
 
		framebufferEnd(&fb);
		while (appletMainLoop())
		{
			hidScanInput();
			ClearScreen();
			if (KeyDown(KEY_PLUS))
				break;
			framebuf = (u32 *)framebufferBegin(&fb, &stride);
			framebuf_width = stride / sizeof(u32);
			DrawString(ScreenWidth()/2,ScreenHeight()/2,str);
			framebufferEnd(&fb);
		}
	}
	int ConstructConsole(int fontw,int fonth)
	{
		m_nScreenWidth = FB_WIDTH * 1.0f / fontw;
		m_nScreenHeight = FB_HEIGHT * 1.0f / fonth;
		block_size_x = fontw;
		block_size_y = fonth;

		fontcolor = new RGBA();
		//init mouse pos
		touch = new touchPosition[5];
		rc = plInitialize(PlServiceType_User);
		if (R_FAILED(rc))
        	fatalThrow(rc); 
		romfsInit();
		//init windows
		framebufferCreate(&fb, nwindowGetDefault(), FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 3);
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
			hidScanInput();
			// hidKeysDown returns information about which buttons have been
			// just pressed in this frame compared to the previous one
			/*****************************************************************/
			kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
			kUp = hidKeysUp(CONTROLLER_P1_AUTO);

			if (KeyDown(KEY_PLUS))
				break;
			/*****************************************************************/
			//touch input
			touch_count = hidTouchCount();
			//不知道当touch_count大于0时代表的值是什么，所以先处理0⑧
			//当抬起时，touch_count为零，此时不能进入for循环，所以写在循环之外
			if(touch_count == 0 && prev_touchcount >0)m_mouse[0].bReleased = true;
			else m_mouse[0].bReleased = false;
			if(touch_count != 0 || prev_touchcount != 0)
			{
				for(u32 i = 0 ;i<touch_count ;i++)
				{
					//update pos
					hidTouchRead(&touch[i],i);
					if(touch[i].px >=0 && touch[i].px < 1280)mouse_pos_x = touch[i].px;
					if(touch[i].py >=0 && touch[i].py < 720)mouse_pos_y = touch[i].py;
					//end
					if(prev_touchcount == 0 && touch_count > 0){m_mouse[i].bPressed = true;m_mouse[i].bHeld = false;m_mouse[i].bReleased = false;}
					else if(prev_touchcount > 0 && touch_count > 0){m_mouse[i].bPressed = false;m_mouse[i].bHeld = true;m_mouse[i].bReleased = false;}
				}
			}
			/*****************************************************************/
			// Retrieve the framebuffer

			framebuf = (u32 *)framebufferBegin(&fb, &stride);
			framebuf_width = stride / sizeof(u32);
			
			OnUserUpdate(fElapsedTime);
			char s[10];
			sprintf(s,"FPS: %3.2f",1.0f / fElapsedTime);
			DrawString(500,40,s);
			prev_touchcount = touch_count;
			// Each pixel is 4-bytes due to RGBA8888.
			// We're done rendering, so we end the frame here.
			framebufferEnd(&fb);
		}
		OnUserDestroy();
	}
	void SetFontColor(uint32_t _rgba)
	{
		fontcolor->rgba = _rgba;
	}
	void SetFontSize(uint32_t _fontsize)
	{
		fontsize = _fontsize;
	}
	void SetBackGround(const char*file_path)
	{
		background = new uint32_t[FB_WIDTH * FB_HEIGHT];
		background = PNGtoRGBA(file_path).colour;
	}
	void displayBackGround()
	{
		memcpy(framebuf,background,FB_WIDTH * FB_HEIGHT * sizeof(uint32_t));
	}
	bool MousebPressed() {return m_mouse[0].bPressed;}
	bool MousebHeld() {return m_mouse[0].bHeld;}
	bool MousebReleased() {return m_mouse[0].bReleased;}

	bool KeyDown(HidControllerKeys key) {return (kDown & key);}
	bool KeyHeld(HidControllerKeys key) {return (kHeld & key);}
	bool KeyUp(HidControllerKeys key) {return (kUp & key);}

	int ScreenWidth(){return m_nScreenWidth;}
	int ScreenHeight(){return m_nScreenHeight;}
  
public:
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	virtual bool OnUserDestroy() { return true; }
	~SwitchPixelGameEngine()
	{
		framebufferClose(&fb);
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		plExit();
		romfsExit();
		delete[] touch;
		delete fontcolor;
		delete[] background;
	}
protected:
	int m_nScreenWidth;
	int m_nScreenHeight;
	int block_size_x;
	int block_size_y;

	Framebuffer fb;
    u32 framebuf_width = 0;

	NWindow *win;
	u32 stride;
	u32 *framebuf;

	//font
	Result rc ;
	FT_Error ret = 0;
	PlFontData font;
	FT_Library library;
	FT_Face face;

	//keyboards
	u64 kDown,kHeld,kUp;

	//touchPosition
	touchPosition* touch;
	u32 touch_count,prev_touchcount = 0;
	int mouse_pos_x ,mouse_pos_y;

	//
	struct sKeyState
	{
		bool bPressed;
		bool bReleased;
		bool bHeld;
	}m_mouse[5];
  //image
	uint32_t* background;
	RGBA* fontcolor;
	uint32_t fontsize;
};

