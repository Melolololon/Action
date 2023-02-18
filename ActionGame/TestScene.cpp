#include "TestScene.h"

#include"Stage.h"
#include"EnemySpaunPoint.h"
#include"NewEnemy.h"
#include"WeakEnemy.h"
#include"CapsuleEnemyAttack.h"

#include"HPGauge.h"

#include<GameObjectManager.h>

#include"GameItem.h"


void TestScene::Initialize()
{
	Stage::LoadResources(0);
	Stage::LoadResources(2);


	pPlayer = Player::GetPPlayer();
	EnemySpaunPoint::SetPlayer(pPlayer);
	NewEnemy::SetPPlayer(pPlayer);
	CapsuleEnemyAttack::SetPPlayer(pPlayer);
	EnemySpaunPoint::ClearEnemySpauns();
	MelLib::GameObjectManager::GetInstance()->InitializeObject();


	// “–‚½‚è”»’èŠm”F—p
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<WeakEnemy>(0));

	// UI’Ç‰Á
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
