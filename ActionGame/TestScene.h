#pragma once
#include <Scene.h>
#include"Player.h"
class TestScene :
    public MelLib::Scene
{
private:
	Player* pPlayer = nullptr;
public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン
};

