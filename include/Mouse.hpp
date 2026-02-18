#pragma once

#include "./Config.hpp"

namespace gcl2
{

    enum MouseFlag
    {
        BUTTON_LEFT             ,
        BUTTON_RIGHT            ,
        BUTTON_MIDDLE           ,
    };

    enum CursorFlag
    {
        HAND_CURSOR             ,
        POINTING_HAND_CURSOR    ,
        ARROW_CURSOR            ,
        IBEAM_CURSOR            ,
        CENTER_CURSOR           ,
        CROSSHAIR_CURSOR        ,
        NOT_ALLOWED_CURSOR      ,
        HRESIZE_CURSOR          ,
        VRESIZE_CURSOR          ,
        RESIZE_EW_CURSOR        ,
        RESIZE_NS_CURSOR        ,
        RESIZE_NESW_CURSOR      ,
        RESIZE_NWSE_CURSOR      ,
    };


    class Mouse
    {
        friend class Window;

        private :
            GLFWwindow* GLFW_window = NULL;
            GLFWcursor* GLFW_cursor = NULL;
            CursorFlag cursor_flag = ARROW_CURSOR;
            double x;
            double y;

        public :
            bool isIn(Rect rect)
            {
                if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
                    return true;

                return false;
            }

            bool isPressed(MouseFlag mouse_flag)
            {
                if (mouse_flag == BUTTON_LEFT)
                    if (glfwGetMouseButton(GLFW_window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                        return true;
                if (mouse_flag == BUTTON_RIGHT)
                    if (glfwGetMouseButton(GLFW_window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
                        return true;
                if (mouse_flag == BUTTON_MIDDLE)
                    if (glfwGetMouseButton(GLFW_window , GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
                        return true;

                return false;
            }
            
            bool isReleased(MouseFlag mouse_flag)
            {
                if (mouse_flag == BUTTON_LEFT)
                    if (glfwGetMouseButton(GLFW_window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
                        return true;
                if (mouse_flag == BUTTON_RIGHT)
                    if (glfwGetMouseButton(GLFW_window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
                        return true;
                if (mouse_flag == BUTTON_MIDDLE)
                    if (glfwGetMouseButton(GLFW_window , GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
                        return true;

                return false;
            }

            bool isClicked(MouseFlag mouse_flag)
            {
                static bool button_left_is_released = true;
                static bool button_right_is_released = true;
                static bool button_middle_is_released = true;
                if (mouse_flag == BUTTON_LEFT)
                {
                    if (isPressed(BUTTON_LEFT) && button_left_is_released)
                        button_left_is_released = false;
                    else if (isReleased(BUTTON_LEFT) && !button_left_is_released)
                    {
                        button_left_is_released = true;
                        return true;
                    }
                }
                else if (mouse_flag == BUTTON_RIGHT)
                {
                    if (isPressed(BUTTON_RIGHT) && button_right_is_released)
                        button_right_is_released = false;
                    else if (isReleased(BUTTON_RIGHT) && !button_right_is_released)
                    {
                        button_right_is_released = true;
                        return true;
                    }
                }
                else if (mouse_flag == BUTTON_MIDDLE)
                {
                    if (isPressed(BUTTON_MIDDLE) && button_middle_is_released)
                        button_middle_is_released = false;
                    else if (isReleased(BUTTON_MIDDLE) && !button_middle_is_released)
                    {
                        button_middle_is_released = true;
                        return true;
                    }
                }
                return false;
            }


            void setCursor(CursorFlag cf)
            {
                if (cursor_flag != cf)
                {
                    switch (cf)
                    {
                        case ARROW_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
                            break;
                        case HAND_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
                            break;
                        case IBEAM_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
                            break;
                        case  CENTER_CURSOR:
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_CENTER_CURSOR);
                            break;
                        case CROSSHAIR_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
                            break;
                        case NOT_ALLOWED_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
                            break;
                        case HRESIZE_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
                            break;
                        case VRESIZE_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
                            break;
                        case RESIZE_NS_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
                            break;
                        case RESIZE_EW_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
                            break;
                        case RESIZE_NESW_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
                            break;
                        case RESIZE_NWSE_CURSOR :
                            GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
                            break;
                    }
                    
                    cursor_flag = cf;
                    glfwSetCursor(GLFW_window , GLFW_cursor);
                }
            }


            int getPosX() { return (int)x ; }
            int getPosY() { return (int)y ; }


            void destroy()
            {
                if (GLFW_cursor != NULL)
                    glfwDestroyCursor(GLFW_cursor);
            }
    };
}