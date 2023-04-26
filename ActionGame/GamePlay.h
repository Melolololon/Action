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

	// �Q�[�����V�[��
	void Play();
	void Clear();
	void GameOver();

public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��
};

