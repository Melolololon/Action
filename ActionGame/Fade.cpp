#include "Fade.h"


Fade* Fade::GetInstance()
{
    static Fade f;
    return &f;
}

void Fade::Initializ()
{
    fadeSpr.Create(MelLib::Color(0, 0, 0, 0));
}

void Fade::Update()
{
    if (!isFade)return;

    static const float FADE_ALPHA_ADD = 2.0f;
    if(fadeIn)fadeAlpha += FADE_ALPHA_ADD;
    else fadeAlpha -= FADE_ALPHA_ADD;
    fadeSpr.SetAddColor(MelLib::Color(0, 0, 0,MelLib::Color::ParToUChar(fadeAlpha)));

    if (!fadeIn && fadeAlpha <= 0.0f)isFade = false;
}

bool Fade::GetChangeSceneFlag()
{
    if(fadeAlpha >= 100.0f)
    {
        fadeIn = false;
        return true;
    }

    return false;
}
