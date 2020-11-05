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

        Fill(0,0,100,100,FG_RED);
        Fill(50,50,200, 200,FG_YELLOW);

        DrawTriangle(200,200,300,400,500,500,FG_YELLOW);
        DrawCircle(300,400,100,FG_GREEN);

        DrawString(ScreenWidth()/2, ScreenHeight()/2, s_str);

        if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
        {
            if(kDown & KEY_UP) DrawString(0,ScreenWidth()/2,"UP KEY is down");
            if(kUp & KEY_UP) DrawString(0,ScreenWidth()/3,"UP KEY is kUp");
            if(kHeld & KEY_UP) DrawString(0,ScreenWidth()/4,"UP KEY is held");
        }
        std::string tmp_str;
        //if  touch_count != prev_touchcount means mouse touch ! 
        if (touch_count != prev_touchcount)
        {
            prev_touchcount = touch_count;
            //replace here for your code 
            char t1[10];
            sprintf(t1,"%d %d",mouse_pos_x ,mouse_pos_y);
            DrawString(ScreenWidth()/4,ScreenWidth()/4,std::string("mouse pos is ")+std::string(t1));
            //
        } 


        return true;
    }
};

int main()
{
    Demo example;
    example.ConstructConsole(1280,720,1,1);
    example.GameThread();
}