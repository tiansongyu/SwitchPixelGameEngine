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
        
        /**********************************************************************************/
        // Draw and Fill graphics 
        Fill(0,0,ScreenWidth()/10,ScreenHeight()/10,FG_RED);
        Fill(ScreenWidth()/10,ScreenHeight()/10,ScreenWidth()/2, ScreenHeight()/2,FG_YELLOW);
        DrawTriangle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/3,ScreenHeight()/3,ScreenWidth()/4,ScreenHeight()/4,FG_YELLOW);
        DrawCircle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/10,FG_GREEN);
        DrawString(ScreenWidth()/2, ScreenHeight()/2, s_str);
        /**********************************************************************************/


        /**********************************************************************************/
        //keyboard input 

        if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
        {
            if(kDown & KEY_UP) DrawString(0,ScreenWidth()/2,"UP KEY is down");
            if(kUp & KEY_UP) DrawString(0,ScreenWidth()/3,"UP KEY is kUp");
            if(kHeld & KEY_UP) DrawString(0,ScreenWidth()/4,"UP KEY is held");
        }
        /**********************************************************************************/


        /**********************************************************************************/
        //mouse input
        //if  touch_count != prev_touchcount means mouse touch ! 
        if (touch_count != prev_touchcount)
        {
            prev_touchcount = touch_count;
            //replace here for your code 

        } 
        
        //and delete this 
        char t1[20];
        sprintf(t1,"%d %d",mouse_pos_x ,mouse_pos_y);
        DrawString(ScreenWidth()/2,ScreenWidth()/3,std::string("mouse pos is") + std::string(t1));

        /**********************************************************************************/

       return true;
    }
};

int main()
{
    Demo example;
    example.ConstructConsole(1280,720,1,1);
    example.GameThread();
    return 0;
}