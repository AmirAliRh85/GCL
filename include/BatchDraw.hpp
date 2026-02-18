// #pragma once

// #include "Config.hpp"
// // #include "./Texture.hpp"
// #include <cstring>
// #include <iostream>
// #define STB_IMAGE_IMPLEMENTATION
// #include "../external/stb/stb_image.h"


// namespace gcl2
// {
//     const unsigned int MAX_DYNAMIC_PRIMITIVE_BATCH_DRAW         = 4096  ;
//     const unsigned int MAX_STATIC_TEXTURE_BATCH_DRAW            = 1024  ;
//     const unsigned int MAX_DYNAMIC_TEXTURE_BATCH_DRAW           = 4096  ;
//     const unsigned int MAX_BATCH_DRAW                           = 8192  ;
//     const unsigned int VERTEX_ATTRIBUTE                         = 7     ; // x , y , r , g , b , u , v


//     enum Shape
//     {
//         TRIANGLE                        = 21    ,
//         LINE                            = 14    ,
//         POINT                           = 7     ,
//         RECTANGLE                       = 42    ,
//         BEGIN_SCISSOR                   = 1     ,
//         END_SCISSOR                     = 2     ,
//         CIRCLE                          = 3     ,
//         TEXTURE                         = 4     ,
//         TEXT                            = 5     ,
//         NONE                            = 0     ,
//     };

//     struct Texture
//     {
//         Vec2 size;                  // w , h
//         Vec2 bottomLeft;
//         Vec2 topRight;
//         unsigned char* byte;
//     };


//     struct BatchDraw
//     {    
//         GLuint program;
//         void setProgram(unsigned int prg)
//         {
//             stbi_set_flip_vertically_on_load(1);
//             program = prg;
//         }

//         GLuint vao , vbo;
//         void init()
//         {
//             glGenVertexArrays(1 , &vao);
//             glGenBuffers(1 , &vbo);

//             glBindVertexArray(vao);
//             glBindBuffer(GL_ARRAY_BUFFER , vbo);
            
//             glBufferData(GL_ARRAY_BUFFER , sizeof(float) * VERTEX_ATTRIBUTE * MAX_DYNAMIC_PRIMITIVE_BATCH_DRAW , NULL , GL_DYNAMIC_DRAW);
//             glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)0);
//             glEnableVertexAttribArray(0);
//             glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)(sizeof(float) * 2));
//             glEnableVertexAttribArray(1);
//             glVertexAttribPointer(2 , 2 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)(sizeof(float) * 5));
//             glEnableVertexAttribArray(2);
//         }


//         int batch[256]                                      ; // [shape , amount , shape , amount , scissor , shape , amount , texture , texture , ...]
//         unsigned int currBatchIdx                   = 0     ;
//         Shape currBatchShape                        = NONE  ;
//         Rect scissor[20]                                    ;
//         unsigned int currScissorIdx                 = 0     ;

//         float* vertexDynamic                        = new float[MAX_DYNAMIC_PRIMITIVE_BATCH_DRAW * VERTEX_ATTRIBUTE];
//         unsigned int currVertexDynamicPrimitveIdx   = 0     ;
//         unsigned int currVertexDynamicTextureIdx    = 0     ;
//         float* vertexStaticTexture                  = new float[MAX_STATIC_TEXTURE_BATCH_DRAW * VERTEX_ATTRIBUTE];
//         unsigned int currVertexStaticTextureIdx     = 0     ;


//         // ======================================
//         // Bathcing for primitive types
//         struct _Dynamic
//         {
//             BatchDraw* owner;
//             _Dynamic(BatchDraw* own) : owner(own) { }

//             void addPrimitive(float* arr , Shape shape)
//             {
//                 memcpy(owner->vertexDynamic + owner->currVertexDynamicPrimitveIdx + owner->currVertexDynamicTextureIdx , arr , ((int)shape) * sizeof(float));
//                 owner->currVertexDynamicPrimitveIdx += (int)shape;
//                 if (owner->currBatchShape == shape)
//                 {
//                     owner->batch[owner->currBatchIdx + 1]++;
//                 }
//                 else
//                 {
//                     if (owner->currBatchShape == NONE)
//                     {
//                         owner->batch[owner->currBatchIdx] = shape;
//                         owner->batch[owner->currBatchIdx + 1] = 1;
//                         owner->currBatchShape = shape;
//                     }
//                     else
//                     {
//                         if (owner->currBatchShape == BEGIN_SCISSOR || owner->currBatchShape == END_SCISSOR)
//                             owner->currBatchIdx += 1;
//                         else
//                             owner->currBatchIdx += 2;

