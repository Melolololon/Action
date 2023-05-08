#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

// 敵が斬られたときに出てくるパーティクル
class EnemyDamageParticle :
    public MelLib::GameObject
{
private:
    MelLib::Sprite3D spr;
    MelLib::Vector3 moveVector;

    static const std::string PARTICLE_NAME;
    float moveSpeed = 0.0f;
    float downSpeed = 0.01f;

    MelLib::FrameTimer deadTimer;

public:

    EnemyDamageParticle(const MelLib::Vector3& pos , const MelLib::Vector3& vec);
    ~EnemyDamageParticle();

    void Initialize()override;
    void Update()override;
    void Draw()override;


    static void LoadResources();
};

