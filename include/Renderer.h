#ifndef _RENDERER_INCLUDED
#define _RENDERER_INCLUDED

#include <memory>

#include "SceneManager.h"
#include "Logger.h"
#include "RenderTarget.h"


// 遅延シェーディング Gバッファ
// １枚目：位置
// ２枚目：法線
// ３枚目：反射率
class Renderer{
    private:

        // 画面サイズ
        uint width,height;
        // シェーダハンドラ
        std::shared_ptr<ShaderManager> shader;
        // パスごとのサブルーチン
        GLuint pass1,pass2;

        // フレームバッファオブジェクト
        GLuint deferredFBO;
        // gバッファを構成するテクスチャ
        GLuint depthBuf,posTex,normTex,colorTex,materialTex;
        // gバッファテクスチャのロケーション
        GLuint positionLoc,normalLoc,colorLoc,materialLoc;
        
        // パス２描画用ポリゴン
        RenderTarget renderTarget;

        // 描画対象のシーン
        std::shared_ptr<SceneManager> scene;

        void makeBuffers();

    public:
        Renderer(uint width, uint height);
        void init(std::shared_ptr<ShaderManager>);
        void setScene(std::shared_ptr<SceneManager> scene);
        void render();

};

#endif
