#pragma once
#include <GameObject2D.h>

#include"Player.h"
class HPGauge :
    public MelLib::GameObject2D
{
private:
    static const Player* pPlayer;

public:
    static void LoadResources();
    static void SetPPlayer(const Player* p) { pPlayer = p; }

    HPGauge();
    void Update()override;
    void Draw()override;
};

