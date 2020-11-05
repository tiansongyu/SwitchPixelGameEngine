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