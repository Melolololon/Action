#pragma once
#include<Scene.h>
#include"Player.h"
class GamePlay : public MelLib::Scene
{
private:
	MelLib::GameObject* pPlayer = nullptr;

	enum class GameState 
	{
		PLAY,
		CLEAR,
		GAME_OVER,
	};
	GameState gameState = GameState::PLAY;
private:
	void CheckClear();
	void CheckGameOver();

public:
	void Initialize()override;//‰Šú‰»
	void Update()override;
	void Draw()override;
	void Finalize()override;//I—¹ˆ—
	Scene* GetNextScene()override;//Ÿ‚ÌƒV[ƒ“
};

