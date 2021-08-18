#include "ActionPart.h"

#include"GameObjectManager.h"

#include"Player.h"

void ActionPart::Initialize()
{
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>(0));

}

void ActionPart::Update()
{
	MelLib::GameObjectManager::GetInstance()->Update();
}

void ActionPart::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void ActionPart::Finalize()
{
}

MelLib::Scene* ActionPart::GetNextScene()
{
	return new ActionPart();
}
