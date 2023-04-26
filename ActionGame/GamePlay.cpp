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
	// �{�X��ڍs


	// �{�X��
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

	// �X�e�[�W�Z���N�g�ǉ�������R�����g�A�E�g����
	//const unsigned int STAGE_NUM = StageSelect::GetStageNum();
	const unsigned int STAGE_NUM = 1;

	// �V�[���̓ǂݍ���
	//MelLib::SceneEditer::GetInstance()->LoadEditData("Stage_" + std::to_string(STAGE_NUM));
	

		//Stage::LoadResources(0);
	Stage::LoadResources(1);
	//Stage::LoadResources(2);


	// ���񂱂ꏑ���̑�ς�����A�V���O���g���̃v���C���[�̃_���[�W���炵���肷��N���X��邩�A
	// Player�N���X��static�̃|�C���^�擾�֐���������ق�������
	pPlayer = Player::GetPPlayer();
	EnemySpaunPoint::SetPlayer(pPlayer);
	NewEnemy::SetPPlayer(pPlayer);
	CapsuleEnemyAttack::SetPPlayer(pPlayer);
	BossAttack::SetPPlayer(pPlayer); 
	Boss::SetPPlayer(pPlayer);
	EnemyAttack::SetPPlayer(pPlayer);
	
	EnemySpaunPoint::ClearEnemySpauns();
	MelLib::GameObjectManager::GetInstance()->InitializeObject();

	// UI�ǉ�
	HPGauge::SetPPlayer(pPlayer);
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());

	GameItem::SetPPlayer(pPlayer);




	//// �v���C���[�̎擾
	//pPlayer = Player::GetPPlayer();
	//NewEnemy::SetPPlayer(pPlayer);
	//EnemySpaunPoint::SetPlayer(pPlayer);
	//EnemySpaunPoint::ClearEnemySpauns();

	//// UI�ǉ�
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());

	// ���Ԃ͉��ݒ�
	gameOverFadeStartTimer.SetMaxTime(60 * 0.5);
	crealFadeStartTimer.SetMaxTime(60 * 0.5f);

	gameState = GameState::PLAY;
}

void GamePlay::Update()
{
	// �G�����񂾂�J�E���g���₵�A�J�E���g�����ȏ�ŕǂ���������A�N���A�������肷��΂���?

	MelLib::GameObjectManager::GetInstance()->Update();
	Fade::GetInstance()->Update();

#pragma region ���ʏ���

	if (pPlayer->GetHP() <= 0)
	{
		gameState = GamePlay::GameState::GAME_OVER;
		gameOverFadeStartTimer.SetStopFlag(false);
	}

	// �V�[���؂�ւ�
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
