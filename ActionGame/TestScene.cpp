#include "TestScene.h"

#include"Stage.h"

#include<GameObjectManager.h>

void TestScene::Initialize()
{
	Stage::LoadResources(0);

	// ステージを追加
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Stage>(0));
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
