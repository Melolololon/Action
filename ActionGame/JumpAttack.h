#pragma once
#include <GameObject.h>
class JumpAttack :
    public MelLib::GameObject
{
public:
    JumpAttack(const MelLib::Vector3& pos,const float radius);
    void Update()override;
    void Draw()override;
};

