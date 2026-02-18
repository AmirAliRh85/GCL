#pragma once

#include "./Config.hpp"
#include "./BatchDraw.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"


namespace gcl2
{
    struct Texture
    {
        Vec2 vec2;                  // w , h
        Vec2 UV;                    // U , V
        unsigned char* byte;
    };

    struct TextureAtlas
    {
        BatchDraw* owner;
        
        TextureAtlas(BatchDraw* own);

        Rect rect{0 , 0 , 0 , 0};
        GLuint texID;
        GLuint texUnit;
        Texture textures[20];
        unsigned int currTexturesIdx = 0;

        void init();

        // Creates a horizontal atlas texture
        unsigned int load(const char* file_path);

        void generate();

        void add(float* arr , Rect rct , unsigned int target_index);

        void destroy();
    };
} // End of namespace gcl2