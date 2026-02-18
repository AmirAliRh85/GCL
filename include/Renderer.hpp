#pragma once


#include "./Window.hpp"
#include "./BatchDraw.hpp"
#include <vector>
// #define STB_IMAGE_IMPLEMENTATION
// #include "../external/stb/stb_image.h"

#define RGBA_TO_GL(r , g , b , a) r / 255.0f , g / 255.0f , b / 255.0f , a / 255.f
#define RGB_TO_GL(r , g , b) r / 255.0f , g / 255.0f , b / 255.0f
#define VEC2_TO_GL(vec , window) ((float)vec.x * (2.0f / window.width)) - 1 , ((float)-(vec.y) * (2.0f / window.height)) + 1

namespace gcl2
{
    class Renderer
    {
        private :
            // WINDOW
            Window& window;

            // RENDER PROPERTY
            BatchDraw CORE;


        public :
            Renderer(Window& wnd) : window(wnd)
            {
                gladLoadGL();
                glViewport(0 , 0 , window.width , window.height);

                GLuint vs = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vs , 1 , &vertex_shader_source , NULL);
                glCompileShader(vs);

                GLint success;
                glGetShaderiv(vs , GL_COMPILE_STATUS , &success);
                if (success == 0)
                {
                    char info[1024];
                    glGetShaderInfoLog(vs , 1024 , NULL , info);
                    std::cout << "Vertex Shader Error : " << info << '\n';
                }
                
                GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fs , 1 , &fragment_shader_source , NULL);
                glCompileShader(fs);
                
                glGetShaderiv(fs , GL_COMPILE_STATUS , &success);
                if (success == 0)
                {
                    char info[1024];
                    glGetShaderInfoLog(fs , 1024 , NULL , info);
                    std::cout << "Fragment Shader Error : " << info << '\n';
                }
                
                program = glCreateProgram();
                glAttachShader(program , vs);
                glAttachShader(program , fs);
                glLinkProgram(program);
                
                glGetProgramiv(program, GL_LINK_STATUS, &success);
                if (!success) {
                    char log[1024];
                    glGetProgramInfoLog(program, 1024, nullptr, log);
                    std::cerr << "Program link error:\n" << log << std::endl;
                }

                glDeleteShader(vs);
                glDeleteShader(fs);


