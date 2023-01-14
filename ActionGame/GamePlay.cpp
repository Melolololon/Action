#include "GamePlay.h"

#include"Title.h"

#include"Fade.h"

#include<SceneEditer.h>
#include<GameObjectManager.h>

#include"StageSelect.h"
#include"Stage.h"

#include"EnemySpaunPoint.h"
#include"NewEnemy.h"

#include"HPGauge.h"

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

	// �V�[���̓ǂݍ���
	//MelLib::SceneEditer::GetInstance()->LoadEditData("Stage_" + std::to_string(STAGE_NUM));
	

	// �v���C���[�̎擾
	pPlayer = Player::GetPPlayer();
	NewEnemy::SetPPlayer(pPlayer);
	EnemySpaunPoint::SetPlayer(pPlayer);
	EnemySpaunPoint::ClearEnemySpauns();

	// UI�ǉ�
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());
}

void GamePlay::Update()
{
	MelLib::GameObjectManager::GetInstance()->Update();
	Fade::GetInstance()->Update();


	if (MelLib::Input::KeyTrigger(DIK_Q) && !Fade::GetInstance()->GetIsFade())
	{
		Fade::GetInstance()->Start();
	}
	if (Fade::GetInstance()->GetChangeSceneFlag())isEnd = true;

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
