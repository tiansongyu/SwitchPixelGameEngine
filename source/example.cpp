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
#include "SwitchPixelGameEngine.h"
#include <string>
class Demo : public SwitchPixelGameEngine
{
public:
    Demo()
    {
    }
    virtual bool OnUserCreate() override
    {
        picture = new SgeSprite(ScreenWidth()* 2 /3 , 0,"romfs:/image/maliao.png");
        SetBackGround("romfs:/image/background.png");
        return true;
    }
    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        ClearScreen();
        //SetFontColor(FG_YELLOW);
        //SetFontSize(LARGE_FONT);
        /**********************************************************************************/
        //Draw and Fill graphics
        displayBackGround();
        Fill(0,0,ScreenWidth()/5,ScreenHeight()/5,FG_RED);
        Fill(ScreenWidth()/15,ScreenHeight()/15,ScreenWidth()/4, ScreenHeight()/4,0x4400FFff);
        DrawTriangle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/3-100,ScreenHeight()/3+100,ScreenWidth()/4,ScreenHeight()/4,FG_YELLOW);
        DrawCircle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/10,FG_GREEN);
        DrawString(ScreenWidth()/2, ScreenHeight()/2, s_str);  
        /**********************************************************************************/
        //keyboard input 
        if(KeyDown(KEY_UP) || KeyHeld(KEY_UP) ) 
        {
            picture->SetPosition(picture->GetPos_x(),picture->GetPos_y()-10);
        }
        if(KeyDown(KEY_UP)) DrawString(0,ScreenHeight()/2,"UP KEY is down");
        if(KeyUp(KEY_UP)) DrawString(0,ScreenHeight()/3,"UP KEY is kUp");
        if(KeyHeld(KEY_UP)) DrawString(0,ScreenHeight()/4,"UP KEY is held");

        if(KeyDown(KEY_DOWN) || KeyHeld(KEY_DOWN)) 
        {
            picture->SetPosition(picture->GetPos_x(),picture->GetPos_y()+10);
        }
        if(KeyDown(KEY_LEFT) || KeyHeld(KEY_LEFT)) 
        {
            picture->SetPosition(picture->GetPos_x()-10,picture->GetPos_y());
        }
        if(KeyDown(KEY_RIGHT) || KeyHeld(KEY_RIGHT)) 
        {
            picture->SetPosition(picture->GetPos_x()+10,picture->GetPos_y());
        }
        /**********************************************************************************/
        //mouse input
        if(MousebReleased())
        {
            sum++;
        }
        char t1[50];
        sprintf(t1,"%d %d\nmouse released number is %d",mouse_pos_x,mouse_pos_y, sum);
        DrawString(ScreenWidth()/2-200,ScreenHeight()/2 + 200,std::string("mouse pos is") + std::string(t1));
        /**********************************************************************************/
        DrawSprite(picture); 
        return true;
    }
    virtual bool OnUserDestroy() override
    {
        delete picture;
        return true;
    }
 public:
    std::string s_str = "你好 world !";
    int sum =0;
    SgeSprite* picture;
};

int main()
{
    Demo example;
    example.ConstructConsole(1,1);
    example.GameThread();
    return 0;
}