#include "./BatchDraw.hpp"
#include <cstring>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace gcl2
{
    /*
    *   BatchDraw Initialization
    *   1. setProgram
    *   2. init
    * 
    */
    // ============================================================================================================================
    void BatchDraw::setProgram(unsigned int prg)
    {
        stbi_set_flip_vertically_on_load(1);
        program = prg;
    }

    void BatchDraw::init()
    {
        glGenVertexArrays(1 , &vao);
        glGenBuffers(1 , &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER , vbo);
        
        glBufferData(GL_ARRAY_BUFFER , sizeof(float) * VERTEX_ATTRIBUTE * MAX_DYNAMIC_PRIMITIVE_BATCH_DRAW , NULL , GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)(sizeof(float) * 2));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2 , 2 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);
    }
    // ============================================================================================================================


    // ============================================================================================================================
    // Bathcing for primitive types
    BatchDraw::_Dynamic::_Dynamic(BatchDraw* own) : owner(own) { }

    void BatchDraw::_Dynamic::addPrimitive(float* arr , Shape shape)
    {
        memcpy(vertexDynamic + currVertexDynamicPrimitveIdx + currVertexDynamicTextureIdx , arr , ((int)shape) * sizeof(float));
        currVertexDynamicPrimitveIdx += (int)shape;
        if (owner->currBatchShape == shape)
        {
            owner->batch[owner->currBatchIdx + 1]++;
        }
        else
        {
            if (owner->currBatchShape == NONE)
            {
                owner->batch[owner->currBatchIdx] = shape;
                owner->batch[owner->currBatchIdx + 1] = 1;
                owner->currBatchShape = shape;
            }
            else
            {
                if (owner->currBatchShape == BEGIN_SCISSOR || owner->currBatchShape == END_SCISSOR)
                    owner->currBatchIdx += 1;
                else
                    owner->currBatchIdx += 2;

                owner->batch[owner->currBatchIdx] = shape;
                owner->batch[owner->currBatchIdx + 1] = 1;
                owner->currBatchShape = shape;
            }
        }
    }
    // ============================================================================================================================




    // Batching for Static types
    // ============================================================================================================================
    BatchDraw::_Static::_Static(BatchDraw* own) : owner(own) { }

    unsigned int BatchDraw::_Static::loadImage(const char* file_path)
    {
        int w , h;
        unsigned char* byte = stbi_load(file_path , &w , &h , NULL , 4);

        if (!TA.isInitialized)
        {
            TA.isInitialized = true;
            TA.width = ATLAS_WIDTH;
            TA.height = ATLAS_HEIGHT;
            TA.shelves.push_back({{w , 0} , h});
            TA.heightOccupied = h;

            image[currTexturesIdx++] = {{0 , 0} , {w , h}};
            
            glGenTextures(1 , &texID);
            glBindTexture(GL_TEXTURE_2D , texID);

            glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA8 , TA.width , TA.height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);

            glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , 0 , w , h , GL_RGBA , GL_UNSIGNED_BYTE , byte);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            bool is_inserted = false;
            for (auto& s : TA.shelves)
            {
                if (h <= s.height && s.current.x + w <= TA.width)
                {
                    image[currTexturesIdx++] = {{s.current.x , s.current.y} , {s.current.x + w , s.current.y + h}};
                    
                    glTexSubImage2D(GL_TEXTURE_2D , 0 , s.current.x , s.current.y , w , h , GL_RGBA , GL_UNSIGNED_BYTE , byte);
                    
                    s.current.x += w;
                    is_inserted = true;
                    break;
                }
            }

            if (!is_inserted)
            {
                TA.shelves.push_back({{0 , TA.heightOccupied} , h});
                image[currTexturesIdx++] = {{0 , TA.heightOccupied} , {w , TA.heightOccupied + h}};
                
                glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , TA.heightOccupied , w , h , GL_RGBA , GL_UNSIGNED_BYTE , byte);
                
                TA.heightOccupied += h;
            }
        }

        stbi_image_free(byte);
        return currTexturesIdx - 1;
    }


    void BatchDraw::_Static::addImage(float* arr , unsigned int target_index)
    {
        Image& temp_image = image[target_index];
        float U1 = temp_image.bottomLeft.x / TA.width   ;
        float U2 = temp_image.topRight.x / TA.width     ;
        float V1 = temp_image.bottomLeft.y / TA.height  ;
        float V2 = temp_image.topRight.y / TA.height    ;

        arr[5]  = U1    ; arr[6]  =  V2    ;
        arr[12] = U2    ; arr[13] =  V2    ;
        arr[19] = U2    ; arr[20] =  V1    ;

        arr[26] = U1    ; arr[27] =  V2    ;
        arr[33] = U1    ; arr[34] =  V1    ;
        arr[40] = U2    ; arr[41] =  V1    ;
        memcpy(vertexStaticTexture + currVertexStaticTextureIdx , arr , sizeof(float) * ((int)RECTANGLE));
        currVertexStaticTextureIdx += (int)RECTANGLE;
        if (owner->currBatchShape == TRIANGLE)
        {
            owner->batch[owner->currBatchIdx + 1] += 2;
        }
        else
        {
            if (owner->currBatchShape == NONE)
            {
                owner->batch[owner->currBatchIdx] = TRIANGLE;
                owner->batch[owner->currBatchIdx + 1] = 2;
                owner->currBatchShape = TRIANGLE;
            }
            else
            {
                if (owner->currBatchShape == BEGIN_SCISSOR || owner->currBatchShape == END_SCISSOR)
                    owner->currBatchIdx += 1;
                else
                    owner->currBatchIdx += 2;

                owner->batch[owner->currBatchIdx] = TRIANGLE;
                owner->batch[owner->currBatchIdx + 1] = 2;
                owner->currBatchShape = TRIANGLE;
            }
        }
    }


    unsigned int BatchDraw::_Static::loadFont(const char* font_path , int r , int g , int b , unsigned int font_size  , Language lang)
    {
        EF[currEFIdx].fontSize = font_size;

        if (lang == LANGUAGE_ENGLISH)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            FT_Library ft;
            FT_Face face;
            FT_Init_FreeType(&ft);
            FT_New_Face(ft , font_path , 0 , &face);
            FT_Set_Pixel_Sizes(face , 0 , font_size);

            for (unsigned char c = 0 ; c < 128 ; c++)
            {
                if (FT_Load_Char(face , c , FT_LOAD_RENDER))
                    continue;

                int w = face->glyph->bitmap.width;
                int h = face->glyph->bitmap.rows;

                std::vector<unsigned char> rgba(w * h * 4);

                for (int i = 0; i < w * h; ++i)
                {
                    rgba[i*4 + 0] = r;
                    rgba[i*4 + 1] = g;
                    rgba[i*4 + 2] = b;
                    rgba[i*4 + 3] = face->glyph->bitmap.buffer[i];
                }

                if (!TA.isInitialized)
                {
                    TA.isInitialized = true;
                    TA.width = ATLAS_WIDTH;
                    TA.height = ATLAS_HEIGHT;
                    TA.shelves.push_back({{w , 0} , h});
                    TA.heightOccupied = h;

                    EF[currEFIdx].characters[c] = {{w , h} , {face->glyph->bitmap_left , face->glyph->bitmap_top} , {0 , 0} , {w , h} , face->glyph->advance.x};
                   
                    glGenTextures(1 , &texID);
                    glBindTexture(GL_TEXTURE_2D , texID);

                    glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA8 , TA.width , TA.height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);

                    // face->glyph->bitmap.buffer
                    glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , 0 , w , h , GL_RGBA , GL_UNSIGNED_BYTE , rgba.data());

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }
                else
                {
                    bool is_inserted = false;
                    for (auto& s : TA.shelves)
                    {
                        if (h <= s.height && s.current.x + w <= TA.width)
                        {
                            glTexSubImage2D(GL_TEXTURE_2D , 0 , s.current.x , s.current.y , w , h , GL_RGBA , GL_UNSIGNED_BYTE , rgba.data());
                            
                            EF[currEFIdx].characters[c] = {{w , h} , {face->glyph->bitmap_left , face->glyph->bitmap_top} , {s.current.x , s.current.y} , {s.current.x + w , s.current.y + h} , face->glyph->advance.x};

                            s.current.x += w;
                            is_inserted = true;
                            break;
                        }
                    }

                    if (!is_inserted)
                    {
                        TA.shelves.push_back({{0 , TA.heightOccupied} , h});
                        
                        glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , TA.heightOccupied , w , h , GL_RGBA , GL_UNSIGNED_BYTE , rgba.data());

                        EF[currEFIdx].characters[c] = {{w , h} , {face->glyph->bitmap_left , face->glyph->bitmap_top} , {0 , TA.heightOccupied} , {w , TA.heightOccupied + h} , face->glyph->advance.x};
                        
                        TA.heightOccupied += h;
                    }
                } 
            }

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }


        currEFIdx++;
        return currEFIdx - 1;
    }

    void BatchDraw::_Static::addText(Vec2 point , const char* text , unsigned int font_id , Vec2 wnd)
    {
        char c;
        int x_pos , y_pos;
        int x = point.x , y = point.y;
        while (*text != '\0')
        {
            c = *text;
            Character& ch = EF[font_id].characters[c]   ;
            float U1 = ch.bottomLeft.x / TA.width       ;
            float U2 = ch.topRight.x / TA.width         ;
            float V1 = ch.bottomLeft.y / TA.height      ;
            float V2 = ch.topRight.y / TA.height        ;

            x_pos = x + ch.bearing.x;
            y_pos = y - ch.bearing.y;

            Vec2 point1 = {x_pos , y_pos};
            Vec2 point2 = {x_pos + ch.size.x , y_pos};
            Vec2 point3 = {x_pos , y_pos + ch.size.y};
            Vec2 point4 = {x_pos + ch.size.x , y_pos + ch.size.y};
            float arr[] = {
                _VEC2_TO_GL(point1 , wnd) ,    0.0f , 0.0f , 0.0f ,   U1 , V1 ,
                _VEC2_TO_GL(point2 , wnd) ,    0.0f , 0.0f , 0.0f ,   U2 , V1 ,
                _VEC2_TO_GL(point4 , wnd) ,    0.0f , 0.0f , 0.0f ,   U2 , V2 ,

                _VEC2_TO_GL(point1 , wnd) ,    0.0f , 0.0f , 0.0f ,   U1 , V1 ,
                _VEC2_TO_GL(point3 , wnd) ,    0.0f , 0.0f , 0.0f ,   U1 , V2 ,
                _VEC2_TO_GL(point4 , wnd) ,    0.0f , 0.0f , 0.0f ,   U2 , V2 ,
            };
            memcpy(vertexStaticTexture + currVertexStaticTextureIdx , arr , sizeof(float) * ((int)RECTANGLE));
            currVertexStaticTextureIdx += (int)RECTANGLE;
            if (owner->currBatchShape == TRIANGLE)
            {
                owner->batch[owner->currBatchIdx + 1] += 2;
            }
            else
            {
                if (owner->currBatchShape == NONE)
                {
                    owner->batch[owner->currBatchIdx] = TRIANGLE;
                    owner->batch[owner->currBatchIdx + 1] = 2;
                    owner->currBatchShape = TRIANGLE;
                }
                else
                {
                    if (owner->currBatchShape == BEGIN_SCISSOR || owner->currBatchShape == END_SCISSOR)
                        owner->currBatchIdx += 1;
                    else
                        owner->currBatchIdx += 2;

                    owner->batch[owner->currBatchIdx] = TRIANGLE;
                    owner->batch[owner->currBatchIdx + 1] = 2;
                    owner->currBatchShape = TRIANGLE;
                }
            }

            x += (ch.advanceX >> 6);
            text++;
        }
    }
    // ============================================================================================================================


    // Batchdraw Scissoring
    void BatchDraw::beginScissor(Rect rct)
    {
        if (currBatchShape != NONE)
            currBatchIdx += 2;

        scissor[currScissorIdx++] = rct;
        batch[currBatchIdx] = BEGIN_SCISSOR;
        currBatchShape = BEGIN_SCISSOR;
    }
    void BatchDraw::endScissor()
    {
        if (currBatchShape != NONE)
            currBatchIdx += 2;
        else if (currBatchShape == BEGIN_SCISSOR)
            currBatchIdx += 1;

        batch[currBatchIdx] = END_SCISSOR;
        currBatchShape = END_SCISSOR;
    }


    // BatchDraw flush
    void BatchDraw::flush()
    {
        if (currBatchShape != NONE && currBatchShape != BEGIN_SCISSOR && currBatchShape != END_SCISSOR)
            currBatchIdx += 2;
        else if (currBatchShape == BEGIN_SCISSOR || currBatchShape == END_SCISSOR)
            currBatchIdx += 1;

        glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D , STA.texID);
        glBindTexture(GL_TEXTURE_2D , Static.texID);

        glBindBuffer(GL_ARRAY_BUFFER , vbo);
        glUniform1i(glGetUniformLocation(program , "tex0") , 0);
        glBufferSubData(GL_ARRAY_BUFFER , 0 , Dynamic.currVertexDynamicPrimitveIdx * sizeof(float) , Dynamic.vertexDynamic);
        glBufferSubData(GL_ARRAY_BUFFER , Dynamic.currVertexDynamicPrimitveIdx * sizeof(float) , Static.currVertexStaticTextureIdx * sizeof(float) , Static.vertexStaticTexture);
        glBindVertexArray(vao);

        int curr = 0;
        int amount = 0;
        int curr_scissor = 0;
        int i = 0;

        // Helper
        static bool once = true;
        if (once)
        {
            debug();
            once = false;
        }

        while (i < currBatchIdx)
        {
            if (batch[i] == TRIANGLE)
            {
                amount = batch[i + 1] * 3;
                glDrawArrays(GL_TRIANGLES , curr , amount);
                i += 2;
            }
            else if (batch[i] == LINE)
            {
                amount = batch[i + 1] * 2;
                glDrawArrays(GL_LINES , curr , amount);
                i += 2;
            }
            else if (batch[i] == POINT)
            {
                amount = batch[i + 1];
                glDrawArrays(GL_POINTS , curr , amount);
                i += 2;
            }
            else if (batch[i] == BEGIN_SCISSOR)
            {
                amount = 0;
                Rect& rct = scissor[curr_scissor];
                glEnable(GL_SCISSOR_TEST);
                glScissor(rct.x , rct.y , rct.w , rct.h);
                curr_scissor++;
                i++;
            }
            else if (batch[i] == END_SCISSOR)
            {
                amount = 0;
                glDisable(GL_SCISSOR_TEST);
                i++;
            }
            curr += amount;
        }

        reset();
    }


    // BatchDraw Helper Functions
    void BatchDraw::debug()
    {
        int j = 0;
        while (j < currBatchIdx)
        {
            if (batch[j] == TRIANGLE)
            {
                std::cout << "TRIANGLE " << batch[j + 1] << " ";
                j += 2;
            }
            else if (batch[j] == LINE)
            {
                std::cout << "LINE " << batch[j + 1] << " ";
                j += 2;
            }
            else if (batch[j] == POINT)
            {
                std::cout << "POINT " << batch[j + 1] << " ";
                j += 2;
            }
            else if (batch[j] == BEGIN_SCISSOR)
            {
                std::cout << "BEGIN_SCISSOR ";
                j += 1;
            }
            else if (batch[j] == END_SCISSOR)
            {
                std::cout << "END_SCISSOR ";
                j += 1;
            }
        }

        std::cout << "| " << currBatchIdx << " | " << Dynamic.currVertexDynamicPrimitveIdx << " | " << currScissorIdx << '\n';

        for (auto& s : Static.TA.shelves)
        {
            std::cout << "{{" << s.current.x << " , " << s.current.y << "} , " << s.height << "}" << '\n';
        }    
    }


    // BatchDraw Ending
    void BatchDraw::reset()
    {
        currBatchIdx = 0;
        currBatchShape = NONE;
        Dynamic.currVertexDynamicPrimitveIdx = 0;
        Dynamic.currVertexDynamicTextureIdx = 0;
        Static.currVertexStaticTextureIdx = 0;
        currScissorIdx = 0;
    }

    void BatchDraw::destroy()
    {
        glDeleteVertexArrays(1 , &vao);
        glDeleteBuffers(1 , &vbo);
        delete[] Dynamic.vertexDynamic;
        delete[] Static.vertexStaticTexture;
    }
}