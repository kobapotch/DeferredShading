#ifndef _KOBA_CUBEMAP_INCLUDED
#define _KOBA_CUBEMAP_INCLUDED

#include <vector>
#include <opencv2/opencv.hpp>

class CubeMap{ 
    private:
        int ubo;
        int unit;


        // posx,negx ... negzの順に画像データを格納する
        cv::Mat image[6];

        unsigned int textureID;
        static const int unitMacro[];

    public:
        void loadCubeTex(const char* posx,const char* negx, 
                const char* posy,const char* negy,
                const char* posz, const char* negz);
        void makeCubeMap(unsigned int shaderID,int unit);
        void setCubeMap();

};

#endif
