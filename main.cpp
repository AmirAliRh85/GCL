#include "./GCL2.hpp"
#include <math.h>
#include <fstream>

using namespace gcl2;
/* IMPORTANT TO LEARN

Instanced rendering

Uniform buffers (UBO)

Texture atlases

State sorting
*/


int main()
{
    init();

    Window window(400 , 400 , "main" , WINDOW_SHOWN , 50 , 50);
    Renderer rend(window);
    Rect rect = {10 , 10 , 100 , 100};
    
    rend.loadFont("Roboto-Regular.ttf" , 12 , LANGUAGE_ENGLISH);
    rend.loadImage("icon-user.png");
    rend.loadImage("./brick.jpg");

    const char* str = "Hello World!";
    while(window.isRunning())
    {
        window.update();
        rend.drawBackground(100 , 100 , 100 , 235);


        rend.drawRectangle({10 , 10 , 100 , 100} , 1 , 135 , 10 , 10);
        rend.drawRectangle({20 , 20 , 100 , 100} , 1 , 135 , 10 , 10);

        rend.drawFilledRectangle({220 , 220 , 10 , 10} , 120 , 120 , 240);
        rend.drawFilledRectangle({230 , 230 , 10 , 10} , 120 , 120 , 240);

        // rend.drawImage(rect , 0);
        rend.drawImage({160 , 10 , 50 , 50} , 0);

        rend.drawImage({250 , 250 , 64 , 64} , 1);

        rend.drawImage({260 , 10 , 50 , 50} , 0);


        rend.drawText({10 , 20} , str , 0);


        if (window.keyboard.isPressed(KEY_W))
            str += 'w';
        if (window.keyboard.isPressed(KEY_S))
            str += 'h';

        rend.update();
    }


    rend.destroy();
    window.destroy();
    quit();
}