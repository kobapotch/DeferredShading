#ifndef _KOBA_ANIMATION_INCLUDED
#define _KOBA_ANIMATION_INCLUDED

#include "Transform.h"
#include "Input.h"

class Animation{

    protected:
        Transform* transform;
        
    public:
        Animation(Transform* trans);
        virtual void Animate() = 0;
};

class RotateAnimation : public Animation{
    private:
        static constexpr float dAngle = 1;
    public:
        RotateAnimation(Transform* trans): Animation(trans){}
        void Animate();
};

class CircleTurningAnimation : public Animation{
    private:
        float angle;
        static constexpr float dAngle = 0.01;
        static constexpr float rad = 50;
    public:
        CircleTurningAnimation(Transform* trans) : Animation(trans) { angle = 0; }
        void Animate();
};

class ArrowKeyAnimation : public Animation{
    private:
        Input* input;
        float theta;
        float phi;
        static constexpr float rad = 50;
        static constexpr float dTheta = 0.02;
        static constexpr float dPhi = 0.02;
    public:
        ArrowKeyAnimation(Transform* trans) : Animation(trans) {
            input = Input::getInstance();
            theta = M_PI/2.0; phi = 0;
        }
        void Animate();
};

#endif
