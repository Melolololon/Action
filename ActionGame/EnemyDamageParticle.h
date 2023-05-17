#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

// �G���a��ꂽ�Ƃ��ɏo�Ă���p�[�e�B�N��
class EnemyDamageParticle :
    public MelLib::GameObject
{
private:
    MelLib::Sprite3D spr;
    const MelLib::Vector3 MOVE_VECTOR;

    static const std::string PARTICLE_NAME;

    float speed = 0.0f;

    MelLib::FrameTimer deadTimer;
public:
    EnemyDamageParticle(const MelLib::Vector3& pos , const MelLib::Vector3& vec);
    ~EnemyDamageParticle();

    void Initialize()override;
    void Update()override;
    void Draw()override;


    static void LoadResources();
};
