#include "SwitchPixelGameEngine.h"
#include <string>
class Demo : public SwitchGameEngine
{
public:
    Demo()
    {
    }
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
        Fill(0,0,ScreenWidth()/5,ScreenHeight()/5,FG_RED);
        Fill(ScreenWidth()/15,ScreenHeight()/15,ScreenWidth()/4, ScreenHeight()/4,0x4400FFff);
        DrawTriangle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/3-100,ScreenHeight()/3+100,ScreenWidth()/4,ScreenHeight()/4,FG_YELLOW);
        DrawCircle(ScreenWidth()/2,ScreenHeight()/2,ScreenWidth()/10,FG_GREEN);
        DrawString(ScreenWidth()/2, ScreenHeight()/2, s_str);  
        /**********************************************************************************/
        //keyboard input 
        if(KeyDown(KEY_UP) || KeyHeld(KEY_UP) ) 
        {
            DrawString(0,ScreenHeight()/2,"UP KEY is down");
            picture->SetPosition(picture->GetPos_x(),picture->GetPos_y()-10);
        }
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
    std::string s_str = "hello world !";
    int sum =0;
    SgeSprite* picture;
};

int main()
{
    Demo example;
    example.ConstructConsole(1280,720,1,1);
    example.GameThread();
    return 0;
}