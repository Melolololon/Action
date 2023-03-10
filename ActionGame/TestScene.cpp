#include "TestScene.h"

#include"Stage.h"
#include"EnemySpaunPoint.h"
#include"NewEnemy.h"
#include"WeakEnemy.h"
#include"CapsuleEnemyAttack.h"

#include"HPGauge.h"

#include<GameObjectManager.h>

#include"GameItem.h"

#include"BossAttack.h"


void TestScene::Initialize()
{
	
	//Stage::LoadResources(0);
	Stage::LoadResources(1);
	//Stage::LoadResources(2);


	// 毎回これ書くの大変だから、シングルトンのプレイヤーのダメージ減らしたりするクラス作るか、
	// Playerクラスにstaticのポインタ取得関数を作ったほうがいい
	pPlayer = Player::GetPPlayer();
	EnemySpaunPoint::SetPlayer(pPlayer);
	NewEnemy::SetPPlayer(pPlayer);
	CapsuleEnemyAttack::SetPPlayer(pPlayer);
	BossAttack::SetPPlayer(pPlayer);

	EnemySpaunPoint::ClearEnemySpauns();
	MelLib::GameObjectManager::GetInstance()->InitializeObject();


	// 当たり判定確認用
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<WeakEnemy>(0));

	// UI追加
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
