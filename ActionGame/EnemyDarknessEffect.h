#pragma once
#include<GameObject.h>
#include<FrameTimer.h>
class EnemyDarknessEffect :public MelLib::GameObject
{
private:
    MelLib::Sprite3D sprite;
    MelLib::FrameTimer deadTimer;

    float subAlpha = 100.0f;
public:
    static void LoadResources();
public:

    EnemyDarknessEffect(const MelLib::Vector3& pos);
    void Initialize()override;
    void Update()override;
    void Draw()override;
};

