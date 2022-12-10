#include "GamePlay.h"

#include<SceneEditer.h>
#include<GameObjectManager.h>

#include"StageSelect.h"
#include"Stage.h"

#include"EnemySpaunPoint.h"

void GamePlay::CheckClear()
{
	
}

void GamePlay::CheckGameOver()
{
	if (pPlayer->GetHP() <= 0)gameState = GameState::GAME_OVER;
}

void GamePlay::Initialize()
{
	const unsigned int STAGE_NUM = StageSelect::GetStageNum();

	// シーンの読み込み
	MelLib::SceneEditer::GetInstance()->LoadEditData("Stage_" + std::to_string(STAGE_NUM));


	// プレイヤーの取得
	pPlayer = Player::GetPPlayer();
	EnemySpaunPoint::SetPlayer(pPlayer);
	EnemySpaunPoint::ClearEnemySpauns();


}

void GamePlay::Update()
{

}

void GamePlay::Draw()
{
}

void GamePlay::Finalize()
{
}

MelLib::Scene* GamePlay::GetNextScene()
{
	return nullptr;
}
