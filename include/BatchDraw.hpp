#pragma once
#include "Config.hpp"
#include <vector>
#include <map>

#define RGBA_TO_GL(r , g , b , a) r / 255.0f , g / 255.0f , b / 255.0f , a / 255.f
#define RGB_TO_GL(r , g , b) r / 255.0f , g / 255.0f , b / 255.0f
#define VEC2_TO_GL(vec , window) ((float)vec.x * (2.0f / window.width)) - 1 , ((float)-(vec.y) * (2.0f / window.height)) + 1
#define _VEC2_TO_GL(vec , wnd) ((float)vec.x * (2.0f / wnd.x)) - 1 , ((float)-(vec.y) * (2.0f / wnd.y)) + 1

namespace gcl2
{
    const unsigned int MAX_DYNAMIC_PRIMITIVE_BATCH_DRAW         = 4096      ;
    const unsigned int MAX_STATIC_TEXTURE_BATCH_DRAW            = 1024      ;
    const unsigned int MAX_DYNAMIC_TEXTURE_BATCH_DRAW           = 4096      ;
    const unsigned int MAX_BATCH_DRAW                           = 8192      ;
    const unsigned int VERTEX_ATTRIBUTE                         = 7         ; // x , y , r , g , b , u , v

    const unsigned int ATLAS_WIDTH                              = 1024.0f   ;
    const unsigned int ATLAS_HEIGHT                             = 1024.0f   ;


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


    struct Shelf
    {
        Vec2 current;
        int height;
    };
    
    struct TextureAtlas
    {
        float width;
        float height;
        int heightOccupied = 0;
        std::vector<Shelf> shelves;
        bool isInitialized = false;
    };

    struct Image
    {
        Vec2 bottomLeft;
        Vec2 topRight;
    };

    struct Character
    {
        Vec2 size;
        Vec2 bearing;
        Vec2 bottomLeft;
        Vec2 topRight;
        signed long advanceX;
    };

    struct EnglishFont
    {
        unsigned int fontSize;
        std::map<char , Character> characters;
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
            
            GLuint texID;
            GLuint texUnit;
            Image image[20];
            unsigned int currTexturesIdx = 0;
            EnglishFont EF[5];
            unsigned int currEFIdx = 0;

            TextureAtlas TA;

            BatchDraw* owner;
            _Static(BatchDraw* own);

            
            unsigned int loadImage(const char* file_path);

            void addImage(float* arr , unsigned int target_index);

            unsigned int loadFont(const char* font_path , unsigned int font_size , Language lang);

            void addText(Vec2 point , const char* text , unsigned int font_id , Vec2 wnd);
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