//                         owner->batch[owner->currBatchIdx] = shape;
//                         owner->batch[owner->currBatchIdx + 1] = 1;
//                         owner->currBatchShape = shape;
//                     }
//                 }
//             }
//         };


//         // Batching for Static types
//         struct _Static
//         {
//             Rect rect = {0 , 0 , 0 , 0};
//             GLuint texID;
//             GLuint texUnit;
//             Texture textures[20];
//             unsigned int currTexturesIdx = 0;

//             BatchDraw* owner;
//             _Static(BatchDraw* own) : owner(own) { }
            
//             unsigned int loadTexture(const char* file_path)
//             {
//                 // return owner->STA.load(file_path);
//                 int w , h;
//                 unsigned char* byte = stbi_load(file_path , &w , &h , NULL , 4);
//                 textures[currTexturesIdx++] = {{w , h} , {rect.w , 0} , {rect.w + w , h} , byte};
//                 rect.w += w;
//                 rect.h = std::max(h , rect.h);

//                 return currTexturesIdx - 1;
//             }

//             void generateTexture()
//             {
//                 // owner->STA.generate();
//                 glGenTextures(1 , &texID);
//                 glBindTexture(GL_TEXTURE_2D , texID);
//                 // std::cout << 1 << '\n';

//                 glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA8 , rect.w , rect.h , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);

//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//                 int x_offset = 0;
//                 int y_offset = 0;
//                 for (int i = 0 ; i < currTexturesIdx ; i++)
//                 {
//                     glTexSubImage2D(GL_TEXTURE_2D , 0 , x_offset , y_offset , textures[i].size.x , textures[i].size.y , GL_RGBA , GL_UNSIGNED_BYTE , textures[i].byte);
//                     x_offset += textures[i].size.x;
//                     stbi_image_free(textures[i].byte);
//                 }
//             }

//             void addTexture(float* arr , Rect rct , unsigned int target_index)
//             {
//                 // owner->STA.add(arr , rct , target_index);
//                 float U1 = (float)textures[target_index].bottomLeft.x / rect.w;
//                 float U2 = (float)textures[target_index].topRight.x / rect.w;
//                 arr[5]  = U1     ; arr[6]  =  1.0f ;
//                 arr[12] = U2     ; arr[13] =  1.0f ;
//                 arr[19] = U2     ; arr[20] =  0.0f ;

//                 arr[26] = U1     ; arr[27] =  1.0f ;
//                 arr[33] = U1     ; arr[34] =  0.0f ;
//                 arr[40] = U2     ; arr[41] =  0.0f ;
//                 memcpy(owner->vertexStaticTexture + owner->currVertexStaticTextureIdx , arr , sizeof(float) * ((int)RECTANGLE));
//                 owner->currVertexStaticTextureIdx += (int)RECTANGLE;
//                 if (owner->currBatchShape == TRIANGLE)
//                 {
//                     owner->batch[owner->currBatchIdx + 1] += 2;
//                 }
//                 else
//                 {
//                     if (owner->currBatchShape == NONE)
//                     {
//                         owner->batch[owner->currBatchIdx] = TRIANGLE;
//                         owner->batch[owner->currBatchIdx + 1] = 2;
//                         owner->currBatchShape = TRIANGLE;
//                     }
//                     else
//                     {
//                         if (owner->currBatchShape == BEGIN_SCISSOR || owner->currBatchShape == END_SCISSOR)
//                             owner->currBatchIdx += 1;
//                         else
//                             owner->currBatchIdx += 2;

//                         owner->batch[owner->currBatchIdx] = TRIANGLE;
//                         owner->batch[owner->currBatchIdx + 1] = 2;
//                         owner->currBatchShape = TRIANGLE;
//                     }
//                 }
//             }
//         };
//         // ======================================

//         _Dynamic Dynamic{this};
//         _Static Static{this};
//         // TextureAtlas STA{this};

//         void beginScissor(Rect rct)
//         {
//             if (currBatchShape != NONE)
//                 currBatchIdx += 2;

//             scissor[currScissorIdx++] = rct;
//             batch[currBatchIdx] = BEGIN_SCISSOR;
//             currBatchShape = BEGIN_SCISSOR;
//         }
//         void endScissor()
//         {
//             if (currBatchShape != NONE)
//                 currBatchIdx += 2;
//             else if (currBatchShape == BEGIN_SCISSOR)
//                 currBatchIdx += 1;

