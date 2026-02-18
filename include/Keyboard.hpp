#pragma once

#include "./Config.hpp"
#include "./Window.hpp"
#include <vector>
#include <string>
#include <iostream>

namespace gcl2
{
    enum KeyboardFlag
    {
        KEY_A   ,
        KEY_B   ,
        KEY_C   ,
        KEY_D   ,
        KEY_E   ,
        KEY_F   ,
        KEY_G   ,
        KEY_H   ,
        KEY_I   ,
        KEY_J   ,
        KEY_K   ,
        KEY_L   ,
        KEY_M   ,
        KEY_N   ,
        KEY_O   ,
        KEY_P   ,
        KEY_Q   ,
        KEY_R   ,
        KEY_S   ,
        KEY_T   ,
        KEY_U   ,
        KEY_V   ,
        KEY_W   ,
        KEY_X   ,
        KEY_Y   ,
        KEY_Z   ,
    };

    #define MAX_TEXT_INPUT_CHAR 64
    struct TextInput
    {
        char text[MAX_TEXT_INPUT_CHAR];
        unsigned int current_idx = 0;
    };


    #define MAX_TEXT_INPUT_FORM_CHAR 1024
    struct TextInputForm
    {
        char text[MAX_TEXT_INPUT_FORM_CHAR];
        unsigned int current_idx = 0;
    };


    std::vector<TextInput> __text_input_vec;
    std::vector<TextInputForm> __text_input_form_vec;
    int __text_input_enable = 0;
    int __text_input_form_enable = 0;


    // void __textInput(GLFWwindow* window , unsigned int code_point)
    // {
    //     __text_input_vec[__text_input_enable].text[__text_input_vec[__text_input_enable].current_idx] = (char)code_point;
    //     __text_input_vec[__text_input_enable].text[__text_input_vec[__text_input_enable].current_idx + 1] = '\0';

    // }
    // ^^^^^ TESTING ^^^^^
    // 

    std::string* str_ptr;
    void __textInput(GLFWwindow* window , unsigned int code_point)
    {
        *str_ptr += (char)code_point;
    }

    class Keyboard
    {
        friend class Window;
        private :
            GLFWwindow* GLFW_window;

        public :
            bool isPressed(KeyboardFlag keyboard_flag)
            {
                if (keyboard_flag == KEY_A) 
                    if (glfwGetKey(GLFW_window , GLFW_KEY_A) == GLFW_PRESS)
                    return true;
                if (keyboard_flag == KEY_B)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_B) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_C)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_C) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_D)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_D) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_E)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_E) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_F)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_F) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_G) 
                    if (glfwGetKey(GLFW_window , GLFW_KEY_G) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_H)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_H) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_I)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_I) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_J)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_J) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_K)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_K) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_L)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_L) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_M)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_M) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_N)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_N) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_O)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_O) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_P) 
                    if (glfwGetKey(GLFW_window , GLFW_KEY_P) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_Q)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_Q) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_R)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_R) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_S)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_S) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_T)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_T) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_U)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_U) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_V)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_V) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_W)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_W) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_X)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_X) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_Y)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_Y) == GLFW_PRESS)
                        return true;
                if (keyboard_flag == KEY_Z)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_Z) == GLFW_PRESS)
                        return true;

                return false;
            }

            bool isReleased(KeyboardFlag keyboard_flag)
            {
                if (keyboard_flag == KEY_A)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_A) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_B)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_B) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_C)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_C) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_D)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_D) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_E)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_E) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_F)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_F) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_G)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_G) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_H)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_H) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_I)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_I) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_J) 
                    if (glfwGetKey(GLFW_window , GLFW_KEY_J) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_K)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_K) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_L)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_L) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_M)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_M) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_N)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_N) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_O)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_O) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_P)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_P) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_Q)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_Q) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_R)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_R) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_S) 
                    if (glfwGetKey(GLFW_window , GLFW_KEY_S) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_T)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_T) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_U)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_U) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_V)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_V) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_W)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_W) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_X)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_X) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_Y)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_Y) == GLFW_RELEASE)
                        return true;
                if (keyboard_flag == KEY_Z)
                    if (glfwGetKey(GLFW_window , GLFW_KEY_Z) == GLFW_RELEASE)
                        return true;

                return false;
            }
            
            // void createTextInput(int amount)
            // {
            //     __text_input_vec = std::vector<TextInput>(amount);
            // }

            // void enableTextInput(int enable)
            // {
            //     __text_input_enable = enable;
            //     glfwSetCharCallback(GLFW_window , __textInput);
            // }

            // void textInput()
            // {
            //     idx = __text_input_form_vec[__text_input_enable].current_idx;
            //     ch = &__text_input_form_vec[__text_input_enable].text[idx];
            // }
            // 
            // ^^^^^ TESTING ^^^^^

            void enableTextInput(std::string& text)
            {
                str_ptr = &text;
                glfwSetCharCallback(GLFW_window , __textInput);
            }
    };
}