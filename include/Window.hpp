#pragma once

#include "./Config.hpp"
#include "./Keyboard.hpp"
#include "./Mouse.hpp"
#include <iostream>
#include <string>
#include <functional>



namespace gcl2
{
    enum WindowFlag
    {
        WINDOW_SHOWN                    = 0b0000000001  ,
        WINDOW_MAXIMIZED                = 0b0000000010  ,
        WINDOW_MINIMIZED                = 0b0000000100  ,
        WINDOW_FULLSCREEN               = 0b0000001000  ,
        WINDOW_POS_CENTERED             = 0b0000010000  ,
        WINDOW_UNRESIZABLE              = 0b0000100000  ,
        WINDOW_BORDERLESS               = 0b0001000000  ,
        WINDOW_TRANSPARENT              = 0b0010000000  ,
    };



    class Window
    {
        friend class Renderer;

        private :
            // WINDOW PROPERTY
            int width , height , x , y;
            int window_flag;
            const std::string title;

            // GLFW PROPERTY
            GLFWwindow* GLFW_window;
            GLFWmonitor* GLFW_monitor;
            const GLFWvidmode* GLFW_mode;

        public :
            // KEYBOARD
            Keyboard keyboard;

            // MOUSE
            Mouse mouse;

            Window(int w , int h , const std::string window_name , int wnd_flag , int x_pos , int y_pos) : title(window_name)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE); 
                #ifdef __APPLE__
                    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT , GLFW_TRUE);
                #endif
                
            
                GLFW_monitor = glfwGetPrimaryMonitor();
                GLFW_mode = glfwGetVideoMode(GLFW_monitor);
                x = x_pos;
                y = y_pos;
                window_flag = wnd_flag;


                if (WINDOW_BORDERLESS & window_flag)
                    glfwWindowHint(GLFW_DECORATED , 0);
                if (WINDOW_TRANSPARENT & window_flag)
                    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER , 1);
                if (WINDOW_MINIMIZED & window_flag)
                    glfwWindowHint(GLFW_VISIBLE , 0);
                if (WINDOW_UNRESIZABLE & window_flag)
                    glfwWindowHint(GLFW_RESIZABLE , 0);
                if (WINDOW_FULLSCREEN & window_flag)
                {
                    GLFW_window = glfwCreateWindow(GLFW_mode->width , GLFW_mode->height , title.c_str() , GLFW_monitor , NULL);
                    width = GLFW_mode->width;
                    height = GLFW_mode->height;
                }

                if (!(WINDOW_FULLSCREEN & window_flag))
                {   
                    GLFW_window = glfwCreateWindow(w , h , title.c_str() , NULL , NULL);
                    width = w;
                    height = h;
                    if (WINDOW_POS_CENTERED & window_flag)
                    {
                        x = GLFW_mode->width / 2 - w / 2;
                        y = GLFW_mode->height / 2 - h / 2;
                        glfwSetWindowPos(GLFW_window , x , y);
                    }
                    else if (WINDOW_MAXIMIZED & window_flag)
                    {
                        glfwMaximizeWindow(GLFW_window);
                    }
                    else
                    {
                        glfwSetWindowPos(GLFW_window , x , y);
                    }
                }


                glfwGetWindowPos(GLFW_window , &x , &y);
                glfwGetWindowSize(GLFW_window , &width , &height);

                glfwMakeContextCurrent(GLFW_window);
                glfwSwapInterval(1);

                keyboard.GLFW_window = GLFW_window;
                mouse.GLFW_window = GLFW_window;
                mouse.setCursor(ARROW_CURSOR);
            }


            void setOpacity(int alpha)
            {
                if (WINDOW_TRANSPARENT & window_flag)
                {
                    glfwSetWindowOpacity(GLFW_window , alpha / 255.0f);
                }
                else
                {
                    std::cerr << "Warning : Transparency is not On!" << '\n';
                }
            }
            void setStopRunning() { glfwSetWindowShouldClose(GLFW_window , 1) ; }
            void setTitle(const std::string window_name) { glfwSetWindowTitle(GLFW_window , window_name.c_str()) ; }

            
            int getPosX() { return x ; }
            int getPosY() { return y ; }
            int getHeight() { return height ; }
            int getWidth() { return width ; }


            void log()
            {
                std::cout << "============================" << '\n';
                std::cout << "|     WINDOW PROPERTY      |" << '\n';
                std::cout << "============================" << '\n';
                std::cout << "w : " << width << '\n';
                std::cout << "h : " << height << '\n';
                std::cout << "x : " << x << '\n';
                std::cout << "y : " << y << '\n';
                std::cout << "name : " << title << '\n';
            }
            bool isRunning() { return !glfwWindowShouldClose(GLFW_window) ; }
            void update()
            {
                // glfwPollEvents(); // --> should not be uses with glfwWaitEvents()
                glfwWaitEvents();
                glfwGetCursorPos(GLFW_window , &mouse.x , &mouse.y);
                glfwGetWindowPos(GLFW_window , &x , &y);
                glfwGetWindowSize(GLFW_window , &width , &height);
            }
            inline void destroy()
            { 
                mouse.destroy();
                glfwDestroyWindow(GLFW_window);
            }
    };
} // End of namespace gcl2