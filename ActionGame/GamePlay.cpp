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

void GamePlay::Initialize()
{
	EnemyDeadCounter::GetInstance()->Reset();

	const unsigned int STAGE_NUM = StageSelect::GetStageNum();

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

	EnemySpaunPoint::ClearEnemySpauns();
	MelLib::GameObjectManager::GetInstance()->InitializeObject();


	// �����蔻��m�F�p
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<WeakEnemy>(0));

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
}

void GamePlay::Update()
{
	// �G�����񂾂�J�E���g���₵�A�J�E���g�����ȏ�ŕǂ���������A�N���A�������肷��΂���?

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
