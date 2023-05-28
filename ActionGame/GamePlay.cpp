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

#include"ClearSprite.h"
#include"GameOverSprite.h"

#include"EnemyDeadCounter.h"

#include"Pause.h"

GamePlay::GamePlayState GamePlay::gameState = GamePlay::GamePlayState::PLAY;

void GamePlay::CheckClear()
{
	
}

void GamePlay::CheckGameOver()
{
	if (pPlayer->GetHP() <= 0)gameState = GamePlayState::GAME_OVER;
}

void GamePlay::Play()
{
	// ボス戦移行
	if(Boss::CheckStateBattle())gameState = GamePlayState::BOSS;

	// ボス戦
	if (BossAliveChecker::GetInstance()->GetBossDeadFlag())
	{
		gameState = GamePlayState::CLEAR;
		crealFadeStartTimer.SetStopFlag(false);

	}


}

void GamePlay::Clear()
{
	if (crealFadeStartTimer.GetMaxOverFlag())Fade::GetInstance()->Start();

	// クリア追加
	if (crealFadeStartTimer.GetNowTime() == 60.0f * 2.0f)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<ClearSprite>());
	}
}

void GamePlay::GameOver()
{
	if (gameOverFadeStartTimer.GetMaxOverFlag())Fade::GetInstance()->Start();

	// クリア追加
	if (gameOverFadeStartTimer.GetNowTime() == 60.0f * 2.0f)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<GameOverSprite>());
	}
}

void GamePlay::Initialize()
{
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, 180, 0));


	EnemyDeadCounter::GetInstance()->Reset();

	// ステージセレクト追加したらコメントアウト解除
	//const unsigned int STAGE_NUM = StageSelect::GetStageNum();
	const unsigned int STAGE_NUM = 1;



	MelLib::SceneEditer::GetInstance()->LoadEditData("_Kesuna_Mendanyou");

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
	//MelLib::GameObjectManager::GetInstance()->InitializeObject();

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
	gameOverFadeStartTimer.SetMaxTime(60 * 5.0f);
	crealFadeStartTimer.SetMaxTime(60 * 5.0f);

	gameState = GamePlayState::PLAY;

	Pause::GetInstance()->Initialize();
}

void GamePlay::Update()
{
	Fade::GetInstance()->Update();

	if(!Pause::GetInstance()->GetIsPause())
	{
		MelLib::GameObjectManager::GetInstance()->Update();
	}
	
	// これの処理シーン側か別クラスでもいいかも
	pPlayer->ChangeMouseCursorShow();
	// プレイヤーにカーソルの表示非表示処理があるため、あえてここに書いてポーズ開始のフレームをずらしている
	Pause::GetInstance()->Update();

#pragma region 共通処理

	if (pPlayer->GetHP() <= 0)
	{
		gameState = GamePlay::GamePlayState::GAME_OVER;
		gameOverFadeStartTimer.SetStopFlag(false);
	}

	// シーン切り替え
	if (Fade::GetInstance()->GetChangeSceneFlag())isEnd = true;
#pragma endregion

	switch (gameState)
	{
	case GamePlay::GamePlayState::CLEAR:
		Clear();
		break;
	case GamePlay::GamePlayState::GAME_OVER:
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
	Pause::GetInstance()->Draw();
	Fade::GetInstance()->Draw();
}

void GamePlay::Finalize()
{
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* GamePlay::GetNextScene()
{
	return new Title();
}
