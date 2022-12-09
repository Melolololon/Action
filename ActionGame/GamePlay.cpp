#include "GamePlay.h"

#include<SceneEditer.h>
#include<GameObjectManager.h>

#include"StageSelect.h"
#include"Stage.h"

void GamePlay::CheckClear()
{
	
}

void GamePlay::CheckGameOver()
{
	if (Player::GetStaticHP() <= 0)gameState = GameState::GAME_OVER;
}

void GamePlay::Initialize()
{
	const unsigned int STAGE_NUM = StageSelect::GetStageNum();

	// シーンの読み込み
	MelLib::SceneEditer::GetInstance()->LoadEditData("Stage_" + std::to_string(STAGE_NUM));


	// プレイヤーの取得
	pPlayer = MelLib::GameObjectManager::GetInstance()->GetPGameObject("Player_1");
	// 体力とかは、GuiValueManagetから取得する


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
