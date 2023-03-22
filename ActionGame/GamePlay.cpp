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

	// �V�[���̓ǂݍ���
	MelLib::SceneEditer::GetInstance()->LoadEditData("Stage_" + std::to_string(STAGE_NUM));


	// �v���C���[�̎擾
	pPlayer = MelLib::GameObjectManager::GetInstance()->GetPGameObject("Player_1");
	// �̗͂Ƃ��́AGuiValueManaget����擾����


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
