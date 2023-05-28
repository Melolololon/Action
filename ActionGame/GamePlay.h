#pragma once
#include<Scene.h>
#include"Player.h"
class GamePlay : public MelLib::Scene
{
public:
	enum class GamePlayState
	{
		PLAY,
		BOSS,
		CLEAR,
		GAME_OVER,
	};


private:
	Player* pPlayer = nullptr;


	MelLib::FrameTimer crealFadeStartTimer;
	MelLib::FrameTimer gameOverFadeStartTimer;

	static GamePlayState gameState;

	bool isRestart = false;
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

	static GamePlayState GetState() { return gameState; }
};