//             batch[currBatchIdx] = END_SCISSOR;
//             currBatchShape = END_SCISSOR;
//         }

//         void flush()
//         {
//             if (currBatchShape != NONE && currBatchShape != BEGIN_SCISSOR && currBatchShape != END_SCISSOR)
//                 currBatchIdx += 2;
//             else if (currBatchShape == BEGIN_SCISSOR || currBatchShape == END_SCISSOR)
//                 currBatchIdx += 1;

//             glActiveTexture(GL_TEXTURE0);
//             // glBindTexture(GL_TEXTURE_2D , STA.texID);
//             glBindTexture(GL_TEXTURE_2D , Static.texID);

//             glBindBuffer(GL_ARRAY_BUFFER , vbo);
//             glUniform1i(glGetUniformLocation(program , "tex0") , 0);
//             glBufferSubData(GL_ARRAY_BUFFER , 0 , currVertexDynamicPrimitveIdx * sizeof(float) , vertexDynamic);
//             glBufferSubData(GL_ARRAY_BUFFER , currVertexDynamicPrimitveIdx * sizeof(float) , currVertexStaticTextureIdx * sizeof(float) , vertexStaticTexture);
//             glBindVertexArray(vao);

//             int curr = 0;
//             int amount = 0;
//             int curr_scissor = 0;
//             int i = 0;

//             // Helper
//             static bool once = true;
//             if (once)
//             {
//                 debug();
//                 once = false;
//                 std::cout << "| " << currBatchIdx << " | " << currVertexDynamicPrimitveIdx << " | " << currScissorIdx << '\n';
//             }

//             while (i < currBatchIdx)
//             {
//                 if (batch[i] == TRIANGLE)
//                 {
//                     amount = batch[i + 1] * 3;
//                     glDrawArrays(GL_TRIANGLES , curr , amount);
//                     i += 2;
//                 }
//                 else if (batch[i] == LINE)
//                 {
//                     amount = batch[i + 1] * 2;
//                     glDrawArrays(GL_LINES , curr , amount);
//                     i += 2;
//                 }
//                 else if (batch[i] == POINT)
//                 {
//                     amount = batch[i + 1];
//                     glDrawArrays(GL_POINTS , curr , amount);
//                     i += 2;
//                 }
//                 else if (batch[i] == BEGIN_SCISSOR)
//                 {
//                     amount = 0;
//                     Rect& rct = scissor[curr_scissor];
//                     glEnable(GL_SCISSOR_TEST);
//                     glScissor(rct.x , rct.y , rct.w , rct.h);
//                     curr_scissor++;
//                     i++;
//                 }
//                 else if (batch[i] == END_SCISSOR)
//                 {
//                     amount = 0;
//                     glDisable(GL_SCISSOR_TEST);
//                     i++;
//                 }
//                 curr += amount;
//             }

//             reset();
//         }

//         void reset()
//         {
//             currBatchIdx = 0;
//             currBatchShape = NONE;
//             currVertexDynamicPrimitveIdx = 0;
//             currVertexDynamicTextureIdx = 0;
//             currVertexStaticTextureIdx = 0;
//             currScissorIdx = 0;
//         }

//         void debug()
//         {
//             int j = 0;
//             while (j < currBatchIdx)
//             {
//                 if (batch[j] == TRIANGLE)
//                 {
//                     std::cout << "TRIANGLE " << batch[j + 1] << " ";
//                     j += 2;
//                 }
//                 else if (batch[j] == LINE)
//                 {
//                     std::cout << "LINE " << batch[j + 1] << " ";
//                     j += 2;
//                 }
//                 else if (batch[j] == POINT)
//                 {
//                     std::cout << "POINT " << batch[j + 1] << " ";
//                     j += 2;
//                 }
//                 else if (batch[j] == BEGIN_SCISSOR)
//                 {
//                     std::cout << "BEGIN_SCISSOR ";
//                     j += 1;
//                 }
//                 else if (batch[j] == END_SCISSOR)
//                 {
//                     std::cout << "END_SCISSOR ";
//                     j += 1;
//                 }
//             }
//         }

//         void destroy()
//         {
//             glDeleteVertexArrays(1 , &vao);
//             glDeleteBuffers(1 , &vbo);
//             delete[] vertexDynamic;
//             delete[] vertexStaticTexture;
//         }
//     };
// } // End of namespace gcl2

