#pragma once

#include "../external/glad/include/glad.h"
#include <GLFW/glfw3.h>

#define GCL_MAJOR_VERSION   1
#define GCL_MINOR_VERSION   1
#define GCL_PATCH_VERSION   0

#define GCL_VERSION         "1.1.0"

/*
*   1.0.0       Initial version
*   1.1.0       ADD : rendering Text
*/


namespace gcl2
{
    inline void init() { glfwInit() ; }
    inline void quit() { glfwTerminate() ; }

    struct Vec2
    {
        int x;
        int y;
    };

    struct Vec2f
    {
        float x;
        float y;
    };

    struct Rect
    {
        void center(Rect rct)
        {
            y = rct.y + (rct.h - h) / 2;
            x = rct.x + (rct.w - w) / 2;
        }
        int x , y;
        int w , h;
    };  


    struct ColorRGBA
    {
        int r;
        int g;
        int b;
        int a;
    };

    struct ColorRGB
    {
        int r;
        int g;
        int b;
    };

    #define RED         255 , 0 , 0
    #define GREEN       0 , 255 , 0
    #define BLUE        0 , 0 , 255
    #define WHITE       255 , 255 , 255
    #define BLACK       0 , 0 , 0
} // End of namespace sgcl2