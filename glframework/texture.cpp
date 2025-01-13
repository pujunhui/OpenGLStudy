#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

#include <math.h>

Texture::Texture(const std::string& path, unsigned int unit) {
    mUnit = unit;

    //1 stbImage 读取图片
    int channels;

    //--反转y轴
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

    //2 生成纹理并且激活单元绑定
    glGenTextures(1, &mTexture);
    //--激活纹理单元--
    glActiveTexture(GL_TEXTURE0 + mUnit);
    //--绑定纹理对象--
    glBindTexture(GL_TEXTURE_2D, mTexture);

    //3 传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //自动生成mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    //手动生成mipmap，注意：这里数据是错误的
    //int width = mWidth;
    //int height = mHeight;
    //for (int i = 0; true; i++) {
    //    glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //    if (width == 1 && height == 1) {
    //        break;
    //    }
    //    width = std::max(width / 2, 1);
    //    height = std::max(height / 2, 1);
    //}


    //***释放数据
    stbi_image_free(data);

    //4 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //GL_NEAREST_MIPMAP_LINEAR 前面的NEAREST表示单张图片的采样方式，LINEAR表示位于2个层级之间的采样方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    //5 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
}


Texture::~Texture() {
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }
}

void Texture::bind() {
    //先切换纹理单元，然后绑定texture对象
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}