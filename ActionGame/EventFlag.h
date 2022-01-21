#pragma once
#include <GameObject.h>
class EventFlag :
    public MelLib::GameObject
{
private:

public:
    static void LoadResources();

    EventFlag(const MelLib::Vector3& pos,const MelLib::Vector3& scale);

    void Draw()override;

};