/*
...
...

int main()
{
    initialize()

    int ID0 = loadTextureStatic(...) // 0
    int ID1 = loadTextureStatic(...) // 1
    int ID2 = loadTextureStatic(...) // 2
    int ID3 = loadTextureStatic(...) // 3
    int ID4 = loadTextureStatic(...) // 4

    int ID5 = loadTextureStatic(...) // 5
    int ID6 = loadTextureStatic(...) // 6


    while(...)
    {
        rend.drawStaticTexture(ID3); 
    }


    quit()
}

*/


#pragma once
#include "Config.hpp"

namespace gcl2
{
    const unsigned int MAX_DYNAMIC_PRIMITIVE_BATCH_DRAW         = 4096  ;
    const unsigned int MAX_STATIC_TEXTURE_BATCH_DRAW            = 1024  ;
    const unsigned int MAX_DYNAMIC_TEXTURE_BATCH_DRAW           = 4096  ;
    const unsigned int MAX_BATCH_DRAW                           = 8192  ;
    const unsigned int VERTEX_ATTRIBUTE                         = 7     ; // x , y , r , g , b , u , v
    const unsigned int ATLAS_WIDTH                              = 1024  ;
    const unsigned int ATLAS_HEIGHT                             = 1024  ;
    const unsigned int ATLAS_SHELF_WIDTH                        = 1024  ;
    const unsigned int ATLAS_SHELF_HEIGHT                       = 256   ;


    enum Shape
    {
        TRIANGLE                        = 21    ,
        LINE                            = 14    ,
        POINT                           = 7     ,
        RECTANGLE                       = 42    ,
        BEGIN_SCISSOR                   = 1     ,
        END_SCISSOR                     = 2     ,
        CIRCLE                          = 3     ,
        TEXTURE                         = 4     ,
        TEXT                            = 5     ,
        NONE                            = 0     ,
    };

    enum Language
    {
        LANGUAGE_ENGLISH                        ,
        LANGUAGE_PERSIAN                        ,
    };
    
    struct Atlas
    {
        static unsigned int width;
        static unsigned int height;
        unsigned int startingX , startingY;
        unsigned int endingX , endingY;

    };
    unsigned int Atlas::width = ATLAS_WIDTH;
    unsigned int Atlas::height = ATLAS_HEIGHT;

    struct Image
    {
        Vec2 size;                  // w , h
        Vec2 bottomLeft;
        Vec2 topRight;
        unsigned char* byte;
    };

    struct Character
    {
        Vec2 size;                  // w , h
        Vec2 bearing;
        Vec2 bottomLeft;
        Vec2 topRight;
        unsigned int advanceX;
    };

    struct BatchDraw
    {    
        GLuint program;
        void setProgram(unsigned int prg);

        GLuint vao , vbo;
        void init();


        int batch[256]                                      ; // [shape , amount , shape , amount , scissor , shape , amount , texture , texture , ...]
        unsigned int currBatchIdx                   = 0     ;
        Shape currBatchShape                        = NONE  ;
        Rect scissor[20]                                    ;
        unsigned int currScissorIdx                 = 0     ;



        // ======================================
        // Bathcing for primitive types
        struct _Dynamic
        {
            float* vertexDynamic                        = new float[MAX_DYNAMIC_PRIMITIVE_BATCH_DRAW * VERTEX_ATTRIBUTE];
            unsigned int currVertexDynamicPrimitveIdx   = 0     ;
            unsigned int currVertexDynamicTextureIdx    = 0     ;

            BatchDraw* owner;
            _Dynamic(BatchDraw* own);


            void addPrimitive(float* arr , Shape shape);
        };


        // Batching for Static types
        struct _Static
        {
            float* vertexStaticTexture                  = new float[MAX_STATIC_TEXTURE_BATCH_DRAW * VERTEX_ATTRIBUTE];
            unsigned int currVertexStaticTextureIdx     = 0     ;
            
            Rect rect = {0 , 0 , 0 , 0};
            GLuint texID;
            GLuint texUnit;
            Image image[20];
            unsigned int currTexturesIdx = 0;

            BatchDraw* owner;
            _Static(BatchDraw* own);

            
            unsigned int loadTexture(const char* file_path);

            void generateTexture();

            void addTexture(float* arr , Rect rct , unsigned int target_index);

            void destroy();

            // unsigned int loadFont()
        };

        _Dynamic Dynamic{this};
        _Static Static{this};

        void beginScissor(Rect rct);
        void endScissor();

        void flush();

        void reset();

        void debug();

        void destroy();
    };
} // End of namespace gcl2

