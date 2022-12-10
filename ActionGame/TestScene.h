#pragma once
#include <Scene.h>
#include"Player.h"
class TestScene :
    public MelLib::Scene
{
private:
	Player* pPlayer = nullptr;
public:
	void Initialize()override;//‰Šú‰»
	void Update()override;
	void Draw()override;
	void Finalize()override;//I—¹ˆ—
	Scene* GetNextScene()override;//Ÿ‚ÌƒV[ƒ“
};

