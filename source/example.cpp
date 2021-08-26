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
class Demo : public SwitchPixelGameEngine
{
public:
    Demo()
    {
    }
    virtual bool OnUserCreate() override
    {
        picture = new SgeSprite(ScreenWidth()* 2 /3 , 0,"romfs:/image/maliao.png");
        AudioInit();
        SetBackGround("romfs:/image/background.png");
        AudioPlay("romfs:/audio/test.mp3");
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
        DrawRect(20,20,100,100,FG_RED);
        FillRect(50,50,100, 100,0x4400FFff);
        DrawTriangle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/3-100,ScreenHeight()/3+100,ScreenWidth()/4,ScreenHeight()/4,FG_YELLOW);
        DrawCircle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/10,FG_GREEN);
        DrawString(ScreenWidth()/2, ScreenHeight()/2, s_str);  
        /**********************************************************************************/
        //keyboard input
        if (KeyDown(HidNpadButton_Up) || KeyHeld(HidNpadButton_Up))
        {
            picture->SetPosition(picture->GetPos_x(),picture->GetPos_y()-10);
        }
        if(KeyDown(HidNpadButton_Up)) DrawString(0,ScreenHeight()/2,"UP KEY is down");
        if(KeyUp(HidNpadButton_Up)) DrawString(0,ScreenHeight()/3,"UP KEY is kUp");
        if(KeyHeld(HidNpadButton_Up)) DrawString(0,ScreenHeight()/4,"UP KEY is held");

        if (KeyDown(HidNpadButton_Down) || KeyHeld(HidNpadButton_Down))
        {
            picture->SetPosition(picture->GetPos_x(),picture->GetPos_y()+10);
        }
        if (KeyDown(HidNpadButton_Left) || KeyHeld(HidNpadButton_Left))
        {
            picture->SetPosition(picture->GetPos_x()-10,picture->GetPos_y());
        }
        if (KeyDown(HidNpadButton_Right) || KeyHeld(HidNpadButton_Right))
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
        sprintf(t1,"mouse pos is %d %d\nmouse released number is %d",mouse_pos_x,mouse_pos_y, sum);
        DrawString(ScreenWidth()/2-200,ScreenHeight()/2 + 200,t1,2);
        /**********************************************************************************/
        //DrawSprite(picture); 
        DrawPartialSprite(picture,200,200,100,100);

        return true;
    }
    virtual bool OnUserDestroy() override
    {
        delete picture;
        return true;
    }
 public:
    const char* s_str = "你好 world !";
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