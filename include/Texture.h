
#ifndef _KOBA_TEXTURE_INCLUDED
#define _KOBA_TEXTURE_INCLUDED

#include <opencv2/opencv.hpp>
#include <vector>

class Texture{

    int ID;
    // シェーダのテクスチャスロット番号
    int unit;
    static const int unitMacro[];

    int ubo;

    unsigned int dataPos;
    unsigned int width,height;
    int elemSize;
    unsigned int imageSize;
    cv::Mat image;
    GLuint textureID;

public:
    void loadTexture(const char* filename);
    GLuint makeTexture(uint shaderID,int unit);
    GLuint getTextureID();
    void setTexture();

};

#endif
