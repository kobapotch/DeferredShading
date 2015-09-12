#ifndef _RENDERTARGET_INCLUDED
#define _RENDERTARGET_INCLUDED

#include "Primitive.h"

class RenderTarget : public Primitive{
    protected:
        void DrawVertex();

    public:
        RenderTarget();

};

#endif
