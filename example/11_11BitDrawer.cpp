#include "../GCL2.hpp"
#include <math.h>
#include <fstream>

/* IMPORTANT TO LEARN

Instanced rendering

Uniform buffers (UBO)

Texture atlases

State sorting
*/

int main()
{
    gcl2::init();
    gcl2::Window window(700 , 700 , "GCL2" , gcl2::WINDOW_POS_CENTERED , 50 , 50);
    gcl2::Renderer rend(window);
    window.log();


    bool shape[70][70];
    #define test 123 , 34 , 56
    while(window.isRunning())
    {
        window.update();
        rend.drawBackground(100 , 100 , 100 , 235);



        if (window.mouse.isPressed(gcl2::BUTTON_LEFT))
        {
            shape[window.mouse.getPosX() / 10][window.mouse.getPosY() / 10] = 1;
        }
        else if (window.mouse.isPressed(gcl2::BUTTON_RIGHT))
        {
            shape[window.mouse.getPosX() / 10][window.mouse.getPosY() / 10] = 0;
        }
        
        for (int i = 0 ; i < 70 ; i++)
        {
            for (int j = 0 ; j < 70 ; j++)
            {
                if (shape[i][j] == 1)
                    rend.drawFilledRectangle({i * 10 , j * 10 , 10 , 10} , test);
            }
        }

        if (window.keyboard.isPressed(gcl2::KEY_A))
        {
        std::ofstream file("shape.txt");
            for (int i = 0 ; i < 70 ; i++)
            {
                std::string temp = "";
                for (int j = 0 ; j < 70 ; j++)
                {
                    if (shape[i][j])
                        temp += "1,";
                    else
                        temp += "0,";
                }
                file << temp;
                file << '\n';
            }
        }
        if (window.keyboard.isPressed(gcl2::KEY_C))
        {
            for (int i = 0 ; i < 70 ; i++)
            {
                for (int j = 0 ; j < 70 ; j++)
                {
                    shape[i][j] = 0;
                }
            }
        }

        rend.update();
    }

    rend.destroy();
    window.destroy();
    gcl2::quit();
}