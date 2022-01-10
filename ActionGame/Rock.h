#pragma once
#include "StageObject.h"

class Rock :
    public StageObject
{
public:

    static void LoadResources();
    
    Rock(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector3& scale,int rockType);
};

