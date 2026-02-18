#include <iostream>
#include "./Texture.hpp"

gcl2::TextureAtlas::TextureAtlas(BatchDraw* own) : owner(own) { }

void gcl2::TextureAtlas::init()
{
    std::cout << 1 << '\n';
    glGenTextures(1 , &texID);
    glBindTexture(GL_TEXTURE_2D , texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// Creates a horizontal atlas texture
unsigned int gcl2::TextureAtlas::load(const char* file_path)
{
    int w , h;
    unsigned char* byte = stbi_load(file_path , &w , &h , NULL , 4);
    textures[currTexturesIdx++] = {{w , h} , {rect.w , 1} , byte};
    rect.w += w;
    rect.h = std::max(h , rect.h);
    return currTexturesIdx - 1;
}

void gcl2::TextureAtlas::generate()
{
    glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA8 , rect.w , rect.h , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);
    int x_offset = 0;
    int y_offset = 0;
    for (int i = 0 ; i < currTexturesIdx ; i++)
    {
        glTexSubImage2D(GL_TEXTURE_2D , 0 , x_offset , y_offset , textures[i].vec2.x , textures[i].vec2.y , GL_RGBA , GL_UNSIGNED_BYTE , textures[i].byte);
        x_offset += textures[i].vec2.x;
        stbi_image_free(textures[i].byte);
    }
}

void gcl2::TextureAtlas::add(float* arr , Rect rct , unsigned int target_index)
{
    float U = (float)textures[target_index].UV.x / rect.w;
    arr[5]  = 0.0f  ; arr[6]  =  1.0f ;
    arr[12] = U     ; arr[13] =  1.0f ;
    arr[19] = U     ; arr[20] =  0.0f ;

    arr[26] = 0.0f  ; arr[27] =  1.0f ;
    arr[33] = 0.0f  ; arr[34] =  0.0f ;
    arr[40] = U     ; arr[41] =  0.0f ;
    memcpy(owner->currVertexStaticTextureIdx + owner->vertexStaticTexture , arr , sizeof(float) * RECTANGLE);
    owner->currVertexStaticTextureIdx += (int)RECTANGLE;
}

void gcl2::TextureAtlas::destroy()
{
    glDeleteTextures(1 , &texID);
}