                CORE.init();
                CORE.setProgram(program);

                
                // ===================================
                // |            ALPHA                |
                // ===================================
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
            }
            // ===================================
            // |            TEXTURE              |
            // ===================================
            unsigned int loadImage(const char* file_path)
            {
                return CORE.Static.loadTexture(file_path);
            }

            void drawImage(Rect rct , unsigned int index_to_draw)
            {
                Vec2 point1 = {rct.x , rct.y};
                Vec2 point2 = {rct.x + rct.w , rct.y};
                Vec2 point3 = {rct.x , rct.y + rct.h};
                Vec2 point4 = {rct.x + rct.w , rct.y + rct.h};
                float temp[] = {
                    VEC2_TO_GL(point1 , window) ,    0.0f , 0.0f , 0.0f ,   0.0f , 0.0f ,
                    VEC2_TO_GL(point2 , window) ,    0.0f , 0.0f , 0.0f ,   0.0f , 0.0f ,
                    VEC2_TO_GL(point4 , window) ,    0.0f , 0.0f , 0.0f ,   0.0f , 0.0f ,

                    VEC2_TO_GL(point1 , window) ,    0.0f , 0.0f , 0.0f ,   0.0f , 0.0f ,
                    VEC2_TO_GL(point3 , window) ,    0.0f , 0.0f , 0.0f ,   0.0f , 0.0f ,
                    VEC2_TO_GL(point4 , window) ,    0.0f , 0.0f , 0.0f ,   0.0f , 0.0f ,
                };
                static bool is_atlas_generated = false;
                if (!is_atlas_generated)
                {
                    CORE.Static.generateTexture();
                    is_atlas_generated = true;
                }

                if (is_atlas_generated)
                {
                    CORE.Static.addTexture(temp , rct , index_to_draw);
                }
            }

            // unsigned int loadFont(const char* font_path , unsigned int font_size , Language lang)
            // {

            // }

            // unsigned int loadText(const char* text , unsigned int font_id)
            // {

            // }

            // void drawText(Rect rct , unsigned int text_id)
            // {
                
            // }

            // ===================================
            // |            DRAWING              |
            // ===================================
            void drawBackground(int r , int g , int b , int a)
            {
                glClearColor(RGBA_TO_GL(r , g , b , a));
                glClear(GL_COLOR_BUFFER_BIT);
            }
            void drawTriangle(Vec2 point1 , Vec2 point2 , Vec2 point3 , int r , int g , int b)
            {
                float temp[21] = {
                    VEC2_TO_GL(point1, window), RGB_TO_GL(r,g,b), -1.0f , -1.0f ,
                    VEC2_TO_GL(point2, window), RGB_TO_GL(r,g,b), -1.0f , -1.0f ,
                    VEC2_TO_GL(point3, window), RGB_TO_GL(r,g,b) , -1.0f , -1.0f
                };
                CORE.Dynamic.addPrimitive(temp , TRIANGLE);

            }
            void drawFilledRectangle(Rect rect , int r , int g , int b)
            {
                drawTriangle({rect.x , rect.y} , {rect.x + rect.w , rect.y} , {rect.x + rect.w , rect.y + rect.h} , r , g , b);
                drawTriangle({rect.x , rect.y} , {rect.x , rect.y + rect.h} , {rect.x + rect.w , rect.y + rect.h} , r , g , b);
            }
            void drawLine(Vec2 point1 , Vec2 point2 , int r , int g , int b)
            {
                // Don't know why this works
                if (point1.x == point2.x)
                {
                    point1.x++;
                    point2.x++;
                    point2.y++;
                }
                else if (point1.y == point2.y)
                {
                    point1.y++;
                    point2.y++;   
                    point2.x++;
                }
                float temp[14] = {
                    VEC2_TO_GL(point1 , window) , RGB_TO_GL(r , g , b) , -1.0f , -1.0f ,
                    VEC2_TO_GL(point2 , window) , RGB_TO_GL(r , g , b) , -1.0f , -1.0f
                };

                CORE.Dynamic.addPrimitive(temp , LINE);
            }
            void drawPoint(Vec2 point1 , int r , int g , int b)
            {
                point1.y++;
                float temp[7] = {
                    VEC2_TO_GL(point1 , window) , RGB_TO_GL(r , g , b) , -1.0f , -1.0f
                };

                CORE.Dynamic.addPrimitive(temp , POINT);
            }
            void drawRectangle(Rect rect , int thickness , int r , int g , int b)
            {
                drawLine({rect.x , rect.y} , {rect.x + rect.w - 1, rect.y} , r , g , b);
                drawLine({rect.x , rect.y} , {rect.x , rect.y + rect.h - 1} , r , g , b);
                drawLine({rect.x , rect.y + rect.h - 1} , {rect.x + rect.w - 1 , rect.y + rect.h - 1} , r , g , b);
                drawLine({rect.x + rect.w - 1, rect.y} , {rect.x + rect.w -1 , rect.y + rect.h - 1} , r , g , b);
            }
            void drawShape(Rect rect , const std::vector<bool>& shape , int r , int g , int b)
            {
                for (int i = 0 ; i < rect.h ; i++)
                {
                    for (int j = 0 ; j < rect.w ; j++)
                    {
                        if (shape[j + rect.w * i])
                            drawPoint({rect.x + j , rect.y + i} , r , g , b);
                    }
                }
            }
            void drawShapeRGB(Rect rect , const std::vector<std::vector<int>>& shape)
            {
                int r , g , b;
                for (int i = 0 ; i < rect.h ; i++)
                {
                    for (int j = 0 ; j < rect.w ; j++)
                    {
                        if (shape[i][j] != 0)
                        {
                            b = shape[i][j] % 256;
                            g = (shape[i][j] / 256) % 256;
                            r = (shape[i][j] / 256) / 256;
                            drawPoint({j + rect.x , i + rect.y} , r , g , b);
                        }
                    }
                }
            }
            void beginScissor(Rect rect)
            {
                rect.y = window.getHeight() - rect.h - rect.y;
                CORE.beginScissor(rect);
            }
            void endScissor()
            {
                CORE.endScissor();
            }


            void update()
            {
                glViewport(0 , 0 , window.width , window.height);
                glUseProgram(program);
                
                CORE.flush();

                glfwSwapBuffers(window.GLFW_window);
            }

            void destroy()
            {
                CORE.destroy();
                glDeleteProgram(program);
            }


        private :
            // NORMAL SHAPES
            const char* vertex_shader_source = "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "layout (location = 2) in vec2 aUV;\n"
            "out vec3 color;\n"
            "out vec2 UV;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x , aPos.y , 0.0f , 1.0f);\n"
            "   color = aColor;\n"
            "   UV = aUV;\n"
            "}\n";

            const char* fragment_shader_source = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec3 color;\n"
            "in vec2 UV;\n"
            "uniform sampler2D tex0;\n"
            "void main()\n"
            "{\n"
            "   if (UV.x < 0)\n"
            "       FragColor = vec4(color , 1.0f);\n"
            "   else\n"
            "       FragColor = texture(tex0 , UV);\n"
            "}\n";

            
            GLuint program;
    };  
}