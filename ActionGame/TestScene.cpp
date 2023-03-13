#include "TestScene.h"

#include"Stage.h"
#include"EnemySpaunPoint.h"
#include"NewEnemy.h"
#include"WeakEnemy.h"
#include"CapsuleEnemyAttack.h"

#include"HPGauge.h"

#include<GameObjectManager.h>

#include"GameItem.h"

#include"Boss.h"
#include"BossAttack.h"
#include"EnemyAttack.h"


void TestScene::Initialize()
{
	
	Stage::LoadResources(0);
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


	// �����蔻��m�F�p
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<WeakEnemy>(0));

	// UI�ǉ�
	HPGauge::SetPPlayer(pPlayer);
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());

	GameItem::SetPPlayer(pPlayer);
}

void TestScene::Update()
{
	MelLib::GameObjectManager::GetInstance()->Update();
}

void TestScene::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();

	
}

void TestScene::Finalize()
{
}

MelLib::Scene* TestScene::GetNextScene()
{
	return new TestScene();
}
