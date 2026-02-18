#include "./BatchDraw.hpp"
#include <cstring>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"

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

    unsigned int BatchDraw::_Static::loadTexture(const char* file_path)
    {
        // int w , h;
        // unsigned char* byte = stbi_load(file_path , &w , &h , NULL , 4);
        // image[currTexturesIdx++] = {{w , h} , {rect.w , 0} , {rect.w + w , h} , byte};
        // rect.w += w;
        // rect.h = std::max(h , rect.h);

        // return currTexturesIdx - 1;
        
    }

    void BatchDraw::_Static::generateTexture()
    {
        glGenTextures(1 , &texID);
        glBindTexture(GL_TEXTURE_2D , texID);

        glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA8 , rect.w , rect.h , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        int x_offset = 0;
        int y_offset = 0;
        for (int i = 0 ; i < currTexturesIdx ; i++)
        {
            glTexSubImage2D(GL_TEXTURE_2D , 0 , x_offset , y_offset , image[i].size.x , image[i].size.y , GL_RGBA , GL_UNSIGNED_BYTE , image[i].byte);
            x_offset += image[i].size.x;
            stbi_image_free(image[i].byte);
        }
    }

    void BatchDraw::_Static::addTexture(float* arr , Rect rct , unsigned int target_index)
    {
        float U1 = (float)image[target_index].bottomLeft.x / rect.w;
        float U2 = (float)image[target_index].topRight.x / rect.w;
        arr[5]  = U1     ; arr[6]  =  1.0f ;
        arr[12] = U2     ; arr[13] =  1.0f ;
        arr[19] = U2     ; arr[20] =  0.0f ;

        arr[26] = U1     ; arr[27] =  1.0f ;
        arr[33] = U1     ; arr[34] =  0.0f ;
        arr[40] = U2     ; arr[41] =  0.0f ;
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
            std::cout << "| " << currBatchIdx << " | " << Dynamic.currVertexDynamicPrimitveIdx << " | " << currScissorIdx << '\n';
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