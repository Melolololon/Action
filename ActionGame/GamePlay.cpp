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
	// �{�X��ڍs
	if(Boss::CheckStateBattle())gameState = GamePlayState::BOSS;

	// �{�X��
	if (BossAliveChecker::GetInstance()->GetBossDeadFlag())
	{
		gameState = GamePlayState::CLEAR;
		crealFadeStartTimer.SetStopFlag(false);

	}


}

void GamePlay::Clear()
{
	if (crealFadeStartTimer.GetMaxOverFlag())Fade::GetInstance()->Start();

	// �N���A�ǉ�
	if (crealFadeStartTimer.GetNowTime() == 60.0f * 2.0f)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<ClearSprite>());
	}
}

void GamePlay::GameOver()
{
	if (gameOverFadeStartTimer.GetMaxOverFlag())Fade::GetInstance()->Start();

	// �N���A�ǉ�
	if (gameOverFadeStartTimer.GetNowTime() == 60.0f * 2.0f)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<GameOverSprite>());
	}
}

void GamePlay::Initialize()
{
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, 180, 0));


	EnemyDeadCounter::GetInstance()->Reset();

	// �X�e�[�W�Z���N�g�ǉ�������R�����g�A�E�g����
	//const unsigned int STAGE_NUM = StageSelect::GetStageNum();
	const unsigned int STAGE_NUM = 1;



	MelLib::SceneEditer::GetInstance()->LoadEditData("_Kesuna_Mendanyou");

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
	//MelLib::GameObjectManager::GetInstance()->InitializeObject();

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
	
	// ����̏����V�[�������ʃN���X�ł���������
	pPlayer->ChangeMouseCursorShow();
	// �v���C���[�ɃJ�[�\���̕\����\�����������邽�߁A�����Ă����ɏ����ă|�[�Y�J�n�̃t���[�������炵�Ă���
	Pause::GetInstance()->Update();

#pragma region ���ʏ���

	if (pPlayer->GetHP() <= 0)
	{
		gameState = GamePlay::GamePlayState::GAME_OVER;
		gameOverFadeStartTimer.SetStopFlag(false);
	}

	// �V�[���؂�ւ�
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
