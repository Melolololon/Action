#pragma once
#include <GameObject.h>
#include<FrameTimer.h>
class RecoveryEffect :
    public MelLib::GameObject
{
private:
    static const int SPRITE_NUM = 5;
    std::array<MelLib::Sprite3D, SPRITE_NUM>spriteRing;
    std::array<MelLib::Sprite3D, SPRITE_NUM>spriteStick;

    MelLib::FrameTimer deadTimer;

    float subAlpha = 100.0f;
public:
    static void LoadResources();
public:
 
    RecoveryEffect(const MelLib::Vector3& pos);
    void Initialize()override;
    void Update()override;
    void Draw()override;

};

