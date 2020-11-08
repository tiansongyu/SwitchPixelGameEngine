#include "SwitchPixelGameEngine.h"
#include <string>
class Demo : public SwitchGameEngine
{
public:
    Demo()
    {
    }
    std::string s_str = "hello world !";
    int sum =0;
    SgeSprite* picture;
    virtual bool OnUserCreate() override
    {
        picture = new SgeSprite(800,0,447,450,"romfs:/maliao.bin");
        //SetBackGround("romfs:/image.bin");
        return true;
    }
    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        ClearAll();
        /**********************************************************************************/
        // Draw and Fill graphics

        //displayBackGround();

        Fill(0,0,ScreenWidth()/10,ScreenHeight()/10,FG_RED);
        Fill(ScreenWidth()/10,ScreenHeight()/10,ScreenWidth()/2, ScreenHeight()/2,FG_YELLOW);
        DrawTriangle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/3,ScreenHeight()/3,ScreenWidth()/4,ScreenHeight()/4,FG_YELLOW);
        DrawCircle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/10,FG_GREEN);
        DrawString(ScreenWidth()/2, ScreenHeight()/2, s_str);  
        /**********************************************************************************/
        //keyboard input 
        if(KeyDown(KEY_UP)) DrawString(0,ScreenHeight()/2,"UP KEY is down");
        if(KeyUp(KEY_UP)) DrawString(0,ScreenHeight()/3,"UP KEY is kUp");
        if(KeyHeld(KEY_UP)) DrawString(0,ScreenHeight()/4,"UP KEY is held");
        /**********************************************************************************/
        //mouse input
        if(MousebReleased())
        {
            sum++;
        }
        char t1[20];
        sprintf(t1,"%d",sum);
        DrawString(ScreenWidth()/2,ScreenHeight()/3,std::string("mouse pos is") + std::string(t1));
        /**********************************************************************************/

        DrawSprite(picture);  
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