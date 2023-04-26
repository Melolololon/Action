#include "GamePlay.h"

#include"Title.h"

#include"Fade.h"

#include<SceneEditer.h>
#include<GameObjectManager.h>

#include"StageSelect.h"
#include"Stage.h"

#include"EnemySpaunPoint.h"
#include"CapsuleEnemyAttack.h"

#include"Boss.h"
#include"BossAttack.h"
#include"BossAliveChecker.h"
#include"EnemyAttack.h"
#include"NewEnemy.h"

#include"GameItem.h"

#include"HPGauge.h"

#include"EnemyDeadCounter.h"

void GamePlay::CheckClear()
{
	
}

void GamePlay::CheckGameOver()
{
	if (pPlayer->GetHP() <= 0)gameState = GameState::GAME_OVER;
}

void GamePlay::Play()
{
	// ボス戦移行


	// ボス戦
	if (BossAliveChecker::GetInstance()->GetBossDeadFlag())
	{
		gameState = GameState::CLEAR;
		crealFadeStartTimer.SetStopFlag(false);
	}
}

void GamePlay::Clear()
{
	if (crealFadeStartTimer.GetMaxOverFlag())Fade::GetInstance()->Start();

}

void GamePlay::GameOver()
{
	if (gameOverFadeStartTimer.GetMaxOverFlag())Fade::GetInstance()->Start();
}

void GamePlay::Initialize()
{
	EnemyDeadCounter::GetInstance()->Reset();

	// ステージセレクト追加したらコメントアウト解除
	//const unsigned int STAGE_NUM = StageSelect::GetStageNum();
	const unsigned int STAGE_NUM = 1;

	// シーンの読み込み
	//MelLib::SceneEditer::GetInstance()->LoadEditData("Stage_" + std::to_string(STAGE_NUM));
	

		//Stage::LoadResources(0);
	Stage::LoadResources(1);
	//Stage::LoadResources(2);


	// 毎回これ書くの大変だから、シングルトンのプレイヤーのダメージ減らしたりするクラス作るか、
	// Playerクラスにstaticのポインタ取得関数を作ったほうがいい
	pPlayer = Player::GetPPlayer();
	EnemySpaunPoint::SetPlayer(pPlayer);
	NewEnemy::SetPPlayer(pPlayer);
	CapsuleEnemyAttack::SetPPlayer(pPlayer);
	BossAttack::SetPPlayer(pPlayer); 
	Boss::SetPPlayer(pPlayer);
	EnemyAttack::SetPPlayer(pPlayer);
	
	EnemySpaunPoint::ClearEnemySpauns();
	MelLib::GameObjectManager::GetInstance()->InitializeObject();

	// UI追加
	HPGauge::SetPPlayer(pPlayer);
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());

	GameItem::SetPPlayer(pPlayer);




	//// プレイヤーの取得
	//pPlayer = Player::GetPPlayer();
	//NewEnemy::SetPPlayer(pPlayer);
	//EnemySpaunPoint::SetPlayer(pPlayer);
	//EnemySpaunPoint::ClearEnemySpauns();

	//// UI追加
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());

	// 時間は仮設定
	gameOverFadeStartTimer.SetMaxTime(60 * 0.5);
	crealFadeStartTimer.SetMaxTime(60 * 0.5f);

	gameState = GameState::PLAY;
}

void GamePlay::Update()
{
	// 敵が死んだらカウント増やし、カウントが一定以上で壁を消したり、クリアさせたりすればいい?

	MelLib::GameObjectManager::GetInstance()->Update();
	Fade::GetInstance()->Update();

#pragma region 共通処理

	if (pPlayer->GetHP() <= 0)
	{
		gameState = GamePlay::GameState::GAME_OVER;
		gameOverFadeStartTimer.SetStopFlag(false);
	}

	// シーン切り替え
	if (Fade::GetInstance()->GetChangeSceneFlag())isEnd = true;
#pragma endregion

	switch (gameState)
	{
	case GamePlay::GameState::CLEAR:
		Clear();
		break;
	case GamePlay::GameState::GAME_OVER:
		GameOver();
		break;
	default:
		Play();
		break;
	}

}

void GamePlay::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();
	Fade::GetInstance()->Draw();
}

void GamePlay::Finalize()
{
}

MelLib::Scene* GamePlay::GetNextScene()
{
	return new Title();
}
