#pragma once
#include<Scene.h>
#include"Player.h"
class GamePlay : public MelLib::Scene
{
private:
	Player* pPlayer = nullptr;

	enum class GameState 
	{
		PLAY,
		BOSS,
		CLEAR,
		GAME_OVER,
	};
	GameState gameState = GameState::PLAY;
	
	MelLib::FrameTimer sceneChangeTimer;


private:
	void CheckClear();
	void CheckGameOver();

	// ゲーム内シーン
	void Play();
	void Clear();
	void GameOver();

public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン
};